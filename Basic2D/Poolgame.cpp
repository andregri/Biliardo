#include "Poolgame.h"
#include <stdlib.h>
#include <time.h>


Poolgame::Poolgame()
{
	NewGame();
}


Poolgame::~Poolgame()
{
}

void Poolgame::InitPositions()
{
	int n[14] = { 1, 2, 3, 4, 5, 6, 7, 9, 10, 11, 12, 13, 14, 15 };
	srand(time(NULL));
	// initialize randomly starting position of balls
	
	for (int i = 0; i < 14; i++) {
		int random = rand() % 14;	// generate random index of array n

		// swap elements in n
		int n1 = n[i];
		n[i] = n[random];
		n[random] = n1;
	}

	float x_shift = 0.0f;
	// 1st row
	balls[n[0]].SetPosition(glm::vec3(x_shift + 0.0f, 0.0f, Ball::r));

	// 2nd row
	balls[n[1]].SetPosition(glm::vec3(x_shift + 2 * Ball::r, Ball::r, Ball::r));
	balls[n[2]].SetPosition(glm::vec3(x_shift + 2 * Ball::r, -Ball::r, Ball::r));

	// 3rd row
	balls[n[3]].SetPosition(glm::vec3(x_shift + 4 * Ball::r, 2 * Ball::r, Ball::r));
	balls[8].SetPosition(glm::vec3(x_shift + 4 * Ball::r, 0.0f, Ball::r));
	balls[n[4]].SetPosition(glm::vec3(x_shift + 4 * Ball::r, -2 * Ball::r, Ball::r));

	// 4th row
	balls[n[5]].SetPosition(glm::vec3(x_shift + 6 * Ball::r, 3 * Ball::r, Ball::r));
	balls[n[6]].SetPosition(glm::vec3(x_shift + 6 * Ball::r, Ball::r, Ball::r));
	balls[n[7]].SetPosition(glm::vec3(x_shift + 6 * Ball::r, -Ball::r, Ball::r));
	balls[n[8]].SetPosition(glm::vec3(x_shift + 6 * Ball::r, -3 * Ball::r, Ball::r));

	// 5th row
	balls[n[9]].SetPosition(glm::vec3(x_shift + 8 * Ball::r, 4 * Ball::r, Ball::r));
	balls[n[10]].SetPosition(glm::vec3(x_shift + 8 * Ball::r, 2 * Ball::r, Ball::r));
	balls[n[11]].SetPosition(glm::vec3(x_shift + 8 * Ball::r, 0.0f, Ball::r));
	balls[n[12]].SetPosition(glm::vec3(x_shift + 8 * Ball::r, -2 * Ball::r, Ball::r));
	balls[n[13]].SetPosition(glm::vec3(x_shift + 8 * Ball::r, -4 * Ball::r, Ball::r));


	// white ball position
	balls[0].SetPosition(glm::vec3(-0.7f, 0.0f, Ball::r));
}

void Poolgame::NewGame()
{
	turnStarted = false;
	turnEnded = false;
	isPlayer1Turn = true;
	isFault = false;
	ballInPocket = false;
	ballsHitNumber = 0;
	player1BallType = Ball::BallType::Undefined;
	player2BallType = Ball::BallType::Undefined;
	InitPositions();
}

void Poolgame::NextTurn(bool keepTurn)
{
	
	// If it is fault, turn pass to the other player
	if (!keepTurn) {
		isPlayer1Turn = !isPlayer1Turn;
	}
}

void Poolgame::Fault()
{
	isPlayer1Turn = !isPlayer1Turn;
}

bool Poolgame::IsGameOver()
{
	if (isPlayer1Turn) {
		switch (player1BallType)
		{
		case Ball::BallType::Solid:
			if (solidInPocket.size() < 8) {
				return true;
			}
		case Ball::BallType::Striped:
			if (solidInPocket.size() < 8) {
				return true;
			}
		default:
			// No balls in pocket yet; Ball 8 is the 1st one -> Game Over 
			return true;
		}
	}
	else {
		switch (player2BallType)
		{
		case Ball::BallType::Solid:
			if (solidInPocket.size() < 8) {
				return true;
			}
		case Ball::BallType::Striped:
			if (solidInPocket.size() < 8) {
				return true;
			}
		default:
			// No balls in pocket yet; Ball 8 is the 1st one -> Game Over 
			return true;
		}
	}
	return false;
}

void Poolgame::BallFallsInPocket(int ballNumber)
{
	balls[ballNumber].isInPocket = true;
	balls[ballNumber].isMoving = false;

	glm::vec3 pos = balls[ballNumber].GetPosition();
	balls[ballNumber].SetPosition(glm::vec3(pos.x, pos.y, -Ball::r));

	if (ballNumber == 0) {
		// White ball in pocket -> Fault
		isFault = true;
	}
	else {
		ballsInPocket.push_back(ballNumber);
		if (balls[ballNumber].color == Ball::BallType::Striped) {
			stripedInPocket.push_back(ballNumber);
		}
		else if (balls[ballNumber].color == Ball::BallType::Solid) {
			solidInPocket.push_back(ballNumber);
		}

		if (ballNumber == 8) {
			if (IsGameOver()) {
				NewGame();
			}
			else {
				// player won the game
			}
			
		}

		if (ballsInPocket.size() == 1) {	// First ball has fallen
			// Set which type of ball players have to put in pockets
			SetPlayerBallType(balls[ballNumber].color, isPlayer1Turn);
		}
		else {			// Ball different from first one has fallen
			if (isPlayer1Turn) {
				if (balls[ballNumber].color != player1BallType) {
					 // The player puts in pocket a wrong ball -> Fault
					isFault = true;
					balls[0].isInPocket = true;
				}
			}
			else { // Player 2 turn
				if (balls[ballNumber].color != player2BallType) {
					// The player puts in pocket a wrong ball -> Fault
					isFault = true;
					balls[0].isInPocket = true;
				}
			}
		}
	}
}

void Poolgame::PlaceWhiteBall(float cursorY)
{
	// If you try to place the white ball outside the table
	bool isOutside = true;
	float ht = table.h / 2;
	if (cursorY > -ht + Ball::r &&
		cursorY < ht - Ball::r) {
		isOutside = false;
	}

	// If there is another ball where you want to place white ball
	bool isPositionEmpty = true;
	for (int i = 1; i < 16; i++) {
		// Check if you want to place the white ball over another ball
		float ballX = balls[i].GetPosition().x;
		if (!balls[i].isInPocket &&
			ballX < -0.7f + 2 * Ball::r &&
			ballX > -0.7f - 2 * Ball::r) {

			float ballY = balls[i].GetPosition().y;

			if (cursorY > ballY - 2 * Ball::r &&
				cursorY < ballY + 2 * Ball::r) {
				isPositionEmpty = false;
			}
		}
	}

	if (!isOutside && isPositionEmpty) {	// Place the ball
		glm::vec3 whiteNewPos = glm::vec3(-0.7f, cursorY, Ball::r);
		balls[0].SetPosition(whiteNewPos);
		balls[0].SetSlidingVelocity(glm::vec3(0.0f));
		balls[0].isInPocket = false;
	}
}

void Poolgame::CollisionWall(Ball &ball)
{
	glm::vec3 v = ball.GetSlidingVelocity();
	glm::vec3 w = ball.GetAngularVelocity();
	glm::vec3 x = ball.GetPosition();

	if (x[0] >= table.w / 2 - Ball::r) {		// right side
		// Prevent balls from going out the table
		ball.SetPosition(glm::vec3(table.w / 2 - Ball::r, x[1], x[2]));
		ball.SetSlidingVelocity(glm::vec3(-v.x, v.y, v.z));
		ball.SetAngularVelocity(glm::vec3(w.x, -w.y, w.z));
	}
	else if (x[0] <= -table.w / 2 + Ball::r) {	// left side
		ball.SetPosition(glm::vec3(-table.w / 2 + Ball::r, x[1], x[2]));
		ball.SetSlidingVelocity(glm::vec3(-v.x, v.y, v.z));
		ball.SetAngularVelocity(glm::vec3(w.x, -w.y, w.z));
	}
	else if (x[1] >= table.h / 2 - Ball::r) {	// top side
		ball.SetPosition(glm::vec3(x[0], table.h / 2 - Ball::r, x[2]));
		ball.SetSlidingVelocity(glm::vec3(v.x, -v.y, v.z));
		ball.SetAngularVelocity(glm::vec3(-w.x, w.y, w.z));
	}
	else if (x[1] <= -table.h / 2 + Ball::r) {	// bottom side
		ball.SetPosition(glm::vec3(x[0], -table.h / 2 + Ball::r, x[2]));
		ball.SetSlidingVelocity(glm::vec3(v.x, -v.y, v.z));
		ball.SetAngularVelocity(glm::vec3(-w.x, w.y, w.z));
	}
}

void Poolgame::CollisionBalls(Ball & ball1, Ball & ball2)
{
	glm::vec3 x1 = ball1.GetPosition();
	glm::vec3 x2 = ball2.GetPosition();
	glm::vec3 v1 = ball1.GetSlidingVelocity();
	glm::vec3 v2 = ball2.GetSlidingVelocity();

	// detect if balls are close (distance < 2R), but they are not colliding
	float distance = glm::distance(x1, x2);						// actual distance between balls
	float dt = 0.01;
	float nextDistance = glm::distance(x1 + v1 * dt, x2 + v2 * dt);		// future distance between balls after a time 'dt'
	float ds = nextDistance - distance;
	if (ds > 0) {	// balls are going away, they are not colliding 
		return;
	}

	glm::vec3 n = glm::vec3(x1.x - x2.x, x1.y - x2.y, x1.z - x2.z);
	n = glm::normalize(n);	// normal vector of the collision plane

	// Perpendicular and tangent components of velocity of ball1
	// with respect to the collision plane
	glm::vec3 vn1 = glm::dot(v1, -n) * -n;
	glm::vec3 vt1 = v1 - vn1;

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

	//numberOfBallsHit++;
}

void Poolgame::SetPlayerBallType(Ball::BallType color, bool player1Turn)
{
	switch (color) {
	case Ball::BallType::Striped:
		if (player1Turn) {
			player1BallType = Ball::BallType::Striped;
			player2BallType = Ball::BallType::Solid;
		}
		else {
			player1BallType = Ball::BallType::Solid;
			player2BallType = Ball::BallType::Striped;
		}
		break;
	case Ball::BallType::Solid:
		if (player1Turn) {
			player1BallType = Ball::BallType::Solid;
			player2BallType = Ball::BallType::Striped;
		}
		else {
			player1BallType = Ball::BallType::Striped;
			player2BallType = Ball::BallType::Solid;
		}
		break;
	}
}
