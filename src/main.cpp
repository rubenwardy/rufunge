#include "rufunge.hpp"

#define TEST(cond) if (!(cond)) { std::cerr << "Failed test, line " << __LINE__ << ": " << #cond << std::endl; return false; }
bool TEST_POS(Canvas &c, int x, int y, char b, char a)
{
	TEST(c.get((x), (y)) == (b));
	c.set((x), (y), (a));
	TEST(c.get((x),(y))  == (a));

	return true;
}
bool run_tests() {
	Canvas c;
	for (int x = -100; x < 100; x++) {
		for (int y = -100; y < 100; y++) {
			if (!TEST_POS(c, x, y, 0, (x + y * 200) % 255)) {
				std::cerr << " - was " << x << ", " << y << std::endl;
				return false;
			}
		}
	}

	TEST(dirRight(RIGHT) == DOWN);
	TEST(dirRight(DOWN)  == LEFT);
	TEST(dirRight(LEFT)  == UP);
	TEST(dirRight(UP)    == RIGHT);

	TEST(dirLeft(RIGHT)  == UP);
	TEST(dirLeft(UP)     == LEFT);
	TEST(dirLeft(DOWN)   == RIGHT);
	TEST(dirLeft(LEFT)   == DOWN);

	return true;
}

int main() {
	if (run_tests()) {
		std::cerr << "All tests passed!" << std::endl;
	} else {
		std::cerr << "A test failed!" << std::endl;
		return 1;
	}

	std::cerr << "Loaded!" << std::endl;
}
