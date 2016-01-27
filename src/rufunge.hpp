#pragma once
#include "cursor.hpp"

class VM;
class Subroutine
{
public:
	/// Return true to
	virtual bool init(VM *vm, Thread *th) = 0;
};

class VM
{
public:
	std::vector<Thread*> threads;
	std::map<int, Subroutine*> subroutines;

	int numAliveThreads()
	{
		return threads.size();
	}

	void init(Canvas *canvas)
	{
		
	}
};
