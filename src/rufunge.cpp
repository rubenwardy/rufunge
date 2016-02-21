#include "rufunge.hpp"
#include <cmath>
#include <iostream>
#include <assert.h>

class DirSR : public Subroutine
{
public:
	virtual void run(Thread *th)
	{
		char ch = th->getChar();
		switch (ch) {
		case '>':
			std::cerr << "right" << std::endl;
			th->setDir(RIGHT);
			break;
		case 'v':
			std::cerr << "down" << std::endl;
			th->setDir(DOWN);
			break;
		case '<':
			std::cerr << "left" << std::endl;
			th->setDir(LEFT);
			break;
		case '^':
			std::cerr << "up" << std::endl;
			th->setDir(UP);
			break;
		}
		th->move();
	}
};

class NumSR : public Subroutine
{
public:
	virtual void run(Thread *th)
	{
		char ch = th->getChar();
		int num = ch - '0';
		if (num < 0 || num > 9) {
			th->state = ETS_DEAD;
			std::cerr << "ERROR: Character is not a number." << std::endl;
			return;
		}

		th->push(num);
		std::cerr << "Pushed " << ch << std::endl;
		th->move();
	}
};

class PrintCharSR : public Subroutine
{
public:
	virtual void run(Thread *th)
	{
		char ch = th->pop();
		std::cout << ch;
		th->move();
	}
};

class PrintNumSR : public Subroutine
{
public:
	virtual void run(Thread *th)
	{
		char ch = th->pop();
		std::cout << (int)ch;
		th->move();
	}
};

class AddSR : public Subroutine
{
public:
	virtual void run(Thread *th)
	{
		char a = th->pop();
		char b = th->pop();
		th->push(b + a);

		th->move();
	}
};

class TakeSR : public Subroutine
{
public:
	virtual void run(Thread *th)
	{
		char a = th->pop();
		char b = th->pop();
		th->push(b - a);

		th->move();
	}
};

class MultSR : public Subroutine
{
public:
	virtual void run(Thread *th)
	{
		char a = th->pop();
		char b = th->pop();
		th->push(b * a);

		th->move();
	}
};

class DivSR : public Subroutine
{
public:
	virtual void run(Thread *th)
	{
		char a = th->pop();
		char b = th->pop();
		th->push(floor(b / a));

		th->move();
	}
};

class EndSR : public Subroutine
{
public:
	virtual void run(Thread *th)
	{
		th->state = ETS_DEAD;
		std::cerr << std::endl << "*** Program Exited ***" << std::endl;
	}
};

class HIfSR : public Subroutine
{
public:
	virtual void run(Thread *th)
	{
		char a = th->pop();
		std::cerr << "If " << (int)a << std::endl;
		if (a == 0)
			th->setDir(RIGHT);
		else
			th->setDir(LEFT);
		th->move();
	}
};

class VIfSR : public Subroutine
{
public:
	virtual void run(Thread *th)
	{
		char a = th->pop();
		std::cerr << "V-If " << (int)a << std::endl;
		if (a == 0)
			th->setDir(DOWN);
		else
			th->setDir(UP);
		th->move();
	}
};

class DupSR : public Subroutine
{
public:
	virtual void run(Thread *th)
	{
		char a = th->pop();
		std::cerr << "Duplicate " << (int)a << std::endl;
		th->push(a);
		th->push(a);
		th->move();
	}
};

class ModSR : public Subroutine
{
public:
	virtual void run(Thread *th)
	{
		char a = th->pop();
		char b = th->pop();
		int ret = b % a;
		if (ret < 0)
			ret = a - ret;
		th->push(ret);
		th->move();
	}
};

class TemplateSR : public Subroutine
{
public:
	virtual void run(Thread *th)
	{
		th->move();
	}
};

int VM::numAliveThreads()
{
	int c = 0;
	for (Thread *th : threads) {
		if (th->state > ETS_DEAD)
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

void VM::assignStandardSR(Thread *th)
{
	assert(th->cursor);
	Cursor *c = th->cursor;
	c->operators['>'] = 1;
	c->operators['v'] = 1;
	c->operators['<'] = 1;
	c->operators['^'] = 1;
	for (int i = 0; i < 10; i++)
		c->operators[i + '0'] = 2;
	c->operators[','] = 3;
	c->operators['.'] = 4;
	c->operators['+'] = 5;
	c->operators['-'] = 6;
	c->operators['*'] = 7;
	c->operators['/'] = 8;
	c->operators['@'] = 9;
	c->operators['_'] = 10;
	c->operators['|'] = 11;
	c->operators[':'] = 12;
	c->operators['%'] = 13;
}

void VM::init(Canvas *canvas)
{
	assert(threads.size() == 0);
	assert(subroutines.size() == 0);

	// Create thread
	Thread *th = new Thread(canvas);
	threads.push_back(th);

	// Create subroutines
	loadSubroutine(new DirSR);
	loadSubroutine(new NumSR);
	loadSubroutine(new PrintCharSR);
	loadSubroutine(new PrintNumSR);
	loadSubroutine(new AddSR);
	loadSubroutine(new TakeSR);
	loadSubroutine(new MultSR);
	loadSubroutine(new DivSR);
	loadSubroutine(new EndSR);
	loadSubroutine(new HIfSR);
	loadSubroutine(new VIfSR);
	loadSubroutine(new DupSR);
	loadSubroutine(new ModSR);
	assignStandardSR(th);
}

void VM::step()
{
	for (Thread *th : threads) {
		th->step(subroutines);
	}
}
