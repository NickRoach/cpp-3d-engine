using namespace std;
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
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
	int color;
};

struct Mesh
{
	std::vector<Triangle> tris;
	bool LoadFromObjectFile(std::string sFilename)
	{
		ifstream f(sFilename);
		if (!f.is_open())
			return false;

		// Local cache of verts
		vector<Vec3d> verts;

		while (!f.eof())
		{
			char line[128];
			f.getline(line, 128);

			basic_stringstream<char> s;
			s << line;

			char junk;
			if (line[0] == 'v')
			{
				Vec3d v;
				s >> junk >> v.x >> v.y >> v.z;
				verts.push_back(v);
			}

			if (line[0] == 'f')
			{
				int f[3];
				s >> junk >> f[0] >> f[1] >> f[2];
				tris.push_back({verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1]});
			}
		}

		return true;
	}
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
