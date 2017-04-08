#pragma once

// Include headers accessible 
#include "Globals.h"

class Border : public MainObject
{
public:
	Border(float x, float y);
	Border(float x, float y, float scale);
	Border();
	void Reset();
	void Update(float speed, float deltaTime);
	void Collided(MainObject *otherObject, float deltaTime);
};