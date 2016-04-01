#ifndef player_h
#define player_h

#include "vector2D.h"
#include "text.h"

class Player {
public:
	static Vector2D pos;
	static Direction dir;

	static Text getSymbol();
	static void rotateLeft();
	static void rotateRight();
	static bool canGoForward();
	static bool goForward();
	static bool canGoBackward();
	static bool goBackward();
	static bool canOpenDoor();
	static void openDoor();
	static bool canCloseDoor();
	static void closeDoor();
};

#endif