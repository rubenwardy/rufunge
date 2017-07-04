#pragma once
#include "canvas.hpp"
#include <memory>

enum EDIR {
	RIGHT,
	DOWN,
	LEFT,
	UP
};

inline EDIR dirRight(EDIR dir)
{
	int tmp = ((int)dir + 1) % 4;
	return (EDIR)((tmp < 0) ? -tmp : tmp);
}

inline EDIR dirLeft(EDIR dir)
{
	// +3 is equiv to -1 in modular arith.
	int tmp = ((int)dir + 3) % 4;
	return (EDIR)((tmp < 0) ? -tmp : tmp);
}


class Cursor
{
public:
	std::shared_ptr<Canvas> canvas;
	int x = 0;
	int y = 0;
	EDIR dir = RIGHT;
	std::map<char, int> operators;

	Cursor(std::shared_ptr<Canvas> canvas):
	 	canvas(canvas)
	{}

	Cursor(const Cursor &other) {
		canvas = other.canvas;
		x = other.x;
		y = other.y;
		dir = other.dir;
		operators = other.operators;
	}

	inline void move()
	{
		switch(dir) {
		case RIGHT:
			x++;
			break;
		case DOWN:
			y++;
			break;
		case LEFT:
			x--;
			break;
		case UP:
			y--;
			break;
		}
	}

	inline void turnRight()
	{
		dir = dirRight(dir);
	}

	inline void turnLeft()
	{
		dir = dirLeft(dir);
	}

	inline char getChar()
	{
		return canvas->get(x, y);
	}

	inline int getOperator(char ch)
	{
		auto it = operators.find(ch);
		return (it == operators.end()) ? -1 : it->second;
	}
};
