#ifndef editor_h
#define editor_h

#include "mapArray.h"
#include "Vector2D.h"

class Editor {
public:
	mapArray tMap;
	Vector2D cursor;

	Editor();
	~Editor();

	void moveCursor(Direction dir);
	void placeObject(char obj);
	Vector2D getMapSize();
	void cloneMap();
	void createNewMap();
	mapArray getMapToDraw();
	bool prepareMapToSave();

private:
	char getTexture(int element);
};


#endif