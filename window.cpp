#include <time.h>
#include "conio2.h"
#include "text.h"
#include "player.h"
#include "Vector2D.h"
#include "map.h"
#include "minimap.h"
#include "editor.h"
#include "window.h"

#define MINIMAP_SIZE 25

text_info getTextInfo() {
	text_info* info = new text_info;
	gettextinfo(info);
	text_info var = *info;
	delete info;
	return var;
}

Window* Window::instance = 0;

Window::Window() {
	if (instance) return; // throw exception
	isMessageVisible = false;
	isInEditor = false;
	inputModeType = OPENMAP;
	isInInputMode = false;
	timeDisplayed = 0;
	message = NULL;

	timeStarted = time(0);
	map = new Map;
	minimap = new Minimap;
	editor = new Editor;
	instance = this;
}

Window::~Window() {
	delete map;
	delete minimap;
	delete editor;
	InputText.clear();
	instance = NULL;
}

void Window::drawWindow() {
	text_info info = getTextInfo();

	_setcursortype(_NOCURSOR);

	// Rogi ekranu
	gotoxy(1, 1);
	putch(201);

	gotoxy(info.screenwidth, 1);
	putch(187);

	gotoxy(1, info.screenheight);
	putch(200);

	gotoxy(info.screenwidth, info.screenheight);
	putch(188);


	// Poziome obramowanie
	gotoxy(2, 1);
	for (int i = 2; i < info.screenwidth; i++) {
		putch(205);
	}
	gotoxy(2, info.screenheight);
	for (int i = 2; i < info.screenwidth; i++) {
		putch(205);
	}

	// Pionowe obramowanie
	for (int i = 2; i < info.screenheight; i++) {
		gotoxy(1, i);
		putch(186);
		gotoxy(info.screenwidth, i);
		putch(186);
	}

	// Obramowanie minimapy
	for (int j = 0; j < 2; j++) {
		for (int i = 2; i < info.screenheight; i++) {
			gotoxy(info.screenwidth - MINIMAP_SIZE - j, i);
			putch(186);
		}
		gotoxy(info.screenwidth - MINIMAP_SIZE - j, 1);
		putch(203);
		gotoxy(info.screenwidth - MINIMAP_SIZE - j, info.screenheight);
		putch(202);
	}


	gotoxy(info.screenwidth - MINIMAP_SIZE + 1, MINIMAP_SIZE / 2 + 2);
	for (int i = info.screenwidth - MINIMAP_SIZE + 1; i < info.screenwidth; i++) {
		putch(205);
	}

	gotoxy(info.screenwidth - MINIMAP_SIZE + 1, MINIMAP_SIZE / 2 + 4);
	for (int i = info.screenwidth - MINIMAP_SIZE + 1; i < info.screenwidth; i++) {
		putch(205);
	}

	gotoxy(info.screenwidth, MINIMAP_SIZE / 2 + 2);
	putch(185);
	gotoxy(info.screenwidth, MINIMAP_SIZE / 2 + 4);
	putch(185);
	gotoxy(info.screenwidth - MINIMAP_SIZE, MINIMAP_SIZE / 2 + 2);
	putch(204);
	gotoxy(info.screenwidth - MINIMAP_SIZE, MINIMAP_SIZE / 2 + 4);
	putch(204);

	printSteps(0);
}

void Window::drawEditorWindow() {
	text_info info = getTextInfo();

	_setcursortype(_SOLIDCURSOR);

	// Rogi ekranu
	gotoxy(1, 1);
	putch(201);

	gotoxy(info.screenwidth, 1);
	putch(187);

	gotoxy(1, info.screenheight);
	putch(200);

	gotoxy(info.screenwidth, info.screenheight);
	putch(188);


	// Poziome obramowanie
	gotoxy(2, 1);
	for (int i = 2; i < info.screenwidth; i++) {
		putch(205);
	}
	gotoxy(2, info.screenheight);
	for (int i = 2; i < info.screenwidth; i++) {
		putch(205);
	}

	// Pionowe obramowanie
	for (int i = 2; i < info.screenheight; i++) {
		gotoxy(1, i);
		putch(186);
		gotoxy(info.screenwidth, i);
		putch(186);
	}

	// Obramowanie panelu bocznego
	for (int j = 0; j < 2; j++) {
		for (int i = 2; i < info.screenheight; i++) {
			gotoxy(info.screenwidth - MINIMAP_SIZE - j, i);
			putch(186);
		}
		gotoxy(info.screenwidth - MINIMAP_SIZE - j, 1);
		putch(203);
		gotoxy(info.screenwidth - MINIMAP_SIZE - j, info.screenheight);
		putch(202);
	}

	int linia = 2;
	gotoxy(info.screenwidth - MINIMAP_SIZE + 2, linia++);
	cputs(T("EDYTOR:"));

	linia++;

	gotoxy(info.screenwidth - MINIMAP_SIZE + 2, linia++);
	cputs(T("[S] - Zapisz"));

	gotoxy(info.screenwidth - MINIMAP_SIZE + 2, linia++);
	cputs(T("[W] - Zapisz jako..."));

	gotoxy(info.screenwidth - MINIMAP_SIZE + 2, linia++);
	cputs(T("[E] - Wyjdz"));

	gotoxy(info.screenwidth - MINIMAP_SIZE + 2, linia++);
	cputs(T("[0] - Korytarz"));

	gotoxy(info.screenwidth - MINIMAP_SIZE + 2, linia++);
	cputs(T("[1 - 4] - Sciana"));

	gotoxy(info.screenwidth - MINIMAP_SIZE + 2, linia++);
	cputs(T("[5 / 6] - Drzwi"));

	gotoxy(info.screenwidth - MINIMAP_SIZE + 2, linia++);
	cputs(T("[8] - Start"));

	gotoxy(info.screenwidth - MINIMAP_SIZE + 2, linia++);
	cputs(T("[9] - Meta"));

	gotoxy(info.screenwidth - MINIMAP_SIZE + 2, linia++);
	cputs(T("[A] - Wyczysc pole"));

	gotoxy(info.screenwidth - MINIMAP_SIZE + 2, linia++);
	cputs(T("[N] - Nowa mapa"));

}

void Window::drawMessageWindow() {
	text_info info = getTextInfo();

	_setcursortype(_NOCURSOR);

	double padding = 0.2;

	// Czyszczenie œrodka
	for (int y = info.screenheight * 2 * padding; y < info.screenheight * (1 - 2 * padding); y++) {
		for (int x = info.screenwidth * padding; x < info.screenwidth * (1 - padding); x++) {
			gotoxy(x, y);
			putch(' ');
		}
	}

	// Rogi ekranu
	gotoxy(info.screenwidth * padding, info.screenheight * 2 * padding);
	putch(201);

	gotoxy(info.screenwidth * (1 - padding), info.screenheight * 2 * padding);
	putch(187);

	gotoxy(info.screenwidth * padding, info.screenheight * (1 - 2 * padding));
	putch(200);

	gotoxy(info.screenwidth * (1 - padding), info.screenheight * (1 - 2 * padding));
	putch(188);

	for (int i = info.screenwidth * padding + 1; i < info.screenwidth * (1 - padding); i++) {
		gotoxy(i, info.screenheight * 2 * padding);
		putch(205);
		gotoxy(i, info.screenheight * (1 - 2 * padding));
		putch(205);
	}
	for (int i = info.screenheight * 2 * padding + 1; i < info.screenheight * (1 - 2 * padding); i++) {
		gotoxy(info.screenwidth * padding, i);
		putch(186);
		gotoxy(info.screenwidth * (1 - padding), i);
		putch(186);
	}

	gotoxy(info.screenwidth * padding + 4, info.screenheight / 2);
	cputs(message);

	gotoxy(info.screenwidth * (1 - padding) - 24, info.screenheight / 2 + 2);
	cputs(T("Press space to continue"));
}

void Window::drawHelpWindow() {
	text_info info = getTextInfo();

	_setcursortype(_NOCURSOR);

	double padding = 0.2;

	// Czyszczenie œrodka
	for (int y = info.screenheight * padding; y < info.screenheight * (1 - padding); y++) {
		for (int x = info.screenwidth * padding; x < info.screenwidth * (1 - padding); x++) {
			gotoxy(x, y);
			putch(' ');
		}
	}

	// Rogi ekranu
	gotoxy(info.screenwidth * padding, info.screenheight * padding);
	putch(201);

	gotoxy(info.screenwidth * (1 - padding), info.screenheight * padding);
	putch(187);

	gotoxy(info.screenwidth * padding, info.screenheight * (1 - padding));
	putch(200);

	gotoxy(info.screenwidth * (1 - padding), info.screenheight * (1 - padding));
	putch(188);

	for (int i = info.screenwidth * padding + 1; i < info.screenwidth * (1 - padding); i++) {
		gotoxy(i, info.screenheight * padding);
		putch(205);
		gotoxy(i, info.screenheight * (1 - padding));
		putch(205);
	}

	for (int i = info.screenheight * padding + 1; i < info.screenheight * (1 - padding); i++) {
		gotoxy(info.screenwidth * padding, i);
		putch(186);
		gotoxy(info.screenwidth * (1 - padding), i);
		putch(186);
	}

	int h = 1;

	gotoxy(info.screenwidth / 2 - 2, info.screenheight * padding + h++);
	cputs("Pomoc");

	h++;

	gotoxy(info.screenwidth * padding + 2, info.screenheight * padding + h++);
	cputs("Strzalka w przod/tyl - poruszanie sie");

	gotoxy(info.screenwidth * padding + 2, info.screenheight * padding + h++);
	cputs("Strzalki w boki - obracanie");

	gotoxy(info.screenwidth * padding + 2, info.screenheight * padding + h++);
	cputs("D - otwieranie/zamykanie drzwi");

	gotoxy(info.screenwidth * padding + 2, info.screenheight * padding + h++);
	cputs("I - wczytanie domyslnego labiryntu");

	gotoxy(info.screenwidth * padding + 2, info.screenheight * padding + h++);
	cputs("H - Pomoc");

	gotoxy(info.screenwidth * padding + 2, info.screenheight * padding + h++);
	cputs("E - edytor map");

	gotoxy(info.screenwidth * padding + 2, info.screenheight * padding + h++);
	cputs("O - wczytaj mape");

	gotoxy(info.screenwidth * padding + 2, info.screenheight * padding + h++);
	cputs("S - zapisz mape (Edytor map)");

	gotoxy(info.screenwidth * padding + 2, info.screenheight * padding + h++);
	cputs("R - restart mapy");

	gotoxy(info.screenwidth * padding + 2, info.screenheight * padding + h++);
	cputs("A - plynne poruszanie");

	gotoxy(info.screenwidth * padding + 2, info.screenheight * padding + h++);
	cputs("Q - Wyjscie");

	gotoxy(info.screenwidth * (1 - padding) - 24, info.screenheight * (1 - padding) - 1);
	cputs(T("Press space to continue"));
}

void Window::drawInputWindow() {
	text_info info = getTextInfo();

	_setcursortype(_NORMALCURSOR);

	// Czyszczenie œrodka
	for (int y = info.screenheight / 2 - 2; y <= info.screenheight / 2 + 1; y++) {
		for (int x = info.screenwidth * 0.2; x <= info.screenwidth * 0.8; x++) {
			gotoxy(x, y);
			putch(' ');
		}
	}

	// Rogi ekranu
	gotoxy(info.screenwidth * 0.2, info.screenheight / 2 - 2);
	putch(201);

	gotoxy(info.screenwidth * 0.8, info.screenheight / 2 - 2);
	putch(187);

	gotoxy(info.screenwidth * 0.2, info.screenheight / 2 + 1);
	putch(200);

	gotoxy(info.screenwidth * 0.8, info.screenheight / 2 + 1);
	putch(188);

	for (int i = info.screenwidth * 0.2 + 1; i < info.screenwidth * 0.8; i++) {
		gotoxy(i, info.screenheight / 2 - 2);
		putch(205);
		gotoxy(i, info.screenheight / 2 + 1);
		putch(205);
	}

	for (int i = info.screenheight / 2 - 1; i <= info.screenheight / 2; i++) {
		gotoxy(info.screenwidth * 0.2, i);
		putch(186);
		gotoxy(info.screenwidth * 0.8, i);
		putch(186);
	}
	
	gotoxy(info.screenwidth * 0.8 - 12, info.screenheight / 2);
	cputs(T("Press Enter"));

	gotoxy(info.screenwidth * 0.2 + 4, info.screenheight / 2 - 1);
	cputs(T("Load map:"));
}

void Window::showInputWindow() {
	InputText.clear();
	drawInputWindow();
	isInInputMode = true;
}

void Window::addInputChar(char input) {
	text_info info = getTextInfo();
	gotoxy(info.screenwidth * 0.2 + 14 + InputText.length(), info.screenheight / 2 - 1);
	putch(input);

	Window::InputText += input;
}

void Window::saveInput() {
	hideWindows();
	isInInputMode = false;

	if (inputModeType == OPENMAP) {
		map->loadMap(InputText);

		if (map->loaded)
			drawMinimap();
	}
	else if (inputModeType == SAVEMAP) {
		drawEditorWindow();
		map->filename = InputText;
		map->saveMap(map->filename);
		map->loaded = true;
		drawMinimap();

		printMessage(T("Map has been saved. (") + map->filename + ")");
	}
}

void Window::clearInput() {
	hideWindows();
	isInInputMode = false;
	InputText.clear();
}

void Window::hideWindows() {
	isMessageVisible = false;
	message = nullptr;
	clearWindow();
	drawWindow();
}

void Window::printMessage(char* msg) {
	if (message == nullptr) {
		message = msg;
		drawMessageWindow();
		isMessageVisible = true;
	}
}

void Window::showHelpWindow() {
	drawHelpWindow();
	isMessageVisible = true;
}

void Window::printMapInfo(char* mapInfo) {
	textcolor(DARKGRAY);
	text_info info = getTextInfo();

	gotoxy(info.screenwidth - MINIMAP_SIZE + 1, MINIMAP_SIZE / 2 + 3);
	cputs(mapInfo);
}

void Window::printSteps(int steps) {
	textcolor(DARKGRAY);
	text_info info = getTextInfo();

	gotoxy(info.screenwidth - MINIMAP_SIZE + 1, MINIMAP_SIZE / 2 + 5);
	cputs(T("Kroki: ") + T(steps));
}

void Window::printTime() {
	textcolor(DARKGRAY);
	text_info info = getTextInfo();

	int seconds = difftime(time(0), timeStarted);
	if ((timeDisplayed < seconds || seconds == 0) && map->loaded) {
		timeDisplayed = seconds;

		int minutes = seconds / 60;
		seconds %= 60;

		Text str = Text();
		if (minutes > 0)
			str = T(minutes) + ":";
		if (seconds < 10 && minutes > 0)
			str += '0';
		str += seconds;

		gotoxy(info.screenwidth - MINIMAP_SIZE + 1, MINIMAP_SIZE / 2 + 6);
		cputs(T("Czas: ") + str + "   ");
	}
}

void Window::resetTime() {
	timeStarted = time(0);
	timeDisplayed = 0;
}

void Window::drawMinimap() {
	text_info info = getTextInfo();
	int size = MINIMAP_SIZE / 2;
	minimap->setSize(V2D(size, size));

	mapArray _minimap = minimap->draw();

	for (int i = 0; i < _minimap.size.y; i++) {
		for (int j = 0; j < _minimap.size.x; j++) {
			if (_minimap.map[i][j] == 'P') {
				textcolor(GREEN);
				gotoxy(info.screenwidth - MINIMAP_SIZE + 2 * j + 1, i + 2);
				cputs(Player::getSymbol());
				continue;
			}

			textcolor(DARKGRAY);

			gotoxy(info.screenwidth - MINIMAP_SIZE + 2 * j + 1, i + 2);
			putch(_minimap.map[i][j]);
			gotoxy(info.screenwidth - MINIMAP_SIZE + 2 * j + 2, i + 2);
			putch(_minimap.map[i][j]);
		}
	}
	textcolor(DARKGRAY);
}

void Window::drawEditor() {
	text_info info = getTextInfo();

	_setcursortype(_SOLIDCURSOR);

	mapArray m = editor->getMapToDraw();

	for (int i = 0; i < m.size.y; i++) {
		gotoxy(2, i + 2);
		for (int j = 0; j < m.size.x; j++) {
			putch(m.map[i][j]);
		}
	}
	gotoxy(editor->cursor.x + 2, editor->cursor.y + 2);

	m.freeMemory();
}

void Window::clearWindow() {
	text_info info = getTextInfo();

	for (int y = 1; y < info.screenheight; y++) {
		for (int x = 1; x < info.screenwidth; x++) {
			gotoxy(x, y);
			putch(' ');
		}
	}
}

Vector2D Window::getGameWindowSize() {
	text_info info = getTextInfo();
	int width = info.screenwidth - MINIMAP_SIZE - 3;
	int height = info.screenheight - 2;
	return V2D(width, height);
}

void Window::moveCursor(int x, int y) {
	gotoxy(x, y);
}