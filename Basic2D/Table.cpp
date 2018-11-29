#include "Table.h"

#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library

Table::Table()
	: width(2), height(1)
{
}


Table::~Table()
{
}

void Table::draw()
{
	glBegin(GL_QUADS);
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(-width/2,  height/2, 0.0);		// left-up
	glVertex3f( width/2,  height/2, 0.0);		// right-up
	glVertex3f( width/2, -height/2, 0.0);		// right-down
	glVertex3f(-width/2, -height/2, 0.0);		// left-down
	glEnd();
}
