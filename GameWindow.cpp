#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "conio2.h"
#include "Vector2D.h"
#include "player.h"
#include "map.h"
#include "minimap.h"
#include "editor.h"
#include "window.h"
#include "text.h"
#include "GameWindow.h"

GameWindow::GameWindow() : animationStep(0), depth(7) {
	window.size = V2D(0, 0);
	animationsOn = false;
	animationType = NONE;
}

void GameWindow::setSize(Vector2D s) {
	if (s >= V2D(0, 0)) {
		window.size = s;
		window.allocateMemory();
	}
}

mapArray GameWindow::getPlayerView() {
	Vector2D playerPos = Player::pos;
	Vector2D dir = Vector2D(Player::dir);
	Vector2D dirReversed = dir.reversed();
	Vector2D rozmiar = V2D(3, 0);
	Vector2D nextPos;
	Window* window = Window::getInstance();

	for (int i = 1; i <= depth; i++) {
		nextPos = V2D(playerPos.x + dir.x * i, playerPos.y + dir.y * i);
		if (nextPos.x >= window->map->map.size.x || nextPos.y >= window->map->map.size.y || nextPos.x < 0 || nextPos.y < 0)
			break;

		rozmiar.y++;

		if (window->map->map.map[nextPos.y][nextPos.x] != '0' && window->map->map.map[nextPos.y][nextPos.x] != '5' && window->map->map.map[nextPos.y][nextPos.x] != '6')
			break;
	}

	mapArray view = mapArray();
	view.size = V2D(rozmiar.x + 1, rozmiar.y);
	view.allocateMemory();

	for (int i = 1; i <= rozmiar.y; i++) {
		nextPos = V2D(playerPos.x + dir.x * i, playerPos.y + dir.y * i);
		if (nextPos.y + dirReversed.y >= 0 && nextPos.x - dirReversed.x >= 0 && nextPos.y + dirReversed.y < window->map->map.size.y && nextPos.x - dirReversed.x < window->map->map.size.x)
			view.map[i - 1][0] = window->map->map.map[nextPos.y + dirReversed.y][nextPos.x - dirReversed.x];
		view.map[i - 1][1] = window->map->map.map[nextPos.y][nextPos.x];
		if(nextPos.y - dirReversed.y >= 0 && nextPos.x + dirReversed.x >= 0 && nextPos.y - dirReversed.y < window->map->map.size.y && nextPos.x + dirReversed.x < window->map->map.size.x)
			view.map[i - 1][2] = window->map->map.map[nextPos.y - dirReversed.y][nextPos.x + dirReversed.x];

	}

	return view;
}

void GameWindow::prepareWindow() {
	for (int i = 0; i < window.size.y; i++) {
		for (int j = 0; j < window.size.x; j++) {
			window.map[i][j] = ' ';
		}
	}
	for (int i = 0; i < 7; i++) {
		gotoxy(77, 17 + i);
		putch(' ');
		putch(' ');
		putch(' ');
	}

	mapArray view = getPlayerView();
	/*for (int i = 0; i < view.size.y; i++) {
		gotoxy(77, 17 + i);
		putch(view.map[i][0]);
		putch(view.map[i][1]);
		putch(view.map[i][2]);
	}*/
	for (int i = view.size.y - 1; i >= 0; i--) {
		drawWall(view.map[i][0], 1, i, (i == view.size.y - 1));
		drawWall(view.map[i][2], -1, i, (i == view.size.y - 1));
		drawWall(view.map[i][1], 0, i, (i == view.size.y - 1));
	}
	view.freeMemory();
}

void GameWindow::drawWindow() {
	Window* win = Window::getInstance();
	if (win->map->loaded) {
		if (animationStep <= 0) animationStep = 1;

		while (animationStep-- > 0) {
			prepareWindow();

			for (int y = 0; y < window.size.y; y++) {
				gotoxy(2, 2 + y);
				for (int x = 0; x < window.size.x; x++) {
					putch(window.map[y][x]);
				}
			}

			if(animationStep > 0)
				_sleep(100);
		}

		if (animationType == FORWARD) {
			Player::goForward();
		}
		else if (animationType == DOOR_OPEN) {
			Player::openDoor();
		}
		else if (animationType == DOOR_CLOSE) {
			Player::closeDoor();
		}

		if (animationType != NONE) 
			animationType = NONE;
	}
}

void GameWindow::drawWall(int texture, int x, int z, bool first) {
	if (texture == '0' || texture == '8' || texture == '9')
		return;

	double tangens = (double)window.size.y / window.size.x;
	int width = widthInDepth(z);

	int posX = 0;
	for (int i = 0; i < z; i++) posX += widthInDepth(i);

	double fromTop = posX * tangens;
	int fromTopInt = ceil(fromTop);
	double signPos = fromTopInt - fromTop;

	if (fromTopInt >= 11)
		fromTopInt = 10;


	bool isDoor = (texture == '5' || texture == '6');
	bool isOpenedDoor = (texture == '6');

	if (x == 0) {
		int wallSize = window.size.x - 2 * posX;

		int doorAnimationStep = 0;
		if (animationType == DOOR_CLOSE || animationType == DOOR_OPEN)
			doorAnimationStep = animationStep;

		int doorPosLeft = wallSize/2 * doorAnimationStep * 0.2 + posX + 2;
		if (animationType == DOOR_CLOSE)
			doorPosLeft = wallSize/2 * (5 - doorAnimationStep) * 0.2 + posX + 2;

		int doorPosRight = window.size.x - posX - wallSize/2 * doorAnimationStep * 0.2 - 2;
		if (animationType == DOOR_CLOSE)
			doorPosRight = window.size.x - posX - wallSize/2 * (5 - doorAnimationStep) * 0.2 - 2;

		for (int iy = fromTopInt + 1; iy < window.size.y - fromTopInt - 2; iy++) {
			for (int ix = posX; ix < window.size.x - posX; ix++) {
				if ((isDoor && ((animationType == DOOR_CLOSE || animationType == DOOR_OPEN) || isOpenedDoor)
					&& (ix == doorPosLeft || ix == doorPosRight - 1)))
						window.map[iy][ix] = '|';

				if (isDoor && (animationType == DOOR_OPEN || animationType == DOOR_CLOSE || isOpenedDoor) && ix >= doorPosLeft && ix < doorPosRight)
						continue;

				if(ix == posX || ix == window.size.x - posX - 1)
					window.map[iy][ix] = '|';
				else
					window.map[iy][ix] = getTexture(texture);
			}
		}

		for (int ix = posX; ix < window.size.x - posX; ix++) {
			if (isDoor && (animationType == DOOR_OPEN || animationType == DOOR_CLOSE || isOpenedDoor) && ix >= doorPosLeft && ix < doorPosRight)
				continue;

			window.map[fromTopInt][ix] = '_';
			window.map[window.size.y - fromTopInt - 2][ix] = '^';
		}

	}
	else if (x == -1 && first) {
		if (isOpenedDoor)
			return;

		for (int iy = fromTopInt; iy < window.size.y - fromTopInt - 2; iy++)
			for (int ix = 0; ix < posX; ix++)
				window.map[iy][ix] = getTexture(texture);

		for (int i = 0; i < posX; i++)
			window.map[fromTopInt][i] = '_';

		for (int i = 0; i < posX; i++)
			window.map[window.size.y - fromTopInt - 2][i] = '^';
	}
	else if (x == 1 && first) {
		if (isOpenedDoor)
			return;

		for (int iy = fromTopInt; iy < window.size.y - fromTopInt - 2; iy++)
			for (int ix = window.size.x - posX; ix < window.size.x; ix++)
				window.map[iy][ix] = getTexture(texture);

		for (int i = window.size.x - posX; i < window.size.x; i++)
			window.map[fromTopInt][i] = '_';

		for (int i = window.size.x - posX; i < window.size.x; i++)
			window.map[window.size.y - fromTopInt - 2][i] = '^';
	} else if (x == -1) {
		if (isOpenedDoor)
			return;

		for (int i = 0; i < width; i++) {
			double fromTop = (posX + i) * tangens;
			int fromTopInt = ceil(fromTop);
			if (fromTopInt >= 11)
				continue;
			double signPos = fromTopInt - fromTop;
			char sign = getSignByPos(signPos, 1, x);

			window.map[fromTopInt][posX + i] = sign;

			for (int iy = fromTopInt + 1; iy < window.size.y - fromTopInt - 2; iy++) {
				if(i == width - 1)
					window.map[iy][posX + i] = '|';
				else
					window.map[iy][posX + i] = getTexture(texture);
			}

			sign = getSignByPos(signPos, -1, x);
			window.map[window.size.y - fromTopInt - 2][posX + i] = sign;

			if (i == 0 && z > 0) {
				for (int iy = fromTopInt; iy < window.size.y - fromTopInt - 1; iy++) {
					for (int ix = 0; ix < posX - 1; ix++) {
						window.map[iy][ix] = getTexture(texture);
					}
				}

				if (fromTopInt > 0) {
					for (int i = 0; i < posX - 1; i++) {
						window.map[fromTopInt - 1][i] = '_';
					}
				}
				for (int i = 0; i < posX - 1; i++) {
					window.map[window.size.y - fromTopInt - 1][i] = '^';
				}

				if (posX > 1) {
					for (int iy = fromTopInt; iy < window.size.y - fromTopInt - 1; iy++) {
						window.map[iy][posX - 1] = '|';
					}
				}
			}
		}
	} else if (x == 1) {
		if (isOpenedDoor)
			return;

		for (int i = 0; i < width; i++) {
			double fromTop = (posX + i) * tangens;
			int fromTopInt = ceil(fromTop);
			if (fromTopInt >= 11)
				continue;
			double signPos = fromTopInt - fromTop;
			char sign = getSignByPos(signPos, 1, x);

			window.map[fromTopInt][window.size.x - (posX + i) - 1] = sign;

			for (int iy = fromTopInt + 1; iy < window.size.y - fromTopInt - 2; iy++) {
				if (i == width - 1)
					window.map[iy][window.size.x - (posX + i) - 1] = '|';
				else
					window.map[iy][window.size.x - (posX + i) - 1] = getTexture(texture);
			}

			sign = getSignByPos(signPos, -1, x);
			window.map[window.size.y - fromTopInt - 2][window.size.x - (posX + i) - 1] = sign;

			if (i == 0 && z > 0) {
				for (int iy = fromTopInt; iy < window.size.y - fromTopInt - 1; iy++) {
					for (int ix = 0; ix < posX - 1; ix++) {
						window.map[iy][window.size.x - ix - 1] = getTexture(texture);
					}
				}

				if (fromTopInt > 0) {
					for (int i = 0; i < posX - 1; i++) {
						window.map[fromTopInt - 1][window.size.x - i - 1] = '_';
					}
				}
				for (int i = 0; i < posX - 1; i++) {
					window.map[window.size.y - fromTopInt - 1][window.size.x - i - 1] = '^';
				}

				if (posX > 1) {
					for (int iy = fromTopInt; iy < window.size.y - fromTopInt - 1; iy++) {
						window.map[iy][window.size.x - (posX - 1) - 1] = '|';
					}
				}
			}
		}
	}
}

int GameWindow::widthInDepth(int z) {
	switch (z) {
	case 0:
		// anim  wynik
		//  0      6
		//  6      0
		if (animationType == BACKWARD)
			return 6 - animationStep;
		else if (animationType == FORWARD)
			return 6 + (animationStep - 6);
		else
			return 6;
	case 1:
	case 2:
	case 3:
		if (animationType == BACKWARD)
			return 4 + animationStep / 3;
		else if (animationType == FORWARD)
			return 4 - (animationStep - 6) / 3;
		else
			return 4;
	case 4:
	case 5:
		if (animationType == BACKWARD)
			return 3 + animationStep / 6;
		else if (animationType == FORWARD)
			return 3 - (animationStep - 6) / 6;
		else
			return 3;
	default:
		return 2;
	}
}

char GameWindow::getSignByPos(double pos, int h, int x) {
	if (h == 1) {
		if (x == -1) {
			if (pos < 0.5)
				return '_';
			else
				return '\\';
		}
		else {
			if (pos < 0.5)
				return '_';
			else
				return '/';
		}
	}
	else {
		if (x == -1) {
			if (pos < 0.5)
				return ' ';
			else
				return '/';
		}
		else {
			if (pos < 0.5)
				return ' ';
			else
				return '\\';
		}
	}
}

char GameWindow::getTexture(int element) {
	switch (element) {
	case '0':
	case '8':
	case '9':
		return ' ';
	case '2':
		return '@';
	case '3':
		return '$';
	case '4':
		return '&';
	case '5':
	case '6':
		return 178;
	case '1':
		return '#';
	default:
		return 'E';
	}
}