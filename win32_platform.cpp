#include <windows.h>
#include "typedefs.h"
#include "winuser.h"
#include "clearScreen.h"
#include "drawRect.h"
#include "limit.h"
#include "constants.h"
#include "moveSquare.h"
#include "drawLine.h"
#include "drawPolygon.h"
#include "drawPixel.h"
#include "drawTriangle.h"
#include <vector>
#include <math.h>
namespace std _GLIBCXX_VISIBILITY(default)
{
}

bool running = true;
void *buffer_memory;
int buffer_width;
int buffer_height;
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
	HWND window = CreateWindowW(window_class.lpszClassName, L"Window with C++", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, hInstance, 0);
	{
		// center the window on the screen and set the size
		MONITORINFO mi = {sizeof(mi)};
		GetMonitorInfo(MonitorFromWindow(window, MONITOR_DEFAULTTOPRIMARY), &mi);
		long monitorWidth = mi.rcMonitor.right - mi.rcMonitor.left;
		long monitorHeight = mi.rcMonitor.bottom - mi.rcMonitor.top;
		// SetWindowPos(window, HWND_TOP, mi.rcMonitor.left + monitorWidth / 2 - windowWidth / 2, mi.rcMonitor.top + monitorHeight / 2 - windowHeight / 2, windowWidth, windowHeight, SWP_NOOWNERZORDER | SWP_FRAMECHANGED);

		// remove the title bar
		// SetWindowLong(window, GWL_STYLE, GetWindowLong(window, GWL_STYLE) & ~WS_OVERLAPPEDWINDOW);

		// make fullscreen
		// SetWindowPos(window, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
	}
	HDC hdc = GetDC(window);

	Mesh meshCube;

	meshCube.tris = {
		// south
		{0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f},

		// east
		{1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f},

		// north
		{1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f},
		{1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f},

		// west
		{0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f},

		// top
		{0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
		{0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f},

		// bottom
		{1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f},
		{1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f},
	};

	// projection matrix
	float fNear = 0.1f;
	float fFar = 1000.0f;
	float fFov = 90.0f;
	float fAspectRatio = (float)buffer_height / (float)buffer_width;
	float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);

	Matrix4x4 matProj;

	matProj.m[0][0] = fAspectRatio * fFovRad;
	matProj.m[1][1] = fFovRad;
	matProj.m[2][2] = fFar / (fFar - fNear);
	matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
	matProj.m[2][3] = 1.0f;
	matProj.m[3][3] = 0.0f;

	float delta_time = 0.016666f;
	LARGE_INTEGER frame_begin_time;
	QueryPerformanceCounter(&frame_begin_time);

	float performance_frequency;
	{
		LARGE_INTEGER perf;
		QueryPerformanceFrequency(&perf);
		performance_frequency = (float)perf.QuadPart;
	}

	float squareX = buffer_width / 2 - squareWidth / 2;
	float squareY = buffer_height / 2 - squareHeight / 2;
	Input input = {};

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
		moveSquare(squareX, squareY, input, delta_time, buffer_width, buffer_height);
		// clearScreen(buffer_memory, buffer_width, buffer_height, backgroundColor);
		// drawRect(buffer_memory, buffer_width, buffer_height, (int)squareX, (int)squareY, squareWidth, squareHeight, squareColor);
		// drawPolygon(buffer_memory, buffer_width, buffer_height, coords, sizeof(coords) / sizeof(coords[0]), lineColor);
		clearScreen(buffer_memory, buffer_width, buffer_height, backgroundColor);

		Matrix4x4 matRotZ, matRotX;
		float fTheta = squareX;

		// rotaion z
		matRotZ.m[0][0] = cosf(fTheta);
		matRotZ.m[0][1] = sinf(fTheta);
		matRotZ.m[1][0] = -sinf(fTheta);
		matRotZ.m[1][1] = cosf(fTheta);
		matRotZ.m[2][2] = 1;
		matRotZ.m[3][3] = 1;

		// rotation x
		matRotX.m[0][0] = 1;
		matRotX.m[1][1] = cosf(fTheta * 0.5f);
		matRotX.m[1][2] = sinf(fTheta * 0.5f);
		matRotX.m[2][1] = -sinf(fTheta * 0.5f);
		matRotX.m[2][2] = cosf(fTheta * 0.5f);
		matRotX.m[3][3] = 1;

		// draw triangles
		for (auto tri : meshCube.tris)
		{
			Triangle triProjected, triTranslated, triRotatedZ, triRotatedZX;

			MultiplyMatrixVector(tri.p[0], triRotatedZ.p[0], matRotZ);
			MultiplyMatrixVector(tri.p[1], triRotatedZ.p[1], matRotZ);
			MultiplyMatrixVector(tri.p[2], triRotatedZ.p[2], matRotZ);

			MultiplyMatrixVector(triRotatedZ.p[0], triRotatedZX.p[0], matRotX);
			MultiplyMatrixVector(triRotatedZ.p[1], triRotatedZX.p[1], matRotX);
			MultiplyMatrixVector(triRotatedZ.p[2], triRotatedZX.p[2], matRotX);

			triTranslated = triRotatedZX;
			triTranslated.p[0].z = triRotatedZX.p[0].z + 3.0f;
			triTranslated.p[1].z = triRotatedZX.p[1].z + 3.0f;
			triTranslated.p[2].z = triRotatedZX.p[2].z + 3.0f;

			MultiplyMatrixVector(triTranslated.p[0], triProjected.p[0], matProj);
			MultiplyMatrixVector(triTranslated.p[1], triProjected.p[1], matProj);
			MultiplyMatrixVector(triTranslated.p[2], triProjected.p[2], matProj);

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

			drawTriangle(buffer_memory, buffer_width, buffer_height, triProjected, 0xFFFFFF);
		}

		StretchDIBits(hdc, 0, 0, buffer_width, buffer_height, 0, 0, buffer_width, buffer_height, buffer_memory, &buffer_bitmap_info, DIB_RGB_COLORS, SRCCOPY);

		LARGE_INTEGER frame_end_time;
		QueryPerformanceCounter(&frame_end_time);
		delta_time = (float)(frame_end_time.QuadPart - frame_begin_time.QuadPart) / performance_frequency;
		frame_begin_time = frame_end_time;
	}
	return 0;
};
