#ifndef maparray_h
#define maparray_h

#include "Vector2D.h"

class mapArray {
public:
	char** map;
	Vector2D size;

	mapArray();

	void allocateMemory();
	void fillMap(char element);
	void freeMemory();
};

#endif