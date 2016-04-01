#include <stdio.h>
#include <stdlib.h>
#include "mapArray.h"

mapArray::mapArray() {
	size = V2D(0, 0);
	map = NULL;
}

void mapArray::allocateMemory() {
	map = (char**)malloc(sizeof(char*) * size.y);
	for (int i = 0; i < size.y; i++) {
		map[i] = (char*)malloc(sizeof(char) * size.x);
	}
}

void mapArray::fillMap(char element) {
	for (int iy = 0; iy < size.y; iy++)
		for (int ix = 0; ix < size.x; ix++)
			map[iy][ix] = element;
}

void mapArray::freeMemory() {
	if (map != NULL) {
		for (int i = 0; i < size.y; i++)
			if(map[i] != NULL)
				free(map[i]);

		free(map);
	}
}