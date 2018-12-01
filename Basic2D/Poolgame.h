#pragma once

#include "Table.h"
#include "Ball.h"

class Poolgame
{
public:

	Table table;
	Ball* balls[16];
	Ball ball1;
	Ball ball15;

	Poolgame();
	~Poolgame();

	void CollisionWall(Ball &ball);
	void CollisionBalls(Ball &ball1, Ball &ball2);
};

