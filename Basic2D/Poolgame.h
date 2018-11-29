#pragma once

#include "Table.h"
#include "Ball.h"

class Poolgame
{
public:
	Table table;
	Ball ball15;

	Poolgame();
	~Poolgame();

	void CollisionWall(Ball &ball);
};

