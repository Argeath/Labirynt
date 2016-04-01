#ifndef map_h
#define map_h

#include "Vector2D.h"
#include "mapArray.h"

class Map {
public:
	mapArray map;
	char* filename;
	bool loaded;

	Map();

	bool loadMap(char* path);
	void saveMap(char* path);
	void movePlayerToStart();
};

#endif