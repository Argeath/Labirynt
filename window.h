#ifndef window_h
#define window_h

#include "text.h"
#include "Vector2D.h"

enum InputModeType {
	OPENMAP,
	SAVEMAP
};

class Window {
	static Window * instance;

public:

	Map* map;
	Minimap* minimap;
	Editor* editor;

	bool isMessageVisible;
	bool isInInputMode;
	InputModeType inputModeType;
	bool isInEditor;

	int timeDisplayed;
	time_t timeStarted;

	Text InputText;

private:
	char* message;

	void drawHelpWindow();
	void drawMessageWindow();
	void drawInputWindow();

public:
	static Window* getInstance() {
		if (!instance) initInstance();
		return instance;
	}
	static void initInstance() {
		new Window;
	}

	~Window();

	void addInputChar(char input);
	void saveInput();
	void clearInput();
	void showHelpWindow();
	void showInputWindow();
	void drawWindow();
	void drawEditorWindow();
	void hideWindows();
	void printMessage(char *message);
	void printMapInfo(char *mapInfo);
	void printTime();
	void resetTime();
	void printSteps(int steps);
	void clearWindow();
	void drawMinimap();
	void drawEditor();
	Vector2D getGameWindowSize();
	void moveCursor(int x, int y);

protected:
	Window();

};

#endif