#pragma once

//Interface that allows an object to be updated
class IUpdateable
{
public:
	virtual void Update(float deltaTime) = 0;
};
