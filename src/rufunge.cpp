#include "rufunge.hpp"

#include "operators.hpp"

VM::~VM()
{
	std::cerr << "Deconstructing VM..." << std::endl;
	for (Thread *th : threads) {
		delete th;
	}
	threads.clear();

	for (auto pair : subroutines) {
		delete pair.second;
	}
	subroutines.clear();
}

int VM::numAliveThreads()
{
	int c = 0;
	for (Thread *th : threads) {
		if (th->getState() > ETS_DEAD)
			c++;
	}
	return c;
}

int VM::loadSubroutine(Subroutine *sr)
{
	sr_count++;
	subroutines[sr_count] = sr;
	return sr_count;
}

void VM::init(Canvas *canvas)
{
	srand (time(NULL));

	assert(threads.size() == 0);
	assert(subroutines.size() == 0);

	// Create thread
	Thread *th = new Thread(canvas);
	initThread(th);
	threads.push_back(th);

	// Create subroutines
	loadSubroutines(this);
}

void VM::initThread(Thread *th)
{
	assignStandardSR(th);
}

void VM::step()
{
	for (Thread *th : threads) {
		th->step(this, subroutines);
	}
}
