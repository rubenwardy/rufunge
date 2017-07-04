#include "thread.hpp"

Thread::Thread(std::shared_ptr<Canvas> c)
{
	std::cerr << "Created new thread!" << std::endl;
	cursor = new Cursor();
	cursor->canvas = c;
}

Thread::Thread(const Thread &other)
{
	cursor = new Cursor(*other.cursor);

	std::stack<Cursor*> in = other.link;
	std::stack<Cursor*> tmp;
	while (!in.empty()) {
		tmp.push(in.top());
		in.pop();
	}

	while (!tmp.empty()) {
		link.push(new Cursor(*tmp.top()));
		tmp.pop();
	}
}

Thread::~Thread()
{
	std::cerr << "Deconstructing thread!" << std::endl;

	state = ETS_DEAD;
	if (cursor) {
		delete cursor;
	}

	while (!link.empty()) {
		Cursor *c = link.top();
		delete c;
		link.pop();
	}
}

bool Thread::popCursor() {
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

void Thread::pushCursor(Cursor *nc) {
	if (cursor) {
		link.push(cursor);
	}
	cursor = nc;
	state = ETS_READY;
	std::cerr << "Pushed cursor!" << std::endl;
}

char Thread::pop() {
	if (stack.empty()) {
		return 0;
	} else {
		char retval = 0;
		retval = stack.top();
		stack.pop();
		return retval;
	}
}

void Thread::step(VM *vm, std::map<int, Subroutine*> &srman)
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
					it->second->run(vm, this);
				} else {
					state = ETS_DEAD;
					std::cerr << "ERROR: Unable to find operator in memory: " << ch << std::endl;
				}
			} else if (ch == ' ') {
				//std::cerr << "Move" << std::endl;
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
