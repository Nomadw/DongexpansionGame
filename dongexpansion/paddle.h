#pragma once

// Include headers accessible 
#include "Globals.h"

class Paddle : public MainObject
{
public:

	float score;

	bool onplatform;

	bool dead;

	void Jump(float deltaTime);
	float extrajump;
	bool extraj;
	void Jumpincrease(float deltaTime);

	void Update(float deltaTime);
	void Reset();
	Paddle(int ID);
	void MoveLeft(float speed, float deltaTime);
	void MoveRight(float speed, float deltaTime);
	void Score() {score++;}
	float GetScore() {return score;}
	void Die() {score--; y = 5;}
	void ResetScore() {score = 0;}
	void ResetDir() {y_dir = 0;}
	void Collided(MainObject *otherObject, float deltaTime);
};