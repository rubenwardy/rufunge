#include "cursor.hpp"

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
