#include "Table.h"

#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <math.h>
#include "Ball.h"

Table::Table()
	: w(2.0f), h(1.0f), w_border(0.09f), h_border(0.04f),
	pocket_r(Ball::r+0.02)
{
	float wt, ht;
	wt = w / 2;
	ht = h / 2;

	pockets.push_back(glm::vec3(- wt, ht, 0.0f));	// top left
	pockets.push_back(glm::vec3(0.0, ht + pocket_r, 0.0f));	// top middle
	pockets.push_back(glm::vec3(wt, ht, 0.0f));		// top right
	pockets.push_back(glm::vec3(-wt, -ht, 0.0f));	// bottom left
	pockets.push_back(glm::vec3(0.0, -ht - pocket_r, 0.0f));	// bottom middle
	pockets.push_back(glm::vec3(wt, -ht, 0.0f));	// bottom right
}


Table::~Table()
{
}

void Table::Draw()
{
	DrawPlane();
	DrawExternalVerticalBorders();
	DrawInternalVerticalBorders();
	DrawHorizontalBorders();
	DrawPockets();
}

void Table::DrawPlane()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glBegin(GL_QUADS);
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(-w/2 - w_border,  h/2 + w_border, 0.0);		// left-up
	glVertex3f( w/2 + w_border,  h/2 + w_border, 0.0);		// right-up
	glVertex3f( w/2 + w_border, -h/2 - w_border, 0.0);		// right-down
	glVertex3f(-w/2 - w_border, -h/2 - w_border, 0.0);		// left-down
	glEnd();
}

void Table::DrawExternalVerticalBorders()
{
	float wt, ht;
	wt = w / 2 + w_border;
	ht = h / 2 + w_border;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor3f(0.1, 0.5, 0.1);

	// Draw top border
	glBegin(GL_QUADS);
	glVertex3f(-wt, ht, 0.0f);
	glVertex3f(-wt, ht, h_border);
	glVertex3f(wt, ht, h_border);
	glVertex3f(wt, ht, 0.0f);
	glEnd();

	// Draw right border
	glBegin(GL_QUADS);
	glVertex3f(wt, ht, 0.0f);
	glVertex3f(wt, ht, h_border);
	glVertex3f(wt, -ht, h_border);
	glVertex3f(wt, -ht, 0.0f);
	glEnd();

	// Draw bottom border
	glBegin(GL_QUADS);
	glVertex3f(wt, -ht, 0.0f);
	glVertex3f(wt, -ht, h_border);
	glVertex3f(-wt, -ht, h_border);
	glVertex3f(-wt, -ht, 0.0f);
	glEnd();

	// Draw left border
	glBegin(GL_QUADS);
	glVertex3f(-wt, ht, 0.0f);
	glVertex3f(-wt, ht, h_border);
	glVertex3f(-wt, -ht, h_border);
	glVertex3f(-wt, -ht, 0.0f);
	glEnd();
}

void Table::DrawInternalVerticalBorders()
{
	float wt, ht;
	wt = w / 2;
	ht = h / 2;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor3f(0.1, 0.5, 0.1);

	// Draw top border: left half
	glBegin(GL_QUADS);
	glVertex3f(-wt + 1.5*pocket_r, ht, 0.0f);
	glVertex3f(-wt + 1.5*pocket_r, ht, h_border);
	glVertex3f(-pocket_r, ht, h_border);
	glVertex3f(-pocket_r, ht, 0.0f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(-wt + 1.5*pocket_r, ht, 0.0f);
	glVertex3f(-wt + 1.5*pocket_r, ht, h_border);
	glVertex3f(-wt - w_border + 1.5*pocket_r, ht + w_border, h_border);
	glVertex3f(-wt - w_border + 1.5*pocket_r, ht + w_border, 0.0f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(-pocket_r, ht, 0.0f);
	glVertex3f(-pocket_r, ht, h_border);
	glVertex3f(-pocket_r, ht+w_border, h_border);
	glVertex3f(-pocket_r, ht+w_border, 0.0f);
	glEnd();
	// Draw top border: right half
	glBegin(GL_QUADS);
	glVertex3f(pocket_r, ht, 0.0f);
	glVertex3f(pocket_r, ht, h_border);
	glVertex3f(wt - 1.5*pocket_r, ht, h_border);
	glVertex3f(wt - 1.5*pocket_r, ht, 0.0f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(pocket_r, ht, 0.0f);
	glVertex3f(pocket_r, ht, h_border);
	glVertex3f(pocket_r, ht + w_border, h_border);
	glVertex3f(pocket_r, ht + w_border, 0.0f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(wt - 1.5*pocket_r, ht, 0.0f);
	glVertex3f(wt - 1.5*pocket_r, ht, h_border);
	glVertex3f(wt + w_border - 1.5*pocket_r, ht + w_border, h_border);
	glVertex3f(wt + w_border - 1.5*pocket_r, ht + w_border, 0.0f);
	glEnd();

	// Draw right border
	glBegin(GL_QUADS);
	glVertex3f(wt, ht - 1.5*pocket_r, 0.0f);
	glVertex3f(wt, ht - 1.5*pocket_r, h_border);
	glVertex3f(wt, -ht + 1.5*pocket_r, h_border);
	glVertex3f(wt, -ht + 1.5*pocket_r, 0.0f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(wt, ht-1.5*pocket_r, 0.0f);
	glVertex3f(wt, ht-1.5*pocket_r, h_border);
	glVertex3f(wt + w_border, ht+w_border-1.5*pocket_r, h_border);
	glVertex3f(wt + w_border, ht+w_border-1.5*pocket_r, 0.0f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(wt, -ht + 1.5*pocket_r, 0.0f);
	glVertex3f(wt, -ht + 1.5*pocket_r, h_border);
	glVertex3f(wt + w_border, -ht - w_border + 1.5*pocket_r, h_border);
	glVertex3f(wt + w_border, -ht - w_border + 1.5*pocket_r, 0.0f);
	glEnd();

	// Draw bottom border: right half
	glBegin(GL_QUADS);
	glVertex3f(wt - 1.5*pocket_r, -ht, 0.0f);
	glVertex3f(wt - 1.5*pocket_r, -ht, h_border);
	glVertex3f(pocket_r, -ht, h_border);
	glVertex3f(pocket_r, -ht, 0.0f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(pocket_r, -ht, 0.0f);
	glVertex3f(pocket_r, -ht, h_border);
	glVertex3f(pocket_r, -ht - w_border, h_border);
	glVertex3f(pocket_r, -ht - w_border, 0.0f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(wt - 1.5*pocket_r, -ht, 0.0f);
	glVertex3f(wt - 1.5*pocket_r, -ht, h_border);
	glVertex3f(wt + w_border - 1.5*pocket_r, -ht - w_border, h_border);
	glVertex3f(wt + w_border - 1.5*pocket_r, -ht - w_border, 0.0f);
	glEnd();
	// Draw bottom border: left half
	glBegin(GL_QUADS);
	glVertex3f(-wt + 1.5*pocket_r, -ht, 0.0f);
	glVertex3f(-wt + 1.5*pocket_r, -ht, h_border);
	glVertex3f(-pocket_r, -ht, h_border);
	glVertex3f(-pocket_r, -ht, 0.0f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(-wt + 1.5*pocket_r, -ht, 0.0f);
	glVertex3f(-wt + 1.5*pocket_r, -ht, h_border);
	glVertex3f(-wt - w_border + 1.5*pocket_r, -ht - w_border, h_border);
	glVertex3f(-wt - w_border + 1.5*pocket_r, -ht - w_border, 0.0f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(-pocket_r, -ht, 0.0f);
	glVertex3f(-pocket_r, -ht, h_border);
	glVertex3f(-pocket_r, -ht - w_border, h_border);
	glVertex3f(-pocket_r, -ht - w_border, 0.0f);
	glEnd();

	// Draw left border
	glBegin(GL_QUADS);
	glVertex3f(-wt, ht - 1.5*pocket_r, 0.0f);
	glVertex3f(-wt, ht - 1.5*pocket_r, h_border);
	glVertex3f(-wt, -ht + 1.5*pocket_r, h_border);
	glVertex3f(-wt, -ht + 1.5*pocket_r, 0.0f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(-wt, ht - 1.5*pocket_r, 0.0f);
	glVertex3f(-wt, ht - 1.5*pocket_r, h_border);
	glVertex3f(-wt - w_border, ht + w_border - 1.5*pocket_r, h_border);
	glVertex3f(-wt - w_border, ht + w_border - 1.5*pocket_r, 0.0f);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(-wt, -ht + 1.5*pocket_r, 0.0f);
	glVertex3f(-wt, -ht + 1.5*pocket_r, h_border);
	glVertex3f(-wt - w_border, -ht - w_border + 1.5*pocket_r, h_border);
	glVertex3f(-wt - w_border, -ht - w_border + 1.5*pocket_r, 0.0f);
	glEnd();

	// For pocket enclosures
	// Draw top border
	glBegin(GL_QUADS);
	glVertex3f(-wt-w_border, ht+1.5*pocket_r, 0.0f);
	glVertex3f(-wt-w_border, ht + 1.5*pocket_r, h_border);
	glVertex3f(wt+w_border, ht + 1.5*pocket_r, h_border);
	glVertex3f(wt+w_border, ht + 1.5*pocket_r, 0.0f);
	glEnd();

	// Draw right border
	glBegin(GL_QUADS);
	glVertex3f(wt + 1.5*pocket_r, ht+w_border, 0.0f);
	glVertex3f(wt + 1.5*pocket_r, ht + w_border, h_border);
	glVertex3f(wt + 1.5*pocket_r, -ht-w_border, h_border);
	glVertex3f(wt + 1.5*pocket_r, -ht-w_border, 0.0f);
	glEnd();

	// Draw bottom border
	glBegin(GL_QUADS);
	glVertex3f(wt+ w_border, -ht-1.5*pocket_r, 0.0f);
	glVertex3f(wt+ w_border, -ht - 1.5*pocket_r, h_border);
	glVertex3f(-wt- w_border, -ht - 1.5*pocket_r, h_border);
	glVertex3f(-wt- w_border, -ht - 1.5*pocket_r, 0.0f);
	glEnd();

	// Draw left border
	glBegin(GL_QUADS);
	glVertex3f(-wt - 1.5*pocket_r, ht + w_border, 0.0f);
	glVertex3f(-wt - 1.5*pocket_r, ht + w_border, h_border);
	glVertex3f(-wt - 1.5*pocket_r, -ht - w_border, h_border);
	glVertex3f(-wt - 1.5*pocket_r, -ht - w_border, 0.0f);
	glEnd();
}

void Table::DrawHorizontalBorders()
{
	float wt, ht;
	wt = w / 2;
	ht = h / 2;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor3f(0.1, 0.5, 0.1);

	// Top: left half
	glBegin(GL_QUADS);
	glVertex3f(-wt+1.5*pocket_r, ht, h_border);
	glVertex3f(-wt - w_border + 1.5*pocket_r, ht + w_border, h_border);
	glVertex3f(-pocket_r, ht+w_border, h_border);
	glVertex3f(-pocket_r, ht, h_border);
	glEnd();
	// Top: right half
	glBegin(GL_QUADS);
	glVertex3f(pocket_r, ht, h_border);
	glVertex3f(pocket_r, ht + w_border, h_border);
	glVertex3f(wt+w_border-1.5*pocket_r, ht + w_border, h_border);
	glVertex3f(wt-1.5*pocket_r, ht, h_border);
	glEnd();

	// Bottom: left half
	glBegin(GL_QUADS);
	glVertex3f(-wt+1.5*pocket_r, -ht, h_border);
	glVertex3f(-wt+1.5*pocket_r - w_border, -ht - w_border, h_border);
	glVertex3f(-pocket_r, -ht - w_border, h_border);
	glVertex3f(-pocket_r, -ht, h_border);
	glEnd();
	// Bottom: right half
	glBegin(GL_QUADS);
	glVertex3f(pocket_r, -ht, h_border);
	glVertex3f(pocket_r, -ht - w_border, h_border);
	glVertex3f(wt + w_border - 1.5*pocket_r, -ht - w_border, h_border);
	glVertex3f(wt - 1.5*pocket_r, -ht, h_border);
	glEnd();

	// Right
	glBegin(GL_QUADS);
	glVertex3f(wt, ht - 1.5*pocket_r, h_border);
	glVertex3f(wt+w_border, ht + w_border - 1.5*pocket_r, h_border);
	glVertex3f(wt+w_border, -ht - w_border + 1.5*pocket_r, h_border);
	glVertex3f(wt, -ht + 1.5*pocket_r, h_border);
	glEnd();

	// Left
	glBegin(GL_QUADS);
	glVertex3f(-wt, ht - 1.5*pocket_r, h_border);
	glVertex3f(-wt - w_border, ht + w_border - 1.5*pocket_r, h_border);
	glVertex3f(-wt - w_border, -ht - w_border + 1.5*pocket_r, h_border);
	glVertex3f(-wt, -ht + 1.5*pocket_r, h_border);
	glEnd();

	// Draw pocket enclosures
	// Top
	glBegin(GL_QUADS);
	glVertex3f(-wt - w_border, ht + 1.5*pocket_r, h_border);
	glVertex3f(-wt - w_border, ht + w_border, h_border);
	glVertex3f(+wt + w_border, ht + w_border, h_border);
	glVertex3f(+wt + w_border, ht + 1.5*pocket_r, h_border);
	glEnd();
	// Bottom
	glBegin(GL_QUADS);
	glVertex3f(-wt - w_border, -ht - 1.5*pocket_r, h_border);
	glVertex3f(-wt - w_border, -ht - w_border, h_border);
	glVertex3f(wt + w_border, -ht - w_border, h_border);
	glVertex3f(wt + w_border, -ht - 1.5*pocket_r, h_border);
	glEnd();
	// Left
	glBegin(GL_QUADS);
	glVertex3f(-wt - w_border, ht + w_border, h_border);
	glVertex3f(-wt - 1.5*pocket_r, ht + w_border, h_border);
	glVertex3f(-wt - 1.5*pocket_r, -ht - w_border, h_border);
	glVertex3f(-wt - w_border, -ht - w_border, h_border);
	glEnd();
	// Right
	glBegin(GL_QUADS);
	glVertex3f(wt + w_border, ht + w_border, h_border);
	glVertex3f(wt + 1.5 * pocket_r, ht + w_border, h_border);
	glVertex3f(wt + 1.5 * pocket_r, -ht - w_border, h_border);
	glVertex3f(wt + w_border, -ht - w_border, h_border);
	glEnd();
}

void Table::DrawPockets()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor3f(0.0, 0.0, 0.0);
	
	for (int p = 0; p < pockets.size(); p++) {
		for (int i = 0; i < 4; i++)
		{
			glBegin(GL_POLYGON);
			for (double i = 0; i < 2 * 3.14; i += 3.14 / 6) {
				float x = cos(i) * pocket_r + pockets[p].x;
				float y = sin(i) * pocket_r + pockets[p].y;
				float z = pockets[p].z;
				glVertex3f(x, y, z);
			}
			glEnd();
		}
	}
	
}
