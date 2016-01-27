#pragma once
#include <iostream>
#include <assert.h>
#include <cmath>
#include <stack>
#include <map>
#include <vector>

enum EDIR {
	RIGHT,
	DOWN,
	LEFT,
	UP
};

#define CHUNK_SIZE 16

class Chunk
{
public:
	int x;
	int y;

	char data[CHUNK_SIZE * CHUNK_SIZE];
	Chunk *next;
};

class Canvas
{
public:
	Chunk* hashmap[1000] = {NULL};

	Canvas() {}

	int hashCPos(int cx, int cy)
	{
		int retval = (cx + 10 * cy) % 1000;
		if (retval < 0) {
			return -retval;
		} else {
			return retval;
		}
	}

	Chunk *getChunk(int x, int y)
	{
		int cx = floor((float)x / CHUNK_SIZE);
		int cy = floor((float)y / CHUNK_SIZE);
		int hash = hashCPos(cx, cy);
		assert(hash >= 0 && hash < 1000);

		Chunk *c = hashmap[hash];
		while (c && (c->x != cx || c->y != cy))
			c = c->next;
		return c;
	}

	Chunk *getChunkOrCreate(int x, int y)
	{
		int cx = floor((float)x / CHUNK_SIZE);
		int cy = floor((float)y / CHUNK_SIZE);
		int hash = hashCPos(cx, cy);
		assert(hash >= 0 && hash < 1000);

		Chunk *c = hashmap[hash];
		while (c && (c->x != cx || c->y != cy))
			c = c->next;

		if (c) {
			return c;
		} else {
			Chunk *tmp = new Chunk();
			tmp->x = cx;
			tmp->y = cy;
			tmp->next = hashmap[hash];
			hashmap[hash] = tmp;
			return tmp;
		}
	}

	void set(int x, int y, char value) {
		//printf("get %d %d = %d\n", x, y, value);
		Chunk *c = getChunkOrCreate(x, y);
		assert(c);

		int rx = x - c->x  * CHUNK_SIZE;
		int ry = y - c->y  * CHUNK_SIZE;
		c->data[rx + ry * CHUNK_SIZE] = value;
	}

	char get(int x, int y) {
		Chunk *c = getChunk(x, y);
		if (c) {
			int rx = x - c->x  * CHUNK_SIZE;
			int ry = y - c->y  * CHUNK_SIZE;
			//int retval = c->data[rx + ry * CHUNK_SIZE];
			//printf("get %d %d -> %d\n", x, y, retval);
			return  c->data[rx + ry * CHUNK_SIZE]; //retval;
		} else {
			//printf("get %d %d -> 0\n", x, y);
			return 0;
		}
	}
};

struct Cursor
{
	Canvas *canvas;
	int x, y;
	EDIR dir;
	std::map<char, int> operators;
};

class Thread
{
	Cursor *cursor;
	std::stack<Cursor*> link;
	std::stack<char> stack;
};

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
};
