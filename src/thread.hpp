#pragma once
#include "cursor.hpp"
#include "subroutine.hpp"
#include <iostream>
#include <assert.h>

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

	Thread(Canvas *c);

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
	bool popCursor();

	void pushCursor(Cursor *nc);

	char pop();

	inline void push(char v) {
		stack.push(v);
	}


	void step(VM *vm, std::map<int, Subroutine*> &srman);
};
