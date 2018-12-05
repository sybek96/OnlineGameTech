#pragma once

enum class Color { RED, BLUE, GREEN };
class Circle
{
public:
	Circle();
	Circle(int rad, int x, int y, Color _col);
	~Circle();
	int m_radius;
	int m_xPos;
	int m_yPos;
	Color m_color;
};

