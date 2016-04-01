#include "text.h"
#include "vector2D.h"
#include "player.h"
#include "map.h"
#include "minimap.h"
#include "editor.h"
#include "window.h"

Vector2D Player::pos = Vector2D();
Direction Player::dir = SOUTH;

Text Player::getSymbol() {
	Text s;
	switch (Player::dir) {
	case NORTH:
		s = T("/\\");
		break;
	case SOUTH:
		s = T("\\/");
		break;
	case WEST:
		s = T("<-");
		break;
	case EAST:
		s = T("->");
		break;
	}

	return s;
}

void Player::rotateLeft() {
	int d = (int)dir;
	d--;
	if (d == -1)
		d = 3;
	dir = Direction(d);
	Window::getInstance()->drawMinimap();
}

void Player::rotateRight() {
	int d = (int)dir;
	d++;
	if (d == 4)
		d = 0;
	dir = Direction(d);
	Window::getInstance()->drawMinimap();
}

bool Player::canGoForward() {
	Window* window = Window::getInstance();
	Vector2D oldPos = Player::pos;
	Vector2D newPos = oldPos + Vector2D(dir);
	if (newPos >= V2D(0, 0) && newPos < window->map->map.size) {
		if (window->map->map.map[newPos.y][newPos.x] == '0' || window->map->map.map[newPos.y][newPos.x] == '8' || window->map->map.map[newPos.y][newPos.x] == '6' || window->map->map.map[newPos.y][newPos.x] == '9') {
			return true;
		}
	}
	return false;
}

bool Player::goForward() {
	Window* window = Window::getInstance();
	Vector2D oldPos = Player::pos;
	Vector2D newPos = oldPos + Vector2D(dir);
	if (canGoForward()) {
		Player::pos = newPos;
		window->drawMinimap();
		if (window->map->map.map[newPos.y][newPos.x] == '9') {
			window->printMessage(T("Victory !"));
		}
		return true;
	}
	return false;
}


bool Player::canGoBackward() {
	Window* window = Window::getInstance();
	Vector2D oldPos = Player::pos;
	Vector2D newPos = oldPos - Vector2D(dir);
	if (newPos >= V2D(0, 0) && newPos < window->map->map.size) {
		if (window->map->map.map[newPos.y][newPos.x] == '0' || window->map->map.map[newPos.y][newPos.x] == '8' || window->map->map.map[newPos.y][newPos.x] == '6' || window->map->map.map[newPos.y][newPos.x] == '9') {
			return true;
		}
	}
	return false;
}

bool Player::goBackward() {
	Window* window = Window::getInstance();
	Vector2D oldPos = Player::pos;
	Vector2D newPos = oldPos - Vector2D(dir);
	if (canGoBackward()) {
		Player::pos = newPos;
		window->drawMinimap();
		if (window->map->map.map[newPos.y][newPos.x] == '9') {
			window->printMessage(T("Victory !"));
		}
		return true;
	}
	return false;
}

bool Player::canOpenDoor() {
	Vector2D oldPos = Player::pos;
	Vector2D newPos = oldPos + Vector2D(dir);

	if (Window::getInstance()->map->map.map[newPos.y][newPos.x] == '5')
		return true;

	return false;
}

bool Player::canCloseDoor() {
	Vector2D oldPos = Player::pos;
	Vector2D newPos = oldPos + Vector2D(dir);

	if (Window::getInstance()->map->map.map[newPos.y][newPos.x] == '6')
		return true;

	return false;
}

void Player::openDoor() {
	Vector2D oldPos = Player::pos;
	Vector2D newPos = oldPos + Vector2D(dir);

	if (canOpenDoor()) {
		Window::getInstance()->map->map.map[newPos.y][newPos.x] = '6';
		Window::getInstance()->drawMinimap();
	}
}

void Player::closeDoor() {
	Vector2D oldPos = Player::pos;
	Vector2D newPos = oldPos + Vector2D(dir);

	if (canCloseDoor()) {
		Window::getInstance()->map->map.map[newPos.y][newPos.x] = '5';
		Window::getInstance()->drawMinimap();
	}
}