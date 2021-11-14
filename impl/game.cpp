#pragma once
#include <math.h>
#include <stdio.h>

#include "config.h"
#include "raylib.h"

//Main game
namespace Game
{
	//Snake fragment, they will be represnted as a one-way linked-list
	struct SnakePiece
	{
		Vector2 position; //Current position of this fragment
		SnakePiece* prev; //Pointer to the next piece of the snake

		SnakePiece(const Vector2& startPosition) : position(startPosition), prev(nullptr) {}
		SnakePiece() : prev(nullptr) {}
		SnakePiece(SnakePiece *previous) : position(previous->position), prev(previous) {}
		//Adds a direction vector to the current position and flips it to the other side of the screen when the piece exits it (used for moving the head)
		void AddDirection(const Vector2& direction)
		{
			position.x += direction.x;
			position.y += direction.y;

			if (position.x >= Config::GAME_SIZE)
				position.x = 0;
			else if (position.x < 0)
				position.x = Config::GAME_SIZE;

			if (position.y >= Config::GAME_SIZE)
				position.y = 0;
			else if (position.y < 0)
				position.y = Config::GAME_SIZE;
		}
		//Checks if the provided position matches the pieces current position
		bool PositionMatches(const Vector2& other) const
		{
			return position.x == other.x && position.y == other.y;
		}
	};
	
	//Snake variables
	SnakePiece* head;
	SnakePiece* tail;
	unsigned int snakeLength;
	Vector2 currentDirection;

	//Score varaibles
	unsigned int currentScore = 0;
	Vector2 applePosition;

	//Game state variables
	bool paused;
	bool gameOver;
	double gameOverTime;

	//Texts to display when the game is paused
	const char* PAUSED_TEXTS[] = {
		"PRESS [P] TO RESUME",
		"PRESS [ESC] TO EXIT"
	};
	//Text to display on game over
	const char* GAME_OVER_TEXT = "GAME OVER!";

	//Puts the apple on a random position
	void ChangeApplePosition()
	{
		int cellAmount = Config::GAME_SIZE / Config::CELL_SIZE - 1;
		while(true)
		{
			applePosition.x = Config::CELL_SIZE * GetRandomValue(0, cellAmount);
			applePosition.y = Config::CELL_SIZE * GetRandomValue(0, cellAmount);
			SnakePiece* current = tail;
			while(current != nullptr)
			{
				if(current->PositionMatches(applePosition))
					break;
				current = current->prev;
			}
			break;
		}
	}

	//Initializes the game and cleans up after the previous game (should only be called when initializing)
	void Init()
	{
		SnakePiece* current = tail;
		while(current != nullptr)
		{
			SnakePiece* toDestroy = current;
			current = current->prev;
			delete toDestroy;
		}
		head = new SnakePiece((Vector2){ Config::GAME_SIZE / 2, Config::GAME_SIZE / 2 });
		tail = head;
		snakeLength = 1;
		currentDirection.x = 0;
		currentDirection.y = 0;
		paused = false;
		gameOver = false;
		ChangeApplePosition();
	}

	//Adds a new piece to the snake at the end of it's tail
	void ExtendTail()
	{
		SnakePiece* newTail = new SnakePiece(tail);
		tail = newTail;
		currentScore += snakeLength++ * 10;
		if(currentScore > Config::MAX_SCORE)
			currentScore = Config::MAX_SCORE;
	}

	//Updates the game stata
	void Update()
	{
		//If the game is over wait for 3 seconds and then return to the menu
		if(gameOver)
		{
			if (GetTime() - gameOverTime > 3.0)
				Config::PLAYING = false;
			return;
		}

		//Check and execute the pause
		if (IsKeyPressed(KEY_P)) paused = !paused;
		if (paused)
		{
			if (IsKeyPressed(KEY_ESCAPE))
				Config::PLAYING = false;
			return;
		}

		//Get the new direction of the snake
		//Snake can't do a 180 degree turn, so the input is valid only when the oposite axis value is 0
		//Use can use WASD or arrow keys for movement
		if ((IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) && currentDirection.y == 0)
		{
			currentDirection.x = 0;
			currentDirection.y = -Config::CELL_SIZE;
		}
		else if ((IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) && currentDirection.y == 0)
		{
			currentDirection.x = 0;
			currentDirection.y = Config::CELL_SIZE;
		}
		else if ((IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) && currentDirection.x == 0)
		{
			currentDirection.x = -Config::CELL_SIZE;
			currentDirection.y = 0;
		}
		else if ((IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) && currentDirection.x == 0)
		{
			currentDirection.x = Config::CELL_SIZE;
			currentDirection.y = 0;
		}

		//Update snake's position
		SnakePiece* current = tail;
		while (current != nullptr)
		{
			if(current->prev == nullptr)
				break;
			
			current->position = current->prev->position;
			current = current->prev;
		}

		//Move the head by the direction vector
		head->AddDirection(currentDirection);

		//If compiled with the debug flag then the player can extend the tail by pressing space
		#ifdef DEBUG
		if (IsKeyPressed(KEY_SPACE))
			ExtendTail();
		#endif

		//Detect snake collisions with each of it's pieces
		//Start at the second element (head can't collide with itself)
		//Collisions can only occure with the head, since every other piece can only follow its predecesor
		current = tail;
		while (current != head)
		{
			if (current->PositionMatches(head->position))
			{
				//If the collision is detected signal the game over
				gameOver = true;
				gameOverTime = GetTime();
				if(currentScore > Config::CURRENT_HIGH_SCORE)
					Config::CURRENT_HIGH_SCORE = currentScore;
				return;
			}
			current = current->prev;
		}

		//Detect apple collision
		if (head->PositionMatches(applePosition))
		{
			//If collision is detected then extend the tail and move the apple
			ExtendTail();
			ChangeApplePosition();
		}	
	}

	//Draws the current game state
	void Draw()
	{
		//Draw the snake
		SnakePiece *current = tail;
		while (current != nullptr)
		{
			DrawRectangle(current->position.x, current->position.y, Config::CELL_SIZE, Config::CELL_SIZE, GREEN);
			current = current->prev;
		}

		//Draw the apple
		DrawRectangle(applePosition.x, applePosition.y, Config::CELL_SIZE, Config::CELL_SIZE, RED);
		
		//Draw the game over or pause screen (both will never be active at once)
		if (gameOver)
		{
			int xPos = (Config::GAME_SIZE - MeasureText(GAME_OVER_TEXT, Config::TITLE_FONT_SIZE)) * 0.5f;
			int yPos = (Config::GAME_SIZE - Config::TITLE_FONT_SIZE * 0.5f) * 0.5f;
			DrawText(GAME_OVER_TEXT, xPos, yPos, Config::TITLE_FONT_SIZE, BLACK);
		}
		else if (paused)
		{
			//Flash the text (1s on, 1s off)
			if (sin(GetTime() * 2 * PI) > 0)
			{
				for (int i = 0; i < 2; i++)
				{
					int xPos = (Config::GAME_SIZE - MeasureText(PAUSED_TEXTS[i], Config::TITLE_FONT_SIZE)) * 0.5f;
					int yPos = (Config::GAME_SIZE - Config::TITLE_FONT_SIZE * 2) * 0.5f + i * Config::TITLE_FONT_SIZE + 5;
					DrawText(PAUSED_TEXTS[i], xPos, yPos, Config::TITLE_FONT_SIZE, BLACK);
				}
			}
		}

		//Lastly draw the score label
		const char* scoreText = TextFormat("SCORE: %u", currentScore);
		DrawText(scoreText, (Config::GAME_SIZE - MeasureText(scoreText, Config::FONT_SIZE)) * 0.5f, 10, Config::FONT_SIZE, BLACK);
	}
}
