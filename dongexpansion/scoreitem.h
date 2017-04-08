#pragma once

// Include headers accessible 
#include "Globals.h"

class ScoreItem : public MainObject
{
public:
	ScoreItem(float no, int scoreID);
	void Update(float CameraY, float CameraX, float score);
};