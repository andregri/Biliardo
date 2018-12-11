#include "Ball.h"

#include <windows.h>		// Header File For Windows
#include <math.h>
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include "Sfera.h"

const float Ball::r = 0.03f;

Ball::Ball(const int number)
	: number(number), m(0.2),  mu_s(0.01)
{
	Init();
}

Ball::~Ball()
{
}

void Ball::SetShotVelocity(glm::vec3 shot)
{
	v = shot;
	w = glm::cross(glm::vec3(0.0f, 0.0f, r), v) * float(2.5 / (r*r));
}

// delta_t: time interval
void Ball::Move(float dt)
{
	if (glm::length(v) < -0.001 || glm::length(v) > 0.001) {
		glm::vec3 a = glm::vec3(0.0, 0.0, -r);

		// velocity variation
		glm::vec3 dv = glm::normalize(v) * float(-dt * mu_s * 9.81);
		glm::vec3 dw = -float(2.5/(r*r)) * glm::cross(a, dv);

		// update velocity
		v += dv;
		w += dw;
		rot = w;
	
		// update position
		x += (v * float(dt));
		p += (glm::length(w) * dt) * 180.0f / 3.14f;

		isMoving = true;
	}
	else {
		v = glm::vec3(0.0f);
		//w = glm::vec3(0.0f);
		isMoving = false;
	}

}

glm::vec3 Ball::GetPosition()
{
	return glm::vec3(x);
}


glm::vec3 Ball::GetSlidingVelocity()
{
	return glm::vec3(v);
}

glm::vec3 Ball::GetAngularVelocity()
{
	return glm::vec3(w);
}

float Ball::GetPhase()
{
	return p;
}

void Ball::Init()
{
	x = glm::vec3(0.0f, 0.0f, r);
	v = glm::vec3(0.0f);
	w = glm::vec3(0.0f, 0.0f, 0.0f);

	rot = glm::vec3(0.0f, 1.0f, 0.0f);
	p = 90.0f;

	isInPocket = false;
	isMoving = false;
}

void Ball::SetPosition(glm::vec3 x)
{
	this->x = x;
}

void Ball::SetSlidingVelocity(glm::vec3 v)
{
	this->v = v;
}

void Ball::SetAngularVelocity(glm::vec3 w)
{
	this->w = w;
}

void Ball::Draw()
{
	glTranslatef(x[0], x[1], r);
	glRotatef(p, rot[0], rot[1], rot[2]);
	//glRotatef(90.0f, 0.0f, 1.0f, 0.0f);

#if !defined(DEBUG)
	DrawSphere(r);
#endif // DEBUG

#ifdef DEBUG
	GLUquadric *quad;
	quad = gluNewQuadric();
	gluSphere(quad, r, 50, 50);
#endif // DEBUG
}