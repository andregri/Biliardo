#pragma once

#include "Table.h"
#include "Ball.h"

class Poolgame
{
public:

	Table table;
	Ball balls[16] = {	Ball(0), Ball(1), Ball(2), Ball(3),
						Ball(4), Ball(5), Ball(6), Ball(7),
						Ball(8), Ball(9), Ball(10), Ball(11),
						Ball(12), Ball(13), Ball(14), Ball(15),};

	Poolgame();
	~Poolgame();

	void InitPositions();
	void CollisionWall(Ball &ball);
	void CollisionBalls(Ball &ball1, Ball &ball2);
};

