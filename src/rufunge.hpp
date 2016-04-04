#pragma once
#include "thread.hpp"

class VM
{
	int sr_count = 0;
public:
	~VM();

	std::vector<Thread*> threads;
	std::map<int, Subroutine*> subroutines;

	int numAliveThreads();
	int loadSubroutine(Subroutine *sr);

	void assignStandardSR(Thread *th);

	void init(Canvas *canvas);

	void step();
};
