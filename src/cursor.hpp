#pragma once
#include "canvas.hpp"

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
	Canvas *canvas;
	int x, y;
	EDIR dir;
	std::map<char, int> operators;

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
};

class Thread
{
public:
	Cursor *cursor;
	std::stack<Cursor*> link;
	std::stack<char> stack;
};
