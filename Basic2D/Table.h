#pragma once

#include <vector>
#include "glm/glm.hpp"

class Table
{
public:
	const float w, h;		// sizes: w=200cm h=100cm
	const float pocket_r;
	std::vector<glm::vec3> pockets;

	Table();
	~Table();
	void Draw();

private:
	const float w_border, h_border;		// Size of table borders

	void DrawPlane();					// Draw horizontal surface
	void DrawExternalVerticalBorders();	// Draw external vertical borders
	void DrawInternalVerticalBorders();	// Draw internal vertical borders
	void DrawHorizontalBorders();		// Draw horizontal borders
	void DrawPockets();					// Draw black circles for pockets
};

