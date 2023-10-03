#include <windows.h>
#include "typedefs.h"
#include "winuser.h"
#include <algorithm>
#include "clearScreen.h"
#include "constants.h"
#include "rotateCube.h"
#include "drawLine.h"
#include "drawPixel.h"
#include "drawTriangle.h"
#include <vector>
#include <math.h>
#include <string>
#include "fillTriangle.h"
#include "makeHsvData.h"
#include "hsv2rgb.h"
namespace std _GLIBCXX_VISIBILITY(default)
{
}

bool running = true;
void *buffer_memory;
int buffer_width;
int buffer_height;
float fAspectRatio;
int offset = 0;
BITMAPINFO buffer_bitmap_info;

LRESULT CALLBACK window_callback(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam)
{
	LRESULT result = 0;

	switch (uMsg)
	{
	case WM_CLOSE:
	case WM_DESTROY:
	{
		running = false;
	}
	break;

	case WM_SIZE:
	{
		RECT rect;
		GetClientRect(hwnd, &rect);
		buffer_width = rect.right - rect.left;
		buffer_height = rect.bottom - rect.top;
		fAspectRatio = (float)buffer_height / (float)buffer_width;
		int buffer_size = buffer_width * buffer_height * sizeof(unsigned int);

		if (buffer_memory)
			VirtualFree(buffer_memory, 0, MEM_RELEASE);
		buffer_memory = VirtualAlloc(0, buffer_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

		buffer_bitmap_info.bmiHeader.biSize = sizeof(buffer_bitmap_info.bmiHeader);
		buffer_bitmap_info.bmiHeader.biWidth = buffer_width;
		buffer_bitmap_info.bmiHeader.biHeight = buffer_height;
		buffer_bitmap_info.bmiHeader.biPlanes = 1;
		buffer_bitmap_info.bmiHeader.biBitCount = 32;
		buffer_bitmap_info.bmiHeader.biCompression = BI_RGB;
	}
	break;

	default:
	{
		result = DefWindowProcW(hwnd, uMsg, wParam, lParam);
	}
	}
	return result;
}

void MultiplyMatrixVector(Vec3d &i, Vec3d &o, Matrix4x4 &m)
{
	o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
	o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
	o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
	float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

	if (w != 0.0f)
	{
		o.x /= w;
		o.y /= w;
		o.z /= w;
	}
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	// create window class
	WNDCLASSW window_class = {};
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpszClassName = L"Window Class";
	window_class.hCursor = LoadCursor(0, IDC_ARROW);
	window_class.lpfnWndProc = window_callback;
	// ShowCursor(FALSE);

	// register class
	RegisterClassW(&window_class);

	// create window
	HWND window = CreateWindowW(window_class.lpszClassName, L"C++ 3d Graphics Engine", WS_OVERLAPPEDWINDOW | WS_MAXIMIZE | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, hInstance, 0);
	HDC hdc = GetDC(window);

	Mesh meshCube;

	// projection matrix
	float fNear = 0.1f;
	float fFar = 1000.0f;
	float fFov = 60.0f;
	float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);

	Matrix4x4 matProj;

	float delta_time = 0.016666f;
	LARGE_INTEGER frame_begin_time;
	QueryPerformanceCounter(&frame_begin_time);

	float performance_frequency;
	{
		LARGE_INTEGER perf;
		QueryPerformanceFrequency(&perf);
		performance_frequency = (float)perf.QuadPart;
	}

	float rotateX = 0.0f;
	float rotateZ = 0.0f;
	Input input = {};

	meshCube.LoadFromObjectFile("teapot.obj");

	while (running)
	{
		// input
		MSG message;
		while (PeekMessage(&message, window, 0, 0, PM_REMOVE))
		{
			switch (message.message)
			{
			case WM_KEYUP:
			case WM_KEYDOWN:
			{
				unsigned int vk_code = (unsigned int)message.wParam;
				bool is_down = ((message.lParam & (1 << 31)) == 0);

#define process_button(b, vk)                                           \
	case vk:                                                            \
	{                                                                   \
		input.buttons[b].changed = is_down != input.buttons[b].is_down; \
		input.buttons[b].is_down = is_down;                             \
	}                                                                   \
	break;

				switch (vk_code)
				{
				case VK_ESCAPE:
				{
					running = false;
				}
				break;
					process_button(BUTTON_UP, VK_UP);
					process_button(BUTTON_DOWN, VK_DOWN);
					process_button(BUTTON_LEFT, VK_LEFT);
					process_button(BUTTON_RIGHT, VK_RIGHT);
				}
			}
			break;
			default:
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
				break;
			}
			}
		}

		LARGE_INTEGER frame_end_time;
		if (delta_time >= 1.0f / fps)

		{

			matProj.m[0][0] = fAspectRatio * fFovRad;
			matProj.m[1][1] = fFovRad;
			matProj.m[2][2] = fFar / (fFar - fNear);
			matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
			matProj.m[2][3] = 1.0f;
			matProj.m[3][3] = 0.0f;

			frame_begin_time = frame_end_time;
			rotateCube(rotateZ, rotateX, input, delta_time);

			Matrix4x4 matRotZ, matRotX;
			float fTheta = rotateZ;
			Vec3d vCamera = {0, 0, 0};

			// rotaion z
			matRotZ.m[0][0] = cosf(rotateZ);
			matRotZ.m[0][1] = sinf(rotateZ);
			matRotZ.m[1][0] = -sinf(rotateZ);
			matRotZ.m[1][1] = cosf(rotateZ);
			matRotZ.m[2][2] = 1;
			matRotZ.m[3][3] = 1;

			// rotation x
			matRotX.m[0][0] = 1;
			matRotX.m[1][1] = cosf(rotateX);
			matRotX.m[1][2] = sinf(rotateX);
			matRotX.m[2][1] = -sinf(rotateX);
			matRotX.m[2][2] = cosf(rotateX);
			matRotX.m[3][3] = 1;

			clearScreen(buffer_memory, buffer_width, buffer_height, backgroundColor);

			vector<Triangle> vecTrianglesToRaster;

			// draw triangles
			for (auto tri : meshCube.tris)
			{
				Triangle triProjected, triTranslated, triRotatedZ, triRotatedZX;

				// rotate in z axis
				MultiplyMatrixVector(tri.p[0], triRotatedZ.p[0], matRotZ);
				MultiplyMatrixVector(tri.p[1], triRotatedZ.p[1], matRotZ);
				MultiplyMatrixVector(tri.p[2], triRotatedZ.p[2], matRotZ);

				// rotate in x axis
				MultiplyMatrixVector(triRotatedZ.p[0], triRotatedZX.p[0], matRotX);
				MultiplyMatrixVector(triRotatedZ.p[1], triRotatedZX.p[1], matRotX);
				MultiplyMatrixVector(triRotatedZ.p[2], triRotatedZX.p[2], matRotX);

				// offset into the screen
				triTranslated = triRotatedZX;
				triTranslated.p[0].z = triRotatedZX.p[0].z + zOffset;
				triTranslated.p[1].z = triRotatedZX.p[1].z + zOffset;
				triTranslated.p[2].z = triRotatedZX.p[2].z + zOffset;
				;

				// calculate normal
				Vec3d normal, line1, line2;
				line1.x = triTranslated.p[1].x - triTranslated.p[0].x;
				line1.y = triTranslated.p[1].y - triTranslated.p[0].y;
				line1.z = triTranslated.p[1].z - triTranslated.p[0].z;

				line2.x = triTranslated.p[2].x - triTranslated.p[0].x;
				line2.y = triTranslated.p[2].y - triTranslated.p[0].y;
				line2.z = triTranslated.p[2].z - triTranslated.p[0].z;

				// calculate normal with cross product
				normal.x = line1.y * line2.z - line1.z * line2.y;
				normal.y = line1.z * line2.x - line1.x * line2.z;
				normal.z = line1.x * line2.y - line1.y * line2.x;

				float l = sqrtf(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
				normal.x /= l;
				normal.y /= l;
				normal.z /= l;

				float angleToViewer = normal.x * (triTranslated.p[0].x - vCamera.x) + normal.y * (triTranslated.p[0].y - vCamera.y) + normal.z * (triTranslated.p[0].z - vCamera.z);

				// only draw triangles that are facing the screen
				if (angleToViewer > 0.0f)
				{
					Vec3d light_direction = lightDirection;
					float ll = sqrtf(light_direction.x * light_direction.x + light_direction.y * light_direction.y + light_direction.z * normal.z);
					light_direction.x /= ll;
					light_direction.y /= ll;
					light_direction.z /= ll;

					float dp = normal.x * light_direction.x + normal.y * light_direction.y + normal.z * light_direction.z;
					hsv darkenedColor = modelColor;
					if (dp > 0.0f)
					{
						darkenedColor.v = darkenedColor.v * (1 - darkenStrength) + darkenedColor.v * dp * darkenStrength;
					}
					else
					{
						darkenedColor.v = darkenedColor.v * (1 - darkenStrength);
					}

					triTranslated.color = hsv2rgb(darkenedColor);

					// project triangles from 3d to 2d
					MultiplyMatrixVector(triTranslated.p[0], triProjected.p[0], matProj);
					MultiplyMatrixVector(triTranslated.p[1], triProjected.p[1], matProj);
					MultiplyMatrixVector(triTranslated.p[2], triProjected.p[2], matProj);
					triProjected.color = triTranslated.color;

					// scale into view
					triProjected.p[0].x += 1.0f;
					triProjected.p[0].y += 1.0f;
					triProjected.p[1].x += 1.0f;
					triProjected.p[1].y += 1.0f;
					triProjected.p[2].x += 1.0f;
					triProjected.p[2].y += 1.0f;

					triProjected.p[0].x *= 0.5f * (float)buffer_width;
					triProjected.p[0].y *= 0.5f * (float)buffer_height;
					triProjected.p[1].x *= 0.5f * (float)buffer_width;
					triProjected.p[1].y *= 0.5f * (float)buffer_height;
					triProjected.p[2].x *= 0.5f * (float)buffer_width;
					triProjected.p[2].y *= 0.5f * (float)buffer_height;

					vecTrianglesToRaster.push_back(triProjected);
				}
			}

			hsv lineColor = modelColor;
			lineColor.v = lineColor.v * 0.6f;
			int intLineColor = hsv2rgb(lineColor);

			// sort triangles furthest to nearest
			sort(vecTrianglesToRaster.begin(), vecTrianglesToRaster.end(), [](Triangle &t1, Triangle &t2)
				 {
				float z1 = (t1.p[0].z + t1.p[1].z + t1.p[2].z) / 3.0f;
				float z2 = (t2.p[0].z + t2.p[1].z + t2.p[2].z) / 3.0f;
				return z1 > z2; });

			for (auto &triProjected : vecTrianglesToRaster)
			{
				fillTriangle(buffer_memory, buffer_width, buffer_height, triProjected, triProjected.color);
				drawTriangle(buffer_memory, buffer_width, buffer_height, triProjected, intLineColor);
			}
			StretchDIBits(hdc, 0, 0, buffer_width, buffer_height, 0, 0, buffer_width, buffer_height, buffer_memory, &buffer_bitmap_info, DIB_RGB_COLORS, SRCCOPY);
		}
		QueryPerformanceCounter(&frame_end_time);
		delta_time = (float)(frame_end_time.QuadPart - frame_begin_time.QuadPart) / performance_frequency;
	}
	return 0;
};
