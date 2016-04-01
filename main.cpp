#include <iostream>
#include <time.h>
#include "conio2.h"
#include "minimap.h"
#include "map.h"
#include "editor.h"
#include "window.h"
#include "player.h"
#include "GameWindow.h"

int main()
{
	int input;
	int steps = 0;
	GameWindow gameWindow = GameWindow();
	Window* window = Window::getInstance();
	Editor* editor = window->editor;

	settitle("Dominik Kinal 160589");
	textbackground(BLACK);

	window->clearWindow();
	window->drawWindow();

	gameWindow.setSize(window->getGameWindowSize());

	_setcursortype(_NOCURSOR);

	do {
		if (kbhit() != 0) {
			input = getch();
			if (window->isInInputMode) {
				if (input == 13) window->saveInput();
				else if (input == 27) window->clearInput();
				else {
					window->addInputChar(input);
				}
			}
			else if (window->isMessageVisible) {
				if (input == 32) {
					window->hideWindows(); // Press space to hide message

					if (window->isInEditor) {
						window->drawEditorWindow();
						window->drawEditor();
					}
					else {
						window->drawWindow();
						window->drawMinimap();
						if (window->map->loaded)
							window->printMapInfo(T("Map: ") + window->map->filename);
					}
				}
			}
			else if (window->isInEditor) {
				if (input == 's' || input == 'S') {
					if (editor->prepareMapToSave()) {
						if ( ! window->map->loaded) {
							window->inputModeType = SAVEMAP;
							window->showInputWindow();
						}
						else {
							window->map->saveMap(window->map->filename);
							window->map->loaded = true;
							window->printMessage(T("Map has been saved. (") + window->map->filename + ")");
						}
					}
				}
				else if (input == 'w' || input == 'W') {
					if (editor->prepareMapToSave()) {
						window->inputModeType = SAVEMAP;
						window->showInputWindow();
					}
				}
				else if (input == 'e' || input == 'E') {
					if (editor->prepareMapToSave()) {
						window->clearWindow();
						window->isInEditor = false;
						window->map->movePlayerToStart();
						window->drawWindow();
						window->drawMinimap();
						window->printMapInfo(T("Map: ") + window->map->filename);
						gameWindow.drawWindow();
					}
				}
				else if (input == 75)
					editor->moveCursor(WEST);
				else if (input == 77)
					editor->moveCursor(EAST);
				else if (input == 72)
					editor->moveCursor(NORTH);
				else if (input == 80)
					editor->moveCursor(SOUTH);
				else if (input >= '0' && input <= '9') {
					editor->placeObject(input);
					window->drawEditor();
				}
				else if (input == 'A' || input == 'a') {
					editor->placeObject('A');
					window->drawEditor();
				}
				else if (input == 'N' || input == 'n') {
					window->map->filename = T("");
					window->map->loaded = false;
					editor->createNewMap();
					window->drawEditor();
				}
			}
			else {
				if (input == 75) {
					Player::rotateLeft();
					gameWindow.drawWindow();
				}
				else if (input == 77) {
					Player::rotateRight();
					gameWindow.drawWindow();
				}
				else if (input == 72) {
					if (Player::canGoForward()) {
						window->printSteps(++steps);
						gameWindow.animationType = FORWARD;

						if (gameWindow.animationsOn)
							gameWindow.animationStep = 6;
						else
							gameWindow.animationStep = 1;

						gameWindow.drawWindow();
					}
				}
				else if (input == 80) {
					if (Player::goBackward()) {
						window->printSteps(++steps);
						gameWindow.animationType = BACKWARD;

						if (gameWindow.animationsOn)
							gameWindow.animationStep = 6;
						else
							gameWindow.animationStep = 1;

						gameWindow.drawWindow();
					}
				}
				else if (input == 'o' || input == 'O') {
					window->inputModeType = OPENMAP;
					window->showInputWindow();
				}
				else if (input == 'd' || input == 'D') {
					if (Player::canOpenDoor()) {
						gameWindow.animationType = DOOR_OPEN;

						if (gameWindow.animationsOn)
							gameWindow.animationStep = 6;
						else
							gameWindow.animationStep = 1;

						gameWindow.drawWindow();
					}
					else if (Player::canCloseDoor()) {
						gameWindow.animationType = DOOR_CLOSE;

						if (gameWindow.animationsOn)
							gameWindow.animationStep = 6;
						else
							gameWindow.animationStep = 1;

						gameWindow.drawWindow();
					}
				}
				else if (input == 'h' || input == 'H') window->showHelpWindow();
				else if (input == 'r' || input == 'R') {
					window->map->loadMap(window->map->filename);
					gameWindow.drawWindow();
				}
				else if (input == 'i' || input == 'I') {
					window->map->loadMap("default");
					window->drawMinimap();
					gameWindow.drawWindow();
				}
				else if (input == 'e' || input == 'E') {
					if (window->map->loaded)
						editor->cloneMap();
					else
						editor->createNewMap();

					window->clearWindow();
					window->isInEditor = true;
					window->drawEditorWindow();
					window->drawEditor();
				}
				else if (input == 'a' || input == 'A') {
					gameWindow.animationsOn = !gameWindow.animationsOn;
				}
			}
		}
		else input = 0;

		if( ! window->isInEditor && ! window->isMessageVisible && ! window->isInInputMode)
			window->printTime();

	} while (input != 'q');

	return 0;
}