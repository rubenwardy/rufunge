#include "canvas.hpp"

Chunk *Canvas::getChunk(int x, int y)
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

Chunk *Canvas::getChunkOrCreate(int x, int y)
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
		return 0;
	}
}
