#include <stdio.h>
#include <stdlib.h>
#include "text.h"
#include "map.h"
#include "minimap.h"
#include "editor.h"
#include "window.h"
#include "conio2.h"
#include "player.h"
#include "vector2D.h"
#include "mapArray.h"

Map::Map() {
	loaded = false;
	filename = "";
	map = mapArray();
}

bool Map::loadMap(char* path) {
	Window* window = Window::getInstance();
	FILE* file;
	long lSize;
	char* buffer;
	size_t result;

	filename = path;

	file = fopen(T("maps/") + path + ".lmap", "rb");
	if (file == NULL) {
		window->printMessage(T("Map file not found. (") + path + ")");
		loaded = false;
		return false;
	}

	fseek(file, 0, SEEK_END);
	lSize = ftell(file);
	rewind(file);

	buffer = (char*)malloc(sizeof(char) * lSize);


	if (buffer == NULL) {
		window->printMessage(T("Map file is empty (") + path + ")");
		loaded = false;
		return false;
	}

	result = fread(buffer, 1, lSize, file);
	if (result != lSize) {
		window->printMessage(T("Wrong map file size (") + path + ")");
		loaded = false;
		return false;
	}

	map.freeMemory();
	int x = 0;
	int y = 0;
	short windows = 0;

	for (int i = 0; buffer[i] != '\0'; i++) {
		if (buffer[i] == '\r') {
			windows = 1;
			continue;
		}
		if (buffer[i] == '\n') {
			map.size.x = x;
			x = 0;
			y++;
			continue;
		}
		x++;
	}
	map.size.y = y + 1;

	map.allocateMemory();

	for (int i = 0; i < map.size.y; i++) {
		for (int j = 0; j < map.size.x; j++) {
			map.map[i][j] = buffer[i * (map.size.x + 1 + windows) + j];
		}
	}

	movePlayerToStart();


	fclose(file);
	free(buffer);

	loaded = true;

	window->printSteps(0);
	window->printMapInfo(T("Map: ") + path);
	window->resetTime();
	window->printTime();
	window->drawMinimap();

	window->editor->tMap.fillMap('A');

	return true;
}

void Map::movePlayerToStart() {
	for (int i = 0; i < map.size.y; i++) {
		for (int j = 0; j < map.size.x; j++) {
			if (map.map[i][j] == '8') {
				Player::pos = V2D(j, i);
				Player::dir = SOUTH;
			}
		}
	}
}

void Map::saveMap(char* path) {
	Window* window = Window::getInstance();
	FILE* file;
	char* buffer;

	file = fopen(T("maps/") + path + ".lmap", "wb");
	if (file == NULL) {
		window->printMessage(T("File could not be saved or created. (") + path + ")");
	}

	long size = map.size.y * (map.size.x + 1) - 1;
	buffer = (char*)malloc(sizeof(char) * size);
	int ib = 0;
	for (int iy = 0; iy < map.size.y; iy++) {
		for (int ix = 0; ix < map.size.x; ix++) {
			buffer[ib++] = map.map[iy][ix];
		}
		if (iy < map.size.y - 1)
			buffer[ib++] = '\n';
	}

	fwrite(buffer, sizeof(char), size, file);

	fclose(file);
}