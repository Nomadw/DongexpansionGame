#pragma once

// Include headers accessible 
#include "Globals.h"

class Background : public MainObject
{
public:
	Background();
	void Update(float deltaTime);
	void Reset();
	float angle;
	float originX;
};