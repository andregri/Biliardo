#include "Poolgame.h"



Poolgame::Poolgame()
	: ball15(15)
{
}


Poolgame::~Poolgame()
{
}

void Poolgame::CollisionWall(Ball &ball)
{
	glm::vec3 v = ball.GetSlidingVelocity();
	glm::vec3 w = ball.GetAngularVelocity();
	glm::vec3 x = ball.GetPosition();

	if (x[0] >= table.width / 2 - 0.01f) {
		ball.SetSlidingVelocity(glm::vec3(-v.x, v.y, v.z));	// right side
		ball.SetAngularVelocity(glm::vec3(w.x, -w.y, w.z));
	}
	else if (x[0] <= -table.width / 2 + 0.01f) {			// left side
		ball.SetSlidingVelocity(glm::vec3(-v.x, v.y, v.z));
		ball.SetAngularVelocity(glm::vec3(w.x, -w.y, w.z));
	}
	else if (x[1] >= table.height / 2 - 0.01f) {			// top side
		ball.SetSlidingVelocity(glm::vec3(v.x, -v.y, v.z));
		ball.SetAngularVelocity(glm::vec3(-w.x, w.y, w.z));
	}
	else if (x[1] <= -table.height / 2 + 0.01f) {			// bottom side
		ball.SetSlidingVelocity(glm::vec3(v.x, -v.y, v.z));
		ball.SetAngularVelocity(glm::vec3(-w.x, w.y, w.z));
	}
}
