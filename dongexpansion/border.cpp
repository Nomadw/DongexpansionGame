#include "stdafx.h"
#include "Globals.h"
// Include headers accessible 
#include "MainObject.h"
#include "border.h"

Border::Border(float x, float y)
{
	MainObject::Init(x, y, 0.0f, 0.0f, 0.0f, 10.0f, 0.125f);

	MainObject::SetScaleX(10.0f);
	MainObject::SetScaleY(0.125f);
	MainObject::SetScaleZ(0.125f);
	MainObject::SetTexNum(7);
	MainObject::SetID(BORDER);
	SetShow(true);
	SetCollidable(true);
}

Border::Border(float x, float y, float scale)
{
	MainObject::SetX(x);
	MainObject::SetY(y);

	MainObject::SetXBound(scale);

	MainObject::SetScaleX(scale);
	MainObject::SetScaleY(0.125f);
	MainObject::SetScaleZ(0.125f); 
	MainObject::SetTexNum(7);
	MainObject::SetID(BORDER);
	SetShow(true);
	SetCollidable(true);
}

Border::Border()
{
	int scale;
	scale = 1 + rand() % 2;

	MainObject::SetX(-10.0f);
	MainObject::SetY(-3 + rand() % 5);

	MainObject::SetXBound(scale);

	MainObject::SetScaleX(scale);
	MainObject::SetScaleY(0.125f);
	MainObject::SetScaleZ(0.125f);
	MainObject::SetTexNum(7);
	MainObject::SetID(BORDER);
	SetShow(true);
	SetCollidable(true);
}

void Border::Update(float speed, float deltaTime)
{
	x += ((PLATFORMSPEED + speed) * deltaTime);
	green += (0.01 * deltaTime);
}

void Border::Reset()
{
	SetShow(false);
}

void Border::Collided(MainObject *otherObject, float deltaTime)
{
	if(otherObject->GetID() == PLAYER)
	{
		y -= ((GRAVITY / 6) * deltaTime);
	}
	if(otherObject->GetID() == BORDER)
	{
		SetShow(false);
		SetCollidable(false);
	}
}