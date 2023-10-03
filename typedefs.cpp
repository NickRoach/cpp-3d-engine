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

typedef struct
{
	double r; // ∈ [0, 1]
	double g; // ∈ [0, 1]
	double b; // ∈ [0, 1]
} rgb;

typedef struct
{
	double h; // ∈ [0, 360]
	double s; // ∈ [0, 1]
	double v; // ∈ [0, 1]
} hsv;
