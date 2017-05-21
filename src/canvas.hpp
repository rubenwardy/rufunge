#pragma once
#include <stack>
#include <map>
#include <vector>
#include <cstddef>

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

inline int hashCPos(int cx, int cy)
{
	int retval = (cx + 10 * cy) % CANVAS_HASHMAP_SIZE;
	return (retval < 0) ? -retval : retval;
}

class Canvas
{
public:
	Chunk *hashmap[CANVAS_HASHMAP_SIZE] = {NULL};

	Canvas() {}

	Chunk *getChunk(int x, int y);
	Chunk *getChunkOrCreate(int x, int y);
	void set(int x, int y, char value);
	char get(int x, int y);


	bool readFromFile(const char *filepath);
};
