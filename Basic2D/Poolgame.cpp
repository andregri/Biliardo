#include "Poolgame.h"



Poolgame::Poolgame()
	: ball1(1), ball15(15)
{
	for (int i = 0; i < 16; i++) {
		balls[i] = 0;
	}

	balls[1] = &ball1;
	balls[15] = &ball15;
	ball1.SetPosition(glm::vec3(0.5f, 0.0f, Ball::r));
}


Poolgame::~Poolgame()
{
}

void Poolgame::CollisionWall(Ball &ball)
{
	glm::vec3 v = ball.GetSlidingVelocity();
	glm::vec3 w = ball.GetAngularVelocity();
	glm::vec3 x = ball.GetPosition();

	if (x[0] >= table.width / 2) {			// right side
		ball.SetSlidingVelocity(glm::vec3(-v.x, v.y, v.z));	
		ball.SetAngularVelocity(glm::vec3(w.x, -w.y, w.z));
	}
	else if (x[0] <= -table.width / 2) {	// left side
		ball.SetSlidingVelocity(glm::vec3(-v.x, v.y, v.z));
		ball.SetAngularVelocity(glm::vec3(w.x, -w.y, w.z));
	}
	else if (x[1] >= table.height / 2) {	// top side
		ball.SetSlidingVelocity(glm::vec3(v.x, -v.y, v.z));
		ball.SetAngularVelocity(glm::vec3(-w.x, w.y, w.z));
	}
	else if (x[1] <= -table.height / 2) {	// bottom side
		ball.SetSlidingVelocity(glm::vec3(v.x, -v.y, v.z));
		ball.SetAngularVelocity(glm::vec3(-w.x, w.y, w.z));
	}
}

void Poolgame::CollisionBalls(Ball & ball1, Ball & ball2)
{
	glm::vec3 x1 = ball1.GetPosition();
	glm::vec3 x2 = ball2.GetPosition();
	//glm::vec3 collisionPoint = glm::vec3((x1.x + x2.x)/2, (x1.y + x2.y) / 2, ball1.r);

	glm::vec3 n = glm::vec3(x1.x - x2.x, x1.y - x2.y, x1.z - x2.z);
	n = glm::normalize(n);	// normal vector of the collision plane

	glm::vec3 v1 = ball1.GetSlidingVelocity();
	// Perpendicular and tangent components of velocity of ball1
	// with respect to the collision plane
	glm::vec3 vn1 = glm::dot(v1, -n) * -n;
	glm::vec3 vt1 = v1 - vn1;

	glm::vec3 v2 = ball2.GetSlidingVelocity();
	// Perpendicular and tangent components of velociy of ball2
	glm::vec3 vn2 = glm::dot(v2, n) * n;
	glm::vec3 vt2 = v2 - vn2;

	// Update velocities after the collision
	glm::vec3 v1f = vn2 + vt1;
	ball1.SetSlidingVelocity(v1f);
	glm::vec3 w1 = glm::cross(glm::vec3(0.0f, 0.0f, Ball::r), v1f) * float(2.5 / (Ball::r * Ball::r));
	ball1.SetAngularVelocity(w1);

	glm::vec3 v2f = vn1 + vt2;
	ball2.SetSlidingVelocity(v2f);
	glm::vec3 w2 = glm::cross(glm::vec3(0.0f, 0.0f, Ball::r), v2f) * float(2.5 / (Ball::r * Ball::r));
	ball2.SetAngularVelocity(w2);
}	