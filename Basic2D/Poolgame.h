#pragma once

#include "Table.h"
#include "Ball.h"
#include <vector>

class Poolgame
{
public:

	Table table;
	Ball balls[16] = {	Ball(0), Ball(1), Ball(2), Ball(3),
						Ball(4), Ball(5), Ball(6), Ball(7),
						Ball(8), Ball(9), Ball(10), Ball(11),
						Ball(12), Ball(13), Ball(14), Ball(15)};
	bool isPlayer1Turn;					// Determine turn of the player
	Ball::BallType player1BallType;		// Which kind of balls player 1 has to shot
	Ball::BallType player2BallType;
	std::vector<int> ballsInPocket;
	std::vector<int> stripedInPocket;
	std::vector<int> solidInPocket;
	bool isFault;
	bool turnStarted;
	bool turnEnded;
	bool ballInPocket;
	int ballsHitNumber;
	bool ballSound;

	enum GameState {PLAY, HOME, RULES, PLY1_WIN, PLY2_WIN};
	enum View {TOP_VIEW, REAL_VIEW};
	GameState gameState;
	View view;

	Poolgame();
	~Poolgame();

	void InitPositions();
	void NewGame();
	void NextTurn(bool keepTurn);
	void Fault();
	bool IsGameOver();
	void BallFallsInPocket(int ballNumber);
	void PlaceWhiteBall(float cursorY);
	void CollisionWall(Ball &ball);
	void CollisionBalls(Ball &ball1, Ball &ball2);

private:
	void SetPlayerBallType(Ball::BallType color, bool player1Turn);
};

