#include <math.h>
#include <raylib.h>

#include "engine.h"

//Main game
namespace Game
{
	// Snake fragment, they will be represnted as a one-way linked-list
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

			if (position.x >= Engine::GAME_SIZE)
				position.x = 0;
			else if (position.x < 0)
				position.x = Engine::GAME_SIZE;

			if (position.y >= Engine::GAME_SIZE)
				position.y = 0;
			else if (position.y < 0)
				position.y = Engine::GAME_SIZE;
		}
		//Checks if the provided position matches the pieces current position
		bool PositionMatches(const Vector2& other) const
		{
			return (int)position.x == (int)other.x && (int)position.y == (int)other.y;
		}
	};
	
	// Snake variables
	static SnakePiece* head;
	static SnakePiece* tail;
	static unsigned int snakeLength;
	static Vector2 currentDirection;

	// Score varaibles
	static Vector2 applePosition;

	// Game state variables
	static bool paused;
	static bool gameOver;
	static double gameOverTime;

	// Texts to display when the game is paused
	static const char *pauseTexts[] = {
		"PRESS [P] TO RESUME",
		"PRESS [ESC] TO EXIT"};
	// Text to display on game over
	static const char *gameOverText = "GAME OVER!";

	// Puts the apple on a random position
	static void ChangeApplePosition()
	{
		int cellAmount = Engine::GAME_SIZE / Engine::CELL_SIZE - 1;
		while(true)
		{
			applePosition.x = Engine::CELL_SIZE * GetRandomValue(0, cellAmount);
			applePosition.y = Engine::CELL_SIZE * GetRandomValue(0, cellAmount);
			SnakePiece* current = tail;
			bool validPosition = true;
			while(current != nullptr)
			{
				// If random position is on the snake, then mark it as invalid breake out of the loop
				if(current->PositionMatches(applePosition))
				{
					validPosition = false;
					break;
				}
				current = current->prev;
			}
			// If after checking apple's position is still valid then breake out of this loop
			if(validPosition)
				break;
		}
	}

	// Initializes the game and cleans up after the previous game (should only be called when initializing)
	void Init()
	{
		// Delete the previous snake
		SnakePiece* current = tail;
		while(current != nullptr)
		{
			SnakePiece* toDestroy = current;
			current = current->prev;
			delete toDestroy;
		}
		// Create a new head piece at the center of the screen
		head = new SnakePiece((Vector2){ (float)Engine::GAME_SIZE / 2, (float)Engine::GAME_SIZE / 2 });
		tail = head;
		snakeLength = 1;
		currentDirection.x = 0;
		currentDirection.y = 0;
		paused = false;
		gameOver = false;
		// Initialize the apple
		ChangeApplePosition();
	}

	//Adds a new piece to the snake at the end of it's tail
	static void ExtendTail()
	{
		SnakePiece* newTail = new SnakePiece(tail);
		tail = newTail;
		Engine::Scoring::AddToCurrentScore(snakeLength++ * 10);
	}

	// Updates the current snake's heading direction
	static void UpdateSnakeDirection()
	{		
		// Get the new direction of the snake
		// Snake can't do a 180 degree turn, so the input is valid only when the oposite axis value is 0
		// Use can use WASD or arrow keys for movement
		if ((IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) && currentDirection.y == 0)
		{
			currentDirection.x = 0;
			currentDirection.y = -Engine::CELL_SIZE;
		}
		else if ((IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) && currentDirection.y == 0)
		{
			currentDirection.x = 0;
			currentDirection.y = Engine::CELL_SIZE;
		}
		else if ((IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) && currentDirection.x == 0)
		{
			currentDirection.x = -Engine::CELL_SIZE;
			currentDirection.y = 0;
		}
		else if ((IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) && currentDirection.x == 0)
		{
			currentDirection.x = Engine::CELL_SIZE;
			currentDirection.y = 0;
		}
	}

	// Updates each snake's piece position
	static void UpdateSnakePosition()
	{
		SnakePiece *current = tail;
		while (current != nullptr)
		{
			if (current->prev == nullptr)
				break;

			current->position = current->prev->position;
			current = current->prev;
		}

		// Move the head by the direction vector
		head->AddDirection(currentDirection);
	}

	// Checks snake's collision against itself and against the apple
	static void CheckSnakeCollisions()
	{
		// Detect snake collisions with each of it's pieces
		// Start at the second element (head can't collide with itself)
		// Collisions can only occure with the head, since every other piece can only follow its predecesor
		SnakePiece* current = tail;
		while (current != head)
		{
			if (current->PositionMatches(head->position))
			{
				// If the collision is detected signal the game over
				gameOver = true;
				gameOverTime = GetTime();
				Engine::Scoring::SetNewBestScore();
				return;
			}
			current = current->prev;
		}

		// Detect apple collision
		if (head->PositionMatches(applePosition))
		{
			// If collision is detected then extend the tail and move the apple
			ExtendTail();
			ChangeApplePosition();
		}
	}

	// Updates the game stata
	void Update()
	{
		// If the game is over wait for 3 seconds and then return to the menu
		if(gameOver)
		{
			if (GetTime() - gameOverTime > 3.0)
				Engine::GameState::SetGameState(Engine::GameState::IN_MENU);
			return;
		}

		// Check and execute the pause
		if (IsKeyPressed(KEY_P))
			paused = !paused;
		if (paused)
		{
			if (IsKeyPressed(KEY_ESCAPE))
				Engine::GameState::SetGameState(Engine::GameState::IN_MENU);
			return;
		}

		UpdateSnakeDirection();
		UpdateSnakePosition();
		CheckSnakeCollisions();
	}

	//Draws the current game state
	void Draw()
	{
		//Draw the snake
		SnakePiece *current = tail;
		while (current != nullptr)
		{
			DrawRectangle(current->position.x, current->position.y, Engine::CELL_SIZE, Engine::CELL_SIZE, GREEN);
			current = current->prev;
		}

		//Draw the apple
		DrawRectangle(applePosition.x, applePosition.y, Engine::CELL_SIZE, Engine::CELL_SIZE, RED);
		
		//Draw the game over or pause screen (both will never be active at once)
		if (gameOver)
		{
			int xPos = (Engine::GAME_SIZE - MeasureText(gameOverText, Engine::TITLE_FONT_SIZE)) * 0.5f;
			int yPos = (Engine::GAME_SIZE - Engine::TITLE_FONT_SIZE * 0.5f) * 0.5f;
			DrawText(gameOverText, xPos, yPos, Engine::TITLE_FONT_SIZE, BLACK);
		}
		else if (paused)
		{
			//Flash the text (1s on, 1s off)
			if (sin(GetTime() * 2 * PI) > 0)
			{
				for (int i = 0; i < 2; i++)
				{
					int xPos = (Engine::GAME_SIZE - MeasureText(pauseTexts[i], Engine::TITLE_FONT_SIZE)) * 0.5f;
					int yPos = (Engine::GAME_SIZE - Engine::TITLE_FONT_SIZE * 2) * 0.5f + i * Engine::TITLE_FONT_SIZE + 5;
					DrawText(pauseTexts[i], xPos, yPos, Engine::TITLE_FONT_SIZE, BLACK);
				}
			}
		}

		//Lastly draw the score label
		const char* scoreText = TextFormat("SCORE: %u", Engine::Scoring::GetCurretScore());
		DrawText(scoreText, (Engine::GAME_SIZE - MeasureText(scoreText, Engine::FONT_SIZE)) * 0.5f, 10, Engine::FONT_SIZE, BLACK);
	}
}
