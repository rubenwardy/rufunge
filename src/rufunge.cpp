#include "rufunge.hpp"
#include <cmath>
#include <iostream>
#include <assert.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

class DirSR : public Subroutine
{
public:
	virtual void run(VM *vm, Thread *th)
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
	virtual void run(VM *vm, Thread *th)
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
	virtual void run(VM *vm, Thread *th)
	{
		char ch = th->pop();
		std::cout << ch;
		th->move();
	}
};

class PrintNumSR : public Subroutine
{
public:
	virtual void run(VM *vm, Thread *th)
	{
		char ch = th->pop();
		std::cout << (int)ch;
		th->move();
	}
};

class AddSR : public Subroutine
{
public:
	virtual void run(VM *vm, Thread *th)
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
	virtual void run(VM *vm, Thread *th)
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
	virtual void run(VM *vm, Thread *th)
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
	virtual void run(VM *vm, Thread *th)
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
	virtual void run(VM *vm, Thread *th)
	{
		th->state = ETS_DEAD;
		std::cerr << std::endl << "*** Program Exited ***" << std::endl;
	}
};

class HIfSR : public Subroutine
{
public:
	virtual void run(VM *vm, Thread *th)
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
	virtual void run(VM *vm, Thread *th)
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
	virtual void run(VM *vm, Thread *th)
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
	virtual void run(VM *vm, Thread *th)
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

class NotSR : public Subroutine
{
public:
	virtual void run(VM *vm, Thread *th)
	{
		char a = th->pop();
		std::cerr << "Not " << (int)a << std::endl;
		th->push((a == 0) ? 1 : 0);
		th->move();
	}
};

class GtSR : public Subroutine
{
public:
	virtual void run(VM *vm, Thread *th)
	{
		char a = th->pop();
		char b = th->pop();
		if (b > a)
			th->push(1);
		else
			th->push(0);

		th->move();
	}
};

class RandSR : public Subroutine
{
public:
	virtual void run(VM *vm, Thread *th)
	{
		EDIR dir = (EDIR)(rand() % 4);
		th->setDir(dir);
		th->move();
	}
};

class DiscardSR : public Subroutine
{
public:
	virtual void run(VM *vm, Thread *th)
	{
		th->pop();
		th->move();
	}
};

class SwapSR : public Subroutine
{
public:
	virtual void run(VM *vm, Thread *th)
	{
		char a = th->pop();
		char b = th->pop();
		th->push(a);
		th->push(b);

		th->move();
	}
};

class GetSR : public Subroutine
{
public:
	virtual void run(VM *vm, Thread *th)
	{
		char y = th->pop();
		char x = th->pop();
		Canvas *c = th->cursor->canvas;
		char v = c->get(x, y);
		th->push(v);

		std::cerr << "Got " << v << " from " << (int)x << ", " << (int)y << std::endl;

		th->move();
	}
};

class PutSR : public Subroutine
{
public:
	virtual void run(VM *vm, Thread *th)
	{
		char y = th->pop();
		char x = th->pop();
		char v = th->pop();
		Canvas *c = th->cursor->canvas;
		c->set(x, y, v);
		std::cerr << "Put " << v << " to " << (int)x << ", " << (int)y << std::endl;

		th->move();
	}
};

class JumpSR : public Subroutine
{
public:
	virtual void run(VM *vm, Thread *th)
	{
		th->move();
		th->move();
	}
};

class BridgeSR : public Subroutine
{
public:
	virtual void run(VM *vm, Thread *th)
	{
		do {
			th->move();
		} while(th->getChar() != ']');

		th->move();
	}
};

class RtnSR : public Subroutine
{
public:
	virtual void run(VM *vm, Thread *th)
	{
		th->popCursor();
	}
};

class StartRufungeSR : public Subroutine
{
public:
	Canvas *canvas = new Canvas();
	std::string module;
	std::string file;

	~StartRufungeSR()
	{
		std::cerr << "Deconstructing StartRufungeSR" << std::endl;
		delete canvas;
		canvas = NULL;
	}

	virtual void run(VM *vm, Thread *th)
	{
		std::string filepath = std::string("examples/") + file + ".rf";
		std::cerr << "Entering " << filepath << std::endl;

		th->move();

		Cursor *cursor = new Cursor();
		cursor->canvas = canvas;
		th->pushCursor(cursor);
		vm->assignStandardSR(th);
	}
};

class LoadSR : public Subroutine
{
public:
	virtual void run(VM *vm, Thread *th)
	{
		// Get operator
		char op = th->pop();

		// Get module
		std::string module;
		char c = th->pop();
		while (c != 0) {
			module += c;
			c = th->pop();
		}

		// Get file
		std::string file;
		c = th->pop();
		while (c != 0) {
			file += c;
			c = th->pop();
		}

		// Load canvas
		Canvas *d = new Canvas();
		std::string filepath = std::string("examples/") + file + ".rf";
		if (d->readFromFile(filepath.c_str())) {
			std::cerr << "Loaded " << filepath << std::endl;
		} else {
			std::cerr << "Error! Unable to load SR "
				<< module << "::" << file << std::endl;
			th->state = ETS_DEAD;
			return;
		}

		// Load as operator
		StartRufungeSR *sr = new StartRufungeSR;
		sr->module = module;
		sr->file = file;
		sr->canvas = d;
		th->cursor->operators[op] = vm->loadSubroutine(sr);
		std::cerr << "Loaded rufunge operator " << op << " to be "
			<< module << "::" << file << std::endl;

		th->move();
	}
};


class TemplateSR : public Subroutine
{
public:
	virtual void run(VM *vm, Thread *th)
	{
		th->move();
	}
};

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
	c->operators['!'] = 14;
	c->operators['`'] = 15;
	c->operators['?'] = 16;
	c->operators['$'] = 17;
	c->operators['\\'] = 18;
	c->operators['g'] = 19;
	c->operators['p'] = 20;
	c->operators['#'] = 21;
	c->operators['['] = 22;
	c->operators['R'] = 23;
	c->operators['P'] = 24;

	// TODO: & ~ M L
}

void VM::init(Canvas *canvas)
{
	srand (time(NULL));

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
	loadSubroutine(new NotSR);
	loadSubroutine(new GtSR);
	loadSubroutine(new RandSR);
	loadSubroutine(new DiscardSR);
	loadSubroutine(new SwapSR);
	loadSubroutine(new GetSR);
	loadSubroutine(new PutSR);
	loadSubroutine(new JumpSR);
	loadSubroutine(new BridgeSR);
	loadSubroutine(new RtnSR);
	loadSubroutine(new LoadSR);
	assignStandardSR(th);
}

void VM::step()
{
	for (Thread *th : threads) {
		th->step(this, subroutines);
	}
}
