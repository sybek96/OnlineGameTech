#include "Circle.h"



Circle::Circle()
{
}

Circle::Circle(int rad, int x, int y, Color _col)
	: m_radius(rad)
	, m_xPos(x)
	, m_yPos(y)
	, m_color(_col)
{
}


Circle::~Circle()
{
}
