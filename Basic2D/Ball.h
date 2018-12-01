#if !defined(BALL_H)
#define BALL_H

//#define DEBUG

#include "glm/glm.hpp"

class Ball
{
private:
	glm::vec3 x;	// cordinates of the center of the ball
	glm::vec3 v;	// velocity

	float p;		// phase
	glm::vec3 w;	// angular velocity

public:
	const int number;		// number of the ball (0: white, 1-15)
	const float m;			// weight = 0.2 Kg
	static const float r;	// radius = (61.5 mm) 0.03
	const float mu_s;		// friction coefficient: mu_s=0.01

	Ball(int number);
	~Ball();

	void SetShotVelocity(glm::vec3 shot);
	void Move(float delta_t);

	glm::vec3 GetPosition();
	glm::vec3 GetSlidingVelocity();
	glm::vec3 GetAngularVelocity();
	float GetPhase();

	void Init();
	void SetPosition(glm::vec3 x);
	void SetSlidingVelocity(glm::vec3 v);
	void SetAngularVelocity(glm::vec3 w);
	
	void Draw();
};

#endif    // BALL_H