#include "map.h"
#include "Minimap.h"
#include "editor.h"
#include "window.h"

#define EMPTY 176

Editor::Editor() {
	tMap.size = V2D(52, 23);
	tMap.allocateMemory();
	tMap.fillMap('A');

	cursor = V2D(0, 0);
}

Editor::~Editor() {
	tMap.freeMemory();
}

void Editor::moveCursor(Direction dir) {
	Vector2D vDir = Vector2D(dir);
	cursor + vDir;
	if (cursor.x < 0)
		cursor.x = 0;
	else if (cursor.x > tMap.size.x - 1)
		cursor.x = tMap.size.x - 1;
	
	if (cursor.y < 0)
		cursor.y = 0;
	else if (cursor.y > tMap.size.y - 1)
		cursor.y = tMap.size.y - 1;

	Window::getInstance()->moveCursor(cursor.x + 2, cursor.y + 2);
}

void Editor::placeObject(char obj) {
	if (obj != 'A' && (obj < '0' || obj > '9' || obj == '7'))
		return;

	if (obj == '6') obj = '5';

	tMap.map[cursor.y][cursor.x] = obj;
}

Vector2D Editor::getMapSize() {
	Vector2D rozmiar = V2D(0, 0);
	for (int iy = 0; iy < tMap.size.y; iy++) {
		int tmpRozmiar = 0;
		for (int ix = 0; ix < tMap.size.x; ix++) {
			if (tMap.map[iy][ix] != 'A') {
				tmpRozmiar++;

				if(ix > rozmiar.x)
					rozmiar.x = ix;
			}
			if (tmpRozmiar > 0 && iy > rozmiar.y)
				rozmiar.y = iy;
		}
	}

	rozmiar + V2D(1, 1);
	return rozmiar;
}


void Editor::cloneMap() {
	mapArray m = Window::getInstance()->map->map;
	for (int iy = 0; iy < m.size.y; iy++) {
		for (int ix = 0; ix < m.size.x; ix++) {
			tMap.map[iy][ix] = m.map[iy][ix];
		}
	}
}

mapArray Editor::getMapToDraw() {
	mapArray m = mapArray();
	m.size = tMap.size;
	m.allocateMemory();
	
	for (int iy = 0; iy < m.size.y; iy++) {
		for (int ix = 0; ix < m.size.x; ix++) {
			m.map[iy][ix] = getTexture(tMap.map[iy][ix]);
		}
	}

	return m;
}

bool Editor::prepareMapToSave() {
	Vector2D s = getMapSize();
	bool isStart = false;
	bool isMeta = false;

	mapArray newMap = mapArray();
	newMap.size = s;
	newMap.allocateMemory();

	for (int iy = 0; iy < s.y; iy++) {
		for (int ix = 0; ix < s.x; ix++) {
			if (tMap.map[iy][ix] == 'A')
				tMap.map[iy][ix] = '1';

			if (tMap.map[iy][ix] == '0' && tMap.map[iy][ix + 1] == '0' && tMap.map[iy + 1][ix + 1] == '0' && tMap.map[iy + 1][ix] == '0') {
				Window::getInstance()->printMessage(T("Na mapie wystepuja puste pola 2x2. (") + T(ix) + "," + T(iy) + ")");
				return false;
			}

			if (tMap.map[iy][ix] == '8') {
				if (!isStart)
					isStart = true;
				else {
					Window::getInstance()->printMessage("Na mapie istnieje wiele punktow poczatkowych.");
					return false;
				}
			}

			if (tMap.map[iy][ix] == '9') {
				if (!isMeta)
					isMeta = true;
				else {
					Window::getInstance()->printMessage("Na mapie istnieje wiele punktow koncowych.");
					return false;
				}
			}
			newMap.map[iy][ix] = tMap.map[iy][ix];
		}
	}

	if (!isStart && s > V2D(1, 1)) {
		Window::getInstance()->printMessage("Na mapie brakuje punktu poczatkowego.");
		return false;
	}

	if (!isMeta && s > V2D(1, 1)) {
		Window::getInstance()->printMessage("Na mapie brakuje punktu koncowego.");
		return false;
	}

	Window::getInstance()->map->map = newMap;
	return true;
}

void Editor::createNewMap() {
	tMap.fillMap('A');
}

char Editor::getTexture(int element) {
	switch (element) {
	case 'A':
		return EMPTY;
	case '0':
		return ' ';
	case '8':
		return 'S';
	case '9':
		return 'W';
	case '2':
		return '@';
	case '3':
		return '$';
	case '4':
		return '&';
	case '5':
	case '6':
		return 'D';
	case '1':
		return '#';
	default:
		return 'E';
	}
}