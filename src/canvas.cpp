#include "canvas.hpp"
#include <iostream>
#include <assert.h>
#include <cmath>
#include <string>
#include <fstream>

Chunk *Canvas::getChunk(int x, int y)
{
	int cx = floor((float)x / CHUNK_SIZE);
	int cy = floor((float)y / CHUNK_SIZE);
	int hash = hashCPos(cx, cy);
	assert(hash >= 0 && hash < CANVAS_HASHMAP_SIZE);

	Chunk *c = hashmap[hash];
	while (c && (c->x != cx || c->y != cy))
		c = c->next;
	return c;
}

Chunk *Canvas::getChunkOrCreate(int x, int y)
{
	int cx = floor((float)x / CHUNK_SIZE);
	int cy = floor((float)y / CHUNK_SIZE);
	int hash = hashCPos(cx, cy);
	assert(hash >= 0 && hash < CANVAS_HASHMAP_SIZE);

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
		for (int i = 0; i < CHUNK_SIZE * CHUNK_SIZE; i++)
			tmp->data[i] = ' ';
		hashmap[hash] = tmp;
		return tmp;
	}
}

void Canvas::set(int x, int y, char value) {
	//printf("get %d %d = %d\n", x, y, value);
	Chunk *c = getChunkOrCreate(x, y);
	assert(c);

	int rx = x - c->x  * CHUNK_SIZE;
	int ry = y - c->y  * CHUNK_SIZE;
	c->data[rx + ry * CHUNK_SIZE] = value;
}

char Canvas::get(int x, int y) {
	Chunk *c = getChunk(x, y);
	if (c) {
		int rx = x - c->x  * CHUNK_SIZE;
		int ry = y - c->y  * CHUNK_SIZE;
		//int retval = c->data[rx + ry * CHUNK_SIZE];
		//printf("get %d %d -> %d\n", x, y, retval);
		return  c->data[rx + ry * CHUNK_SIZE]; //retval;
	} else {
		//printf("get %d %d -> 0\n", x, y);
		return ' ';
	}
}

bool Canvas::readFromFile(const char *filepath)
{
	std::ifstream infile(filepath);
	std::string line;
	int y = 0;
	while (std::getline(infile, line)) {
		int x = 0;
		for (int i = 0; i < line.size(); i++) {
			char ch = line[i];
			if (ch == '\n')
				break;

			if (ch == '\t') {
				std::cerr << "WARNING: Tab character found. Assuming 4 spaces." << std::endl;
				x += 3;
				continue;
			}

			set(x, y, ch);

			x++;
		}
		std::cerr << "Line " << y << " had " << x << " characters." << std::endl;
		y++;
	}

	return true;
}
