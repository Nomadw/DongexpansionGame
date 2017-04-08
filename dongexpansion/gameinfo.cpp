#include "stdafx.h"
#include "Globals.h"
// Include headers accessible 

#include "gameinfo.h"

// this sets up the default values that are common to all game instances
gameInfo::gameInfo()
{
	frameDelta = IDEAL_DELTA;

	deltaTime = 0.0f;

	pause = false;

	// initialise the other frame delta variables...
	frameStartTime = 0;
	frameEndTime = 0;
	CameraY = 0.0f;
	CameraX = 0.0f;
	CameraZ = 7.5f;
	CameraLookX = 0.0f;
	CameraLookY = 0.0f;
	CameraLookZ = 0.0f;

	musicplaying = true;
	sound = true;

	gamestate = TITLE;
}


gameInfo::~gameInfo()
{

}
