#include "stdafx.h"
#include "Globals.h"
// Include headers accessible 
#include "MainObject.h"
#include "paddle.h"

Paddle::Paddle(int ID)
{
	MainObject::Init(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.125f, 0.25f);

	MainObject::SetScaleX(0.125f);
	MainObject::SetScaleY(0.25f);
	MainObject::SetScaleZ(0.125f);
	MainObject::SetID(ID);

	onplatform = false;
	dead = false;
	extrajump = 0.0f;
	extraj = 0;

	MainObject::SetX(4.0f);
	MainObject::SetY(1.5f);
	MainObject::SetTexNum(2);
	
	SetShow(false);
}

void Paddle::MoveRight(float speed, float deltaTime)
{
	Paddle::x -= (PADDLESPEED + speed) * deltaTime;
	y_dir = 1;
}
void Paddle::MoveLeft(float speed, float deltaTime)
{
	Paddle::x += ((PADDLESPEED * 1.5f) + speed) * deltaTime;
	y_dir = 2;
}



void Paddle::Update(float deltaTime)
{
	if(x > 4.5)
		x = 4.5;
	else if(x < -4.5)
		x = -4.5;
		ry = 0;

	if(!onplatform) {
	y -= (GRAVITY * deltaTime);
	}

	onplatform = false;

	//if(!extraj)
	//	extrajump = 0.0f;
}

void Paddle::Reset()
{
	MainObject::SetX(4.0f);
	MainObject::SetY(-0.5f);
	
	ResetScore();
	ResetDir();
	SetShow(false);
	dead = false;
	onplatform = false;
	extraj = false;
	extrajump = 0.0f;
	red = 0.2f;
}

void Paddle::Collided(MainObject *otherObject, float deltaTime)
{
	if(otherObject->GetID() == BORDER) 
	{
	onplatform = true;
	}
}

void Paddle::Jump(float deltaTime)
{
	if(onplatform)
	{
		//for(float jump = 0.0f; jump > (2.0f + extrajump); jump += 0.1f) 
		//{
		if(!extraj)
			y += (4.0f * deltaTime);
		if(extraj)
			y += (8.0f * deltaTime);
		//y += ((4.0f + extrajump) * deltaTime);
		//}
		extrajump = 0.0f;
		red = 0.2f;
		extraj = false;
		//FILL THIS LATER WITH MATH SHIT
	}
}

void Paddle::Jumpincrease(float deltaTime)
{
	extraj = true;
	if(onplatform) {
	if(extrajump < 4.0f)
	extrajump += (1.0f * deltaTime);
	if(red < 4.0f)
	red += (0.1f * deltaTime);
	}
}