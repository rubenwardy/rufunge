#pragma once

class Thread;
class Subroutine
{
public:
	int users = 0;
	void grab() { users++; }
	void drop() { users--; }

	/// Either performs the calculation and adds it to the stack, or pops a new cursor onto
	/// The stack
	virtual void run(Thread *th) = 0;
};
