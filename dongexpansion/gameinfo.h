#pragma once

// Include headers accessible 
#include "Globals.h"

// this class holds all of the information about a game state
class gameInfo
{
	public:
		gameInfo();			// the constructor
		~gameInfo();		// the destructor

		
		// variables required for the frame delta (DWORD is basically a long integer)
		DWORD frameStartTime;	// what the system time (in millisecs) was at the start of the last frame
		DWORD frameEndTime;		// what the system time (in millisecs) was at the end of the last frame 
		DWORD frameDelta;		// difference between the two (in millisecs), i.e. how long the last frame took

		float deltaTime;

		float CameraX;
		float CameraY;
		float CameraZ;
		float CameraLookX;
		float CameraLookY;
		float CameraLookZ;

		bool pause;
		bool sound;
		bool musicplaying;

		int gamestate;
};