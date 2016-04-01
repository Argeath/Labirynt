#ifndef text_h
#define text_h

#include <string.h>

#define T(a) Text(a)

class Text {
private:
	char* string;

public:
	Text();
	Text(int i);
	Text(char c);
	Text(char* str);
	Text(const Text &str);

	~Text();

	void clear();

	int length();

	Text& operator=(const Text &obj) {
		cpy(obj.string); 
		return *this;
	}

	Text& operator=(char *str) {
		cpy(str);
		return *this;
	}

	Text& operator+(const Text &str) {
		cat(str.string);
		return *this;
	}

	Text& operator+=(const Text &str) {
		cat(str.string);
		return *this;
	}

	Text& operator+(char* str) {
		cat(str);
		return *this;
	}

	friend bool operator==(const Text &t1, const Text &t2) {
		return strcmp(t1.string, t2.string) == 0;
	}

	friend bool operator!=(const Text &t1, const Text &t2) {
		return strcmp(t1.string, t2.string) != 0;
	}

	operator char*(void) {
		return string;
	}

	operator const char*(void) {
		return string;
	}

	const char* toChar() {
		return string;
	}

private:

	void cpy(char *str);
	void cat(char *str);
};

#endif