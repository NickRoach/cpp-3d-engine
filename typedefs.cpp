#include <vector>

struct ButtonState
{
	bool is_down;
	bool changed;
};

enum
{
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_LEFT,
	BUTTON_RIGHT,
	BUTTON_COUNT
};

struct Input
{
	ButtonState buttons[BUTTON_COUNT];
};

struct Coord
{
	int x;
	int y;
};

struct Vec3d
{
	float x, y, z;
};

struct Triangle
{
	Vec3d p[3];
};

struct Mesh
{
	std::vector<Triangle> tris;
};

struct Matrix4x4
{
	float m[4][4] = {0};
};
