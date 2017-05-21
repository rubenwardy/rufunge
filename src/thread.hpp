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
	Cursor *cursor;
	std::stack<Cursor*> link;
	std::stack<char> stack;
	ETHREAD_STATE state = ETS_READY;
public:

	Thread(Canvas *c);
	~Thread();

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

	inline void setCanvasCell(int x, int y, char value) {
		cursor->canvas->set(x, y, value);
	}

	inline char getCanvasCell(int x, int y) {
		return cursor->canvas->get(x, y);
	}

	inline void setOperator(char c, int id) {
		cursor->operators[c] = id;
	}

	inline ETHREAD_STATE getState() const {
		return state;
	}

	inline void setState(ETHREAD_STATE v) {
		state = v;
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
