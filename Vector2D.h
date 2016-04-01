#ifndef vector2d_h
#define vector2d_h

#define V2D(a, b) Vector2D(a, b)

enum Direction {
	NORTH,
	EAST,
	SOUTH,
	WEST
};

class Vector2D {
public:
	int x;
	int y;

	Vector2D() : x(0), y(0) {}
	Vector2D(int a, int b) : x(a), y(b) {}
	Vector2D(Direction dir) : x(0), y(0) {
		switch (dir) {
		case NORTH:
			y = -1;
			break;
		case SOUTH:
			y = 1;
			break;
		case EAST:
			x = 1;
			break;
		case WEST:
			x = -1;
			break;
		}
	}

	Vector2D reversed() {
		return V2D(y, x);
	}

	Vector2D& operator+(const Vector2D &v) {
		x += v.x;
		y += v.y;
		return *this;
	}

	Vector2D& operator-(const Vector2D &v) {
		x -= v.x;
		y -= v.y;
		return *this;
	}

	Vector2D& operator*(const Vector2D &v) {
		x *= v.x;
		y *= v.y;
		return *this;
	}

	Vector2D& operator*(const int &c) {
		x *= c;
		y *= c;
		return *this;
	}

	Vector2D& operator/(const Vector2D &v) {
		x /= v.x;
		y /= v.y;
		return *this;
	}

	Vector2D& operator/(const int &c) {
		x /= c;
		y /= c;
		return *this;
	}

	friend bool operator==(const Vector2D &v1, const Vector2D &v2) {
		return (v1.x == v2.x && v1.y == v2.y);
	}

	friend bool operator!=(const Vector2D &v1, const Vector2D &v2) {
		return (v1.x != v2.x || v1.y != v2.y);
	}

	friend bool operator>(const Vector2D &v1, const Vector2D &v2) {
		return (v1.x > v2.x && v1.y > v2.y);
	}

	friend bool operator<(const Vector2D &v1, const Vector2D &v2) {
		return (v1.x < v2.x && v1.y < v2.y);
	}

	friend bool operator>=(const Vector2D &v1, const Vector2D &v2) {
		return (v1.x >= v2.x && v1.y >= v2.y);
	}

	friend bool operator<=(const Vector2D &v1, const Vector2D &v2) {
		return (v1.x <= v2.x && v1.y <= v2.y);
	}

};


#endif