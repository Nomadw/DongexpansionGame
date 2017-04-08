#include "stdafx.h"
#include "Globals.h"
// Include headers accessible 
#include "MainObject.h"
#include "ball.h"

Ball::Ball(int ID)
{
	MainObject::Init(0.0f, 0.0f, 0.2f, 0.0f, 0.0f, 0.125f, 0.125f);

	Object_ID = ID;

	MainObject::SetX(-6.0f);
	originX = -6.0f;
	MainObject::SetY(-3 + rand() % 5);
	originY = y;

	MainObject::SetScaleX(0.125f);
	MainObject::SetScaleY(0.125f);
	MainObject::SetScaleZ(0.125f);

	if(ID == BALLBAD)
	MainObject::SetTexNum(1);
	if(ID == BALLGOOD)
		MainObject::SetTexNum(8);

	y_dir = rand() % 3 - 2;
	
	SetShow(true);
}

void Ball::Update(float speed, float deltaTime)
{
	if(Object_ID == BALLBAD)
	{
	ry += BALLSPINSPEED;
	rx += BALLSPINSPEED;
	rz += BALLSPINSPEED;
	if(ry >= D3DXToRadian(360))
		ry = 0;
	if(rx >= D3DXToRadian(360))
		rx = 0;
	if(rz >= D3DXToRadian(360))
		rz = 0;
	x += ((BALLSPEED + (speed / 2)) * deltaTime);
	}
	if(Object_ID == BALLGOOD)
	{

	float Size = 1.0f;

	x = originX + (sin(angle)*Size);
	y = originY + (cos(angle)*Size);
	angle -= BACKGROUNDSPEED * deltaTime;
	originX += (((BALLSPEED / 2 + (speed / 2))) * deltaTime);
	}

	if(y_dir == 1)
		y += (BALLSPEED * deltaTime);
	if(y_dir == 2)
		y -= (BALLSPEED * deltaTime);

}

void Ball::Collided(MainObject *otherObject, float deltaTime) 
{
	SetShow(false);
	SetCollidable(false);
}

void Ball::Reset()
{
	SetShow(true);
}