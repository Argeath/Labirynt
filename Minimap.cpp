#include <stdio.h>
#include <stdlib.h>
#include "map.h"
#include "minimap.h"
#include "editor.h"
#include "window.h"
#include "mapArray.h"
#include "player.h"
#include "Vector2D.h"

Minimap::Minimap() {
	mapArray minimap = mapArray();
}

Minimap::~Minimap() {
	minimap.freeMemory();
}

void Minimap::setSize(Vector2D size) {
	Window* window = Window::getInstance();
	if (minimap.size != size) {
		minimap.freeMemory();
		minimap.size = size;
		if (minimap.size.x > window->map->map.size.x)
			minimap.size.x = window->map->map.size.x;
		if (minimap.size.y > window->map->map.size.y)
			minimap.size.y = window->map->map.size.y;

		minimap.allocateMemory();
	}
}

mapArray Minimap::draw() {
	Window* window = Window::getInstance();
	Vector2D playerTmp = Player::pos;
	Vector2D tmp = minimap.size;
	Vector2D poczatek = playerTmp - (tmp / 2);

	if (poczatek.x < 0)
		poczatek.x = 0;
	if (poczatek.y < 0)
		poczatek.y = 0;

	if (window->map->map.size.x >= minimap.size.x && poczatek.x > window->map->map.size.x - minimap.size.x)
		poczatek.x = window->map->map.size.x - minimap.size.x;
	if (window->map->map.size.y >= minimap.size.y && poczatek.y > window->map->map.size.y - minimap.size.y)
		poczatek.y = window->map->map.size.y - minimap.size.y;

	for (int i = 0; i < minimap.size.y; i++) {
		for (int j = 0; j < minimap.size.x; j++) {
			if (V2D(poczatek.x + j, poczatek.y + i) >= V2D(0, 0) && V2D(poczatek.x + j, poczatek.y + i) < window->map->map.size) {
				minimap.map[i][j] = getTexture(window->map->map.map[poczatek.y + i][poczatek.x + j]);

				if (Player::pos == V2D(poczatek.x + j, poczatek.y + i)) {
					minimap.map[i][j] = 'P';
				}
			}
			else {
				minimap.map[i][j] = ' ';
			}
		}
	}

	return minimap;
}

char Minimap::getTexture(char element) {
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
		return 178;
	case '6':
		return 176;
	default:
		return '#';
	}
}