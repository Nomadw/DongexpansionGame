#pragma once

// Include headers accessible 
#include "Globals.h"

class Ball : public MainObject
{
public:
	Ball(int ID);
	void Update(float speed, float deltaTime);
	void Reset();
	float angle;
	void Collided(MainObject *otherObject, float deltaTime);
	float originX;
	float originY;
};