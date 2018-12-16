///////////////////////////////////////////////////////////////////
//  A basic skeleton for 2D like game developpers.
//
//  model data
///////////////////////////////////////////////////////////////////

#include <windows.h>		// Header File For Windows
#include <stdio.h>			// Header File For Standard Input/Output
#include <string.h>
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library

#include "Sfera.h"
#include "Model.h"
#include "SOIL.h"
#include "Table.h"

#include "Ball.h"

// All Setup For OpenGL Goes Here
bool MyModel::InitGL(void)
{
	if (!this->LoadGLTextures())						// Jump To Texture Loading Routine
  {	return false; }										// If Texture Didn't Load Return FALSE


	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping ( NEW )
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	this->BuildFont();

	// eye    (0,0,0)
	// center (0,0,-1)
	// up     (0,1,0)
	//gluLookAt(0.0,0.0,0.0, 0.0,0.0,-1.0, 0.0,1.0,0.0);

	return true;	// Initialization Went OK
}

GLvoid MyModel::SetProjection(GLsizei iWidth, GLsizei iHeight)
{
	if (iHeight == 0) iHeight = 1;
	glViewport(0, 0, iWidth, iHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	if (Pool.view == Poolgame::View::TOP_VIEW) {
		glOrtho(-this->plx, this->plx, -this->ply, this->ply, -2.0f, 2.0f);
	}
	if (Pool.view == Poolgame::View::REAL_VIEW) {
		gluPerspective(45.0, (GLfloat)iWidth / (GLfloat)iHeight, 0.1, 100.0);
		glm::vec3 pos = Data.Pool.balls[0].GetPosition();
		gluLookAt(1.8 * sin(view_angle) + pos.x, 1.8 * cos(view_angle) + pos.y, 0.9,	// eye
			pos.x, pos.y, 0.0,	// center
			0.0, 0.0, 1.0);		// up vector
	}
	//	Try to change the front clipping plane, for example:
	//gluPerspective(CS.fovy,(GLfloat)iWidth/(GLfloat)iHeight,4,200.0);


	glMatrixMode(GL_MODELVIEW);					// Select The Modelview Matrix
	glLoadIdentity();							// Reset The Modelview Matrix
}



void MyModel::ReSizeGLScene(int width, int height)
{
	if (height==0) height=1;					// Prevent A Divide By Zero By
	if (width==0) width=1;						// Prevent A Divide By Zero By

	glViewport(0,0,width,height);				// Reset The Current Viewport

	// saving the window width and height in pixels
	this->Wheight = height;
	this->Wwidth = width;

	if (width >= height) {
		this->plx = 1.1f;
		this->ply = (double)height / (double)width;
	}
	else {
		this->plx = (double)width / (double)height;
		this->ply = 1.1f;
	}

	Background.clear();
	Background.push_back(Vertex(-this->plx, -this->ply, -1, 0, 0));
	Background.push_back(Vertex(this->plx, -this->ply, -1, 1, 0));
	Background.push_back(Vertex(this->plx, this->ply, -1, 1, 1));
	Background.push_back(Vertex(-this->plx, this->ply, -1, 0, 1));

	glMatrixMode(GL_MODELVIEW);					// Select The Modelview Matrix
	glLoadIdentity();							// Reset The Modelview Matrix
}



// Load Bitmaps And Convert To Textures
bool MyModel::LoadGLTextures(void)
{
	/* load an image file directly as a new OpenGL texture */
	for (int i = 1; i < 16; i++) {
		char path[20];
		sprintf(path, "%s%d%s", "../Data/Ball", i, ".jpg");
		//strcat(path,ballNumber);
		//strcat(path, ".jpg");
		texture[i] = SOIL_load_OGL_texture
		(path,
			SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);

		if (texture[i] == 0) {
			return false;
		}
	}

	texture[0] = SOIL_load_OGL_texture
	("../Data/BallCue.jpg",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (texture[0] == 0) {
		return false;
	}

	// Load Rules image
	rules = SOIL_load_OGL_texture
	("../Data/Rules.jpg",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (rules == 0) {
		return false;
	}

	// Load Rules image
	start = SOIL_load_OGL_texture
	("../Data/Schermata_iniziale.jpg",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (rules == 0) {
		return false;
	}

	// Load win images
	for (int i = 0; i < 2; i++) {
		char path[20];
		sprintf(path, "%s%d%s", "../Data/player", i+1, "_vinto.jpg");
		win[i] = SOIL_load_OGL_texture
		(path,
			SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
		if (win[i] == 0) {
			return false;
		}
	}

	return true;								// Return Success
}



bool MyModel::DrawGLScene(void)
{
	this->SetProjection(Wwidth, Wheight);

	//  TIMING ---------------------------------------- start
	clock_t t = clock();
	// elapsed time in seconds from the last draw
	double elapsed = double(t - Tstamp) / (double)CLOCKS_PER_SEC;
	if (elapsed < 0.008) return true;  // to limit fps!!!!
	// elapsed time in milliseconds from the last draw
	int ms_elapsed = (int)(t - Tstamp);
	// elapsed time in seconds from the beginning of the program
	this->Full_elapsed = double(t - Tstart) / (double)CLOCKS_PER_SEC;
	this->frameTime += double(t - Tstamp) / (double)CLOCKS_PER_SEC;

	this->Tstamp = t;
	//  TIMING ---------------------------------------- end

	//  PLAY ---------------------------------------- start
	if (Pool.gameState == Poolgame::GameState::PLAY) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
		glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
		glLoadIdentity();									// Reset The View

		// Draw a table
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		Pool.table.Draw();

		// Draw cursor line
		if (Pool.view == Poolgame::View::TOP_VIEW) {
			if (captured) {
				glBegin(GL_LINES);
				if (Pool.isPlayer1Turn) {
					glColor3f(1.0f, 0.0f, 0.0f);
				}
				else {
					glColor3f(0.0f, 0.0f, 1.0f);
				}
				glVertex3f(ClientX2World(cx), ClientY2World(cy), Ball::r);
				glm::vec3 x_ = Pool.balls[0].GetPosition();
				glVertex3f(x_[0], x_[1], Ball::r);
				glEnd();
			}
		}
		if (Pool.view == Poolgame::View::REAL_VIEW) {
			glBegin(GL_LINES);
			if (Pool.isPlayer1Turn) {
				glColor3f(1.0f, 0.0f, 0.0f);
			}
			else {
				glColor3f(0.0f, 0.0f, 1.0f);
			}
			glm::vec3 x_ = Pool.balls[0].GetPosition();
			glVertex3f(	-force * sin(view_angle) + x_.x,
						-force * cos(view_angle) + x_.y, Ball::r);
			glVertex3f(x_[0], x_[1], Ball::r);
			glEnd();
		}

		// set Textures to all balls
#if !defined(DEBUG)
		for (int i = 0; i < 16; i++) {
			// Always draw white ball, but not balls in pockets
			/*if (!Pool.balls[i].isInPocket || i == 0) {*/
				glEnable(GL_TEXTURE_2D);
				glMatrixMode(GL_MODELVIEW);				// Select The Modelview Matrix
				glLoadIdentity();
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
				glBindTexture(GL_TEXTURE_2D, texture[i]);
				Pool.balls[i].Draw();
			/*}*/
		}
#endif // DEBUG

#ifdef DEBUG
		glEnable(GL_TEXTURE_2D);
		glMatrixMode(GL_MODELVIEW);				// Select The Modelview Matrix
		glLoadIdentity();
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		Pool.balls[0].Draw();
		for (int i = 1; i < 9; i++) {
			glEnable(GL_TEXTURE_2D);
			glMatrixMode(GL_MODELVIEW);				// Select The Modelview Matrix
			glLoadIdentity();
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
			glBindTexture(GL_TEXTURE_2D, texture[8]);
			Pool.balls[i].Draw();
		}
		for (int i = 9; i < 16; i++) {
			glEnable(GL_TEXTURE_2D);
			glMatrixMode(GL_MODELVIEW);				// Select The Modelview Matrix
			glLoadIdentity();
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
			glBindTexture(GL_TEXTURE_2D, texture[1]);
			Pool.balls[i].Draw();
		}
#endif // DEBUG

		int nBalls = 16;

#if !defined(DEBUG)
		for (int i = 0; i < nBalls; i++) {
			if (!Pool.balls[i].isInPocket) {
				Pool.balls[i].Move(elapsed);
			}
		}
#endif // DEBUG

#ifdef DEBUG
		for (int i = 0; i < 16; i++) {
			if (!Pool.balls[i].isInPocket) {
				Pool.balls[i].Move(0.01);
			}
		}
#endif // DEBUG


		float ht = Pool.table.h / 2;
		float wt = Pool.table.w / 2;
		float pr = Pool.table.pocket_r;

		for (int i = 0; i < nBalls; i++) {
			if (!Pool.balls[i].isInPocket) {

				// Wall Collisions
				glm::vec3 pos = Pool.balls[i].GetPosition();
				bool isCollidingWall = (pos.x >= (wt - Ball::r)) ||
					(pos.x <= (-wt + Ball::r)) ||
					(pos.y >= (ht - Ball::r)) ||
					(pos.y <= (-ht + Ball::r));

				// Check if a ball fell in pocket
				bool isInPocket = false;
				// Control for middle pockets
				if ((pos.x >= -pr && pos.x <= pr) &&
					(pos.y >= ht - Ball::r || pos.y <= -ht + Ball::r)) {
					isInPocket = true;
				}
				// Control for left pockets
				if (pos.y <= -ht + pr || pos.y >= ht - pr) {
					if ((pos.x >= -wt && pos.x <= -wt + 1.1*pr) ||
						(pos.x >= wt - 1.1*pr	&& pos.x <= wt)) {
						isInPocket = true;
					}
				}
				if (isCollidingWall) {
					if (isInPocket) {
						Pool.BallFallsInPocket(i);
						Pool.ballInPocket = true;
					}
					else {
						Pool.CollisionWall(Pool.balls[i]);
					}
				}
			}
		}

		// Ball Collisions
		bool ballsAreMoving = false;
		for (int i = 0; i < nBalls; i++) {
			if (Pool.balls[i].isMoving) {
				ballsAreMoving = true;
				break;
			}
		}
		if (ballsAreMoving) {
			for (int i = 0; i < nBalls; i++) {
				if (!Pool.balls[i].isInPocket) {
					for (int j = 0; j < nBalls; j++) {
						if (!Pool.balls[j].isInPocket && j != i) {
							glm::vec3 x1 = Pool.balls[i].GetPosition();
							glm::vec3 x2 = Pool.balls[j].GetPosition();

							if (glm::length(x1 - x2) <= 2 * Ball::r) {
								Pool.CollisionBalls(Pool.balls[i], Pool.balls[j]);
								Pool.ballsHitNumber++;
								Pool.ballSound = true;
							}
						}
					}
				}
			}
		}
		else {
			if (Pool.turnStarted) {
				Pool.turnStarted = false;
				Pool.turnEnded = true;
			}
		}

		if (Pool.turnEnded) {
			if (Pool.ballInPocket) {
				Pool.ballInPocket = false;
				if (Pool.isFault) {					// Player put wrong ball in pocket
					Pool.Fault();
				}
				else {								// Player put correct ball in pocket
					Pool.NextTurn(true);
				}
			}
			else if (Pool.ballsHitNumber == 0) {	// White ball hasn't hit nothing
				Pool.Fault();
				Pool.balls[0].isInPocket = true;
			}
			else if (Pool.balls[0].isInPocket) {	// White ball in pocket
				Pool.Fault();
			}
			else {									// No faults
				Pool.NextTurn(false);
			}
			Pool.turnEnded = false;
			Pool.ballsHitNumber = 0;
			Pool.isFault = false;
		}


		// TEXT ----------------------------------------- start
		glMatrixMode(GL_MODELVIEW);				// Select The Modelview Matrix
		glLoadIdentity();						// Reset The Current Modelview Matrix
		glDisable(GL_TEXTURE_2D);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-this->plx, this->plx, -this->ply, this->ply, 0.0f, 1.0f);
		//glOrtho(-this->plx, this->plx, -this->ply, this->ply, 1, 5.1);
		//glOrtho(0.0f, this->Wwidth, this->Wheight, 0.0f, 0.0f, 1.0f);

		// Color
		glColor3f(1.0f, 1.0f, 1.0f);

		// Position The Text On The Screen
		//glRasterPos3f(-(float)plx + PixToCoord_X(10), (float)ply - PixToCoord_Y(21), 0.0f);

		//// compute fps and write text
		//this->frames++;
		//if (this->frames > 18) {
		//	this->fps = frames / frameTime;
		//	this->frames = 0; this->frameTime = 0;
		//}
		//this->glPrint("Elapsed time: %6.2f sec.  -  Fps %6.2f", Full_elapsed, fps);

		//glRasterPos3f(-(float)plx + PixToCoord_X(10), (float)-ply + PixToCoord_Y(81), 0.0f);
		//this->glPrint("Velocity: %1.6f ", Pool.balls[15].GetSlidingVelocity().x);

		//glRasterPos3f(-(float)plx + PixToCoord_X(10), (float)-ply + PixToCoord_Y(101), 0.0f);
		//this->glPrint("Phase: %2.6f %2.6f %2.6f",
		//	Pool.balls[0].GetPosition().x,
		//	Pool.balls[0].GetPosition().y,
		//	Pool.balls[0].GetPosition().z);

		if (this->Full_elapsed < 6) {
			glRasterPos3f(-(float)plx + PixToCoord_X(10), (float)-ply + PixToCoord_Y(51), 0.0f);
			this->glPrint("...V to change point of view");
		}

		{
			if (Pool.isPlayer1Turn) {
				glColor3f(1.0f, 0.0f, 0.0f);
			}
			else {
				glColor3f(0.0f, 0.0f, 1.0f);
			}
			glRasterPos3f(-(float)plx + PixToCoord_X(10), (float)-ply + PixToCoord_Y(11), 0.0f);
			this->glPrint("Turn: %s", Pool.isPlayer1Turn ? "Player 1" : "Player 2");

			glColor3f(1.0f, 1.0f, 1.0f);
			glRasterPos3f(-(float)plx + PixToCoord_X(10), (float)-ply + PixToCoord_Y(31), 0.0f);
			char* s1;
			char* s2;
			switch (Pool.player1BallType)
			{
			case Ball::BallType::Undefined:
				s1 = "undefined";
				break;
			case Ball::BallType::Solid:
				s1 = "solid";
				break;
			case Ball::BallType::Striped:
				s1 = "striped";
				break;
			default:
				break;
			}
			switch (Pool.player2BallType)
			{
			case Ball::BallType::Undefined:
				s2 = "undefined";
				break;
			case Ball::BallType::Solid:
				s2 = "solid";
				break;
			case Ball::BallType::Striped:
				s2 = "striped";
				break;
			default:
				break;
			}
			this->glPrint("Player1: %s \t Player2: %s", s1, s2);
		}
	}
	// PLAY ---------------------------------------- start

	// HOME ----------------------------------------- start
	if (Pool.gameState == Poolgame::GameState::HOME) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
		glMatrixMode(GL_MODELVIEW);				// Select The Modelview Matrix
		glLoadIdentity();									// Reset The View
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, start);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//  Background
		glBegin(GL_QUADS);
		for (int i = 0; i < 4; i++) {
			glTexCoord2f(Background[i].u, Background[i].v);
			glVertex3f(Background[i].x, Background[i].y, Background[i].z);
		}
		glEnd();
	}
	// HOME  ----------------------------------------- end

	// RULES ----------------------------------------- start
	if (Pool.gameState == Poolgame::GameState::RULES) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
		glMatrixMode(GL_MODELVIEW);				// Select The Modelview Matrix
		glLoadIdentity();									// Reset The View
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, rules);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//  Background
		glBegin(GL_QUADS);
		for (int i = 0; i < 4; i++) {
			glTexCoord2f(Background[i].u, Background[i].v);
			glVertex3f(Background[i].x, Background[i].y, Background[i].z);
		}
		glEnd();
	}
	// RULES  ----------------------------------------- end

	// PLAYER1 WIN ----------------------------------- start
	if (Pool.gameState == Poolgame::GameState::PLY1_WIN) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
		glMatrixMode(GL_MODELVIEW);				// Select The Modelview Matrix
		glLoadIdentity();									// Reset The View
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, win[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//  Background
		glBegin(GL_QUADS);
		for (int i = 0; i < 4; i++) {
			glTexCoord2f(Background[i].u, Background[i].v);
			glVertex3f(Background[i].x, Background[i].y, Background[i].z);
		}
		glEnd();
	}
	// PLAYER1 WIN ----------------------------------- end

	// PLAYER1 WIN ----------------------------------- start
	if (Pool.gameState == Poolgame::GameState::PLY2_WIN) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
		glMatrixMode(GL_MODELVIEW);				// Select The Modelview Matrix
		glLoadIdentity();									// Reset The View
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, win[1]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//  Background
		glBegin(GL_QUADS);
		for (int i = 0; i < 4; i++) {
			glTexCoord2f(Background[i].u, Background[i].v);
			glVertex3f(Background[i].x, Background[i].y, Background[i].z);
		}
		glEnd();
	}
	// PLAYER1 WIN ----------------------------------- end

	glEnable(GL_TEXTURE_2D);
	return true;
}


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//  bitmap fonts
void MyModel::BuildFont(void)								// Build Our Bitmap Font
{
	HFONT	font;										// Windows Font ID
	HFONT	oldfont;									// Used For Good House Keeping

	base = glGenLists(96);								// Storage For 96 Characters

	font = CreateFont(	-20,							// Height Of Font
						0,								// Width Of Font
						0,								// Angle Of Escapement
						0,								// Orientation Angle
						FW_BOLD,						// Font Weight
						FALSE,							// Italic
						FALSE,							// Underline
						FALSE,							// Strikeout
						ANSI_CHARSET,					// Character Set Identifier
						OUT_TT_PRECIS,					// Output Precision
						CLIP_DEFAULT_PRECIS,			// Clipping Precision
						ANTIALIASED_QUALITY,			// Output Quality
						FF_DONTCARE|DEFAULT_PITCH,		// Family And Pitch
						"Courier New");					// Font Name

	oldfont = (HFONT)SelectObject(hDC, font);           // Selects The Font We Want
	wglUseFontBitmaps(hDC, 32, 96, base);				// Builds 96 Characters Starting At Character 32
	SelectObject(hDC, oldfont);							// Selects The Font We Want
	DeleteObject(font);									// Delete The Font
}

void MyModel::KillFont(void)							// Delete The Font List
{
	glDeleteLists(base, 96);							// Delete All 96 Characters
}

void MyModel::glPrint(const char *fmt, ...)				// Custom GL "Print" Routine
{
	char		text[256];								// Holds Our String
	va_list		ap;										// Pointer To List Of Arguments

	if (fmt == NULL)									// If There's No Text
		return;											// Do Nothing

	va_start(ap, fmt);									// Parses The String For Variables
	    vsprintf(text, fmt, ap);						// And Converts Symbols To Actual Numbers
	va_end(ap);											// Results Are Stored In Text

	glPushAttrib(GL_LIST_BIT);							// Pushes The Display List Bits
	glListBase(base - 32);								// Sets The Base Character to 32
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);	// Draws The Display List Text
	glPopAttrib();										// Pops The Display List Bits
}