#include <iostream>
#include <assert.h>
#include "rufunge.hpp"

#define TEST(cond) if (!(cond)) { std::cerr << "Failed test, line " << __LINE__ << ": " << #cond << std::endl; return false; }
inline bool TEST_POS_SET(Canvas &c, int x, int y, char b, char a)
{
	TEST(c.get((x), (y)) == (b));
	c.set((x), (y), (a));
	TEST(c.get((x),(y))  == (a));

	return true;
}

#define TEST_POS(ca, x, y, ch) TEST((ca).get((x), (y)) == (ch))

bool run_tests()
{
	Canvas c;
	for (int x = -100; x < 100; x++) {
		for (int y = -100; y < 100; y++) {
			if (!TEST_POS_SET(c, x, y, ' ', (x + y * 200) % 255)) {
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

	Canvas d;
	TEST(d.readFromFile("examples/hello_world.rf"));

	std::string line = ">              v";
	int y = 0;
	for (int x = 0; x < line.size(); x++)
		TEST_POS(d, x, y, line[x]);

	line = "v  ,,,,,\"Hello\"<"; y++;
	for (int x = 0; x < line.size(); x++)
		TEST_POS(d, x, y, line[x]);

	line = ">48*,          v"; y++;
	for (int x = 0; x < line.size(); x++)
		TEST_POS(d, x, y, line[x]);

	line = "v,,,,,,\"World!\"<"; y++;
	for (int x = 0; x < line.size(); x++)
		TEST_POS(d, x, y, line[x]);

	line = ">25*,@            "; y++;
	for (int x = 0; x < line.size(); x++)
		TEST_POS(d, x, y, line[x]);

	return true;
}

int main()
{
	if (!run_tests()) {
		std::cerr << "A test failed!" << std::endl;
		return 1;
	}

	std::cerr << "All tests passed!" << std::endl;

	return 0;
}
