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

class VM;
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

	/// Exits the current cursor and loads the next from the stack
	///
	/// @return true on cursors remaining
	bool popCursor() {
		delete cursor;
		cursor = NULL;

		if (link.empty()) {
			std::cerr << "Popped cursor, end-of-LS" << std::endl;
			state = ETS_DEAD;
			return false;
		} else {
			cursor = link.top();
			link.pop();
			std::cerr << "Popped cursor, " << link.size()
				<< " remaining in LS" << std::endl;
			return true;
		}
	}

	void pushCursor(Cursor *nc) {
		if (cursor) {
			link.push(cursor);
		}
		cursor = nc;
		state = ETS_READY;
		std::cerr << "Pushed cursor!" << std::endl;
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

	void step(VM *vm, std::map<int, Subroutine*> &srman);
};
