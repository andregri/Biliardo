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
	
	
	glOrtho(-this->plx, this->plx, -this->ply, this->ply, -2.0f, 2.0f);
	//gluPerspective(45.0, (GLfloat)iWidth / (GLfloat)iHeight, 0.1, 100.0);
	//gluLookAt(	0.0, 0.0, 2.5,		// eye
	//			0.0, 0.0, 0.0,		// center
	//			0.0, 1.0, 0.0);		// up vector

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
		this->plx = 1.1;
		this->ply = (double)height / (double)width;
	}
	else {
		this->plx = (double)width / (double)height;
		this->ply = 1.1;
	}

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

	// Typical Texture Generation Using Data From The Bitmap
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

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


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The View

	// Draw a table
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	Pool.table.Draw();

	// Draw cursor line
	if (captured) {
		glBegin(GL_LINES);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(ClientX2World(cx), ClientY2World(cy), Ball::r);
		glm::vec3 x_ = Pool.balls[0].GetPosition();
		glVertex3f(x_[0], x_[1], Ball::r);
		glEnd();
	}

	// set Textures to all balls
	for (int i = 0; i < 16; i++) {
		// Always draw white ball, but not balls in pockets
		if (!Pool.balls[i].isInPocket || i == 0) {
			glEnable(GL_TEXTURE_2D);
			glMatrixMode(GL_MODELVIEW);				// Select The Modelview Matrix
			glLoadIdentity();
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
			glBindTexture(GL_TEXTURE_2D, texture[i]);
			Pool.balls[i].Draw();
		}
	}

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

	
	float ht = Pool.table.h/2;
	float wt = Pool.table.w/2;
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
					Pool.balls[i].isInPocket = true;
				}
				else {
					Pool.CollisionWall(Pool.balls[i]);
				}
			}
		}
	}

	// Ball Collision
	if (!Pool.balls[0].isInPocket) {
		for (int i = 0; i < nBalls; i++) {
			if (!Pool.balls[i].isInPocket) {
				for (int j = 0; j < nBalls && j != i; j++) {
					if (!Pool.balls[i].isInPocket) {
						glm::vec3 x1 = Pool.balls[i].GetPosition();
						glm::vec3 x2 = Pool.balls[j].GetPosition();

						if (glm::length(x1 - x2) <= 2 * Ball::r) {
							Pool.CollisionBalls(Pool.balls[i], Pool.balls[j]);
						}
					}
				}
			}
		}
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
	glColor3f(1.0f,1.0f,1.0f);

	// Position The Text On The Screen
	glRasterPos3f(- (float) plx + PixToCoord_X(10), (float) ply-PixToCoord_Y(21), 0.0f);

	// compute fps and write text
	this->frames++;
	if( this->frames > 18 ) {
		this->fps = frames / frameTime;
		this->frames = 0; this->frameTime = 0;
	}
	this->glPrint("Elapsed time: %6.2f sec.  -  Fps %6.2f", Full_elapsed, fps);

	glRasterPos3f(-(float)plx + PixToCoord_X(10), (float)-ply + PixToCoord_Y(81), 0.0f);
	this->glPrint("Velocity: %1.6f ", Pool.balls[15].GetSlidingVelocity().x);

	glRasterPos3f(-(float)plx + PixToCoord_X(10), (float)-ply + PixToCoord_Y(101), 0.0f);
	this->glPrint("Phase: %2.6f %2.6f %2.6f",
		Pool.balls[0].GetPosition().x,
		Pool.balls[0].GetPosition().y,
		Pool.balls[0].GetPosition().z);

	if(this->Full_elapsed < 6) {
		glRasterPos3f(- (float) plx + PixToCoord_X(10), (float) -ply+PixToCoord_Y(21), 0.0f);
		this->glPrint("...F2/F3/F4 for sounds");
	}

	{
		glRasterPos3f(- (float) plx + PixToCoord_X(10), (float) -ply+PixToCoord_Y(61), 0.0f);
		this->glPrint("%1d %1d  %s",cx,cy, captured ? "captured" : "Not captured" );
	}

	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping
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