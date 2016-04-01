#ifndef minimap_h
#define minimap_h

#include "vector2D.h"
#include "mapArray.h"

class Minimap {
public:
	mapArray minimap;

	void setSize(Vector2D size);
	mapArray draw();

	Minimap();
	~Minimap();

private:
	char getTexture(char element);
};

#endif