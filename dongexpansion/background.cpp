#include "stdafx.h"
#include "Globals.h"
// Include headers accessible 
#include "MainObject.h"
#include "background.h"

Background::Background()
{
	MainObject::Init(0.0f, 0.0f, -5.0f, 0.0f, 0.0f, 0.0f, 0.0f);

	MainObject::SetScaleX(30.0f);
	MainObject::SetScaleY(12.0f);
	MainObject::SetScaleZ(0.001f);
	MainObject::SetTexNum(6);

	angle = 0.0f;
	originX = -7.5f;
	SetShow(false);
	SetCollidable(false);
}

void Background::Update(float deltaTime)
{
	float originY = 0;

	float Size = 5.5f;

	x = originX + (sin(angle)*Size);
	y = originY + (cos(angle)*Size);
	angle += (BACKGROUNDSPEED * deltaTime);
	
	originX += (BACKGROUNDSPEED * deltaTime);
	if(originX > 7.5f)
		originX = -7.5f;
}

void Background::Reset()
{
	SetX(0.0f);
	SetY(0.0f);
	SetShow(false);
	angle = 0.0f;
	originX = -7.5f;
}