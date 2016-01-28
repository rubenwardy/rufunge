#pragma once
#include "canvas.hpp"
#include "subroutine.hpp"
#include <iostream>
#include <assert.h>

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
	Canvas *canvas = NULL;
	int x = 0;
	int y = 0;
	EDIR dir = RIGHT;
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

enum ETHREAD_STATE
{
	ETS_DEAD,
	ETS_READY,
	ETS_STRING,
	ETS_NUMBER
};

class Thread
{
public:
	Cursor *cursor;
	std::stack<Cursor*> link;
	std::stack<char> stack;
	ETHREAD_STATE state = ETS_READY;

	inline void setDir(EDIR dir)
	{
		assert(cursor);
		cursor->dir = dir;
	}

	inline void move() {
		assert(cursor);
		cursor->move();
	}
	inline void turnLeft() {
		assert(cursor);
		cursor->turnLeft();
	}
	inline void turnRight() {
		assert(cursor);
		cursor->turnRight();
	}
	inline char getChar() {
		assert(cursor);
		return cursor->getChar();
	}

	char pop() {
		char retval = 0;
		if (!stack.empty()) {
			retval = stack.top();
			stack.pop();
		}
		return retval;
	}
	inline void push(char v) {
		stack.push(v);
	}

	Thread(Canvas *c)
	{
		std::cerr << "Created new thread!" << std::endl;
		cursor = new Cursor();
		cursor->canvas = c;
	}

	void step(std::map<int, Subroutine*> &srman)
	{
		assert(cursor);
		assert(cursor->canvas);
		char ch = getChar();

		switch (state) {
		case ETS_DEAD:
			std::cerr << "Skipping stepping of dead thread." << std::endl;
			break;
		case ETS_READY: {
			if (ch == '\"') {
				std::cerr << "Enter string mode!" << std::endl;
				state = ETS_STRING;
				move();
			} else if (ch == '`') {
				std::cerr << "Enter numeric mode!" << std::endl;
				state = ETS_NUMBER;
				move();
			} else {
				int id = cursor->getOperator(ch);
				if (id >= 0) {
					// TODO: run operator
					std::cerr << "Running " << ch << " which is subroutine " << id << std::endl;
					auto it = srman.find(id);
					if (it != srman.end()) {
						it->second->run(this);
					} else {
						state = ETS_DEAD;
						std::cerr << "ERROR: Unable to find operator in memory: " << ch << std::endl;
					}
				} else if (ch == ' ') {
					std::cerr << "Move" << std::endl;
					move();
				} else {
					state = ETS_DEAD;
					std::cerr << "ERROR: Undefined operator: " << ch << std::endl;
				}
			}
			break;
		}
		case ETS_STRING: {
			if (ch == '\"') {
				std::cerr << "Exit string mode!" << std::endl;
				state = ETS_READY;
			} else {
				std::cerr << "Push " << ch << std::endl;
				stack.push(ch);
			}
			move();
			break;
		}
		default:
			state = ETS_DEAD;
			std::cerr << "ERROR: Cursor has invalid state." << std::endl;
			break;
		}
	}
};
