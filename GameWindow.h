#ifndef gamewindow_h
#define gamewindow_h

#include "mapArray.h"

enum AnimationType {
	NONE,
	FORWARD,
	BACKWARD,
	DOOR_OPEN,
	DOOR_CLOSE
};

class GameWindow {
private:
	mapArray window;

	int depth;

	void prepareWindow();
	mapArray getPlayerView();

	void drawWall(int texture, int x, int z, bool first);
	int widthInDepth(int z);
	char getSignByPos(double pos, int h, int x);
	char getTexture(int element);

public:
	bool animationsOn;
	int animationStep;
	AnimationType animationType;

	GameWindow();
	void setSize(Vector2D s);
	void drawWindow();
};


#endif