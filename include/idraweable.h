#pragma once

//Interface that allows an object to be drawed
class IDraweable
{
public:
	virtual void Draw(float deltaTime) const = 0;
};

