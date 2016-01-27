#pragma once
#include <iostream>
#include <assert.h>
#include <cmath>
#include <stack>
#include <map>
#include <vector>

#define CHUNK_SIZE 16

class Chunk
{
public:
	int x;
	int y;

	char data[CHUNK_SIZE * CHUNK_SIZE];
	Chunk *next;
};

#define CANVAS_HASHMAP_SIZE 1000

class Canvas
{
public:
	Chunk* hashmap[CANVAS_HASHMAP_SIZE] = {NULL};

	Canvas() {}

	inline int hashCPos(int cx, int cy)
	{
		int retval = (cx + 10 * cy) % CANVAS_HASHMAP_SIZE;
		return (retval < 0) ? -retval : retval;
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
