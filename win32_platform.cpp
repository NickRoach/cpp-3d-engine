﻿#include <windows.h>
#include "typedefs.h"
#include "winuser.h"
#include "clearScreen.h"
#include "drawRect.h"
#include "limit.h"

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

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	// create window class
	WNDCLASSW window_class = {};
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpszClassName = L"Window Class";
	window_class.hCursor = LoadCursor(0, IDC_ARROW);
	window_class.lpfnWndProc = window_callback;

	// register class
	RegisterClassW(&window_class);

	const long windowWidth = 1600;
	const long windowHeight = 900;

	// create window
	HWND window = CreateWindowW(window_class.lpszClassName, L"Window with C++", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, hInstance, 0);
	{
		// remove the title bar
		// SetWindowLong(window, GWL_STYLE, GetWindowLong(window, GWL_STYLE) & ~WS_OVERLAPPEDWINDOW);
		// center the window on the screen and set the size
		MONITORINFO mi = {sizeof(mi)};
		GetMonitorInfo(MonitorFromWindow(window, MONITOR_DEFAULTTOPRIMARY), &mi);
		long monitorWidth = mi.rcMonitor.right - mi.rcMonitor.left;
		long monitorHeight = mi.rcMonitor.bottom - mi.rcMonitor.top;
		SetWindowPos(window, HWND_TOP, mi.rcMonitor.left + monitorWidth / 2 - windowWidth / 2, mi.rcMonitor.top + monitorHeight / 2 - windowHeight / 2, windowWidth, windowHeight, SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
	}
	HDC hdc = GetDC(window);

	const int backgroundColor = 0x000000;
	const int squareColor = 0x009900;
	const int squareWidth = 69;
	const int squareHeight = 69;
	float squareX = buffer_width / 2 - squareWidth / 2;
	float squareY = buffer_height / 2 - squareHeight / 2;

	Input input = {};

	float delta_time = 0.016666f;
	LARGE_INTEGER frame_begin_time;
	QueryPerformanceCounter(&frame_begin_time);

	float performance_frequency;
	{
		LARGE_INTEGER perf;
		QueryPerformanceFrequency(&perf);
		performance_frequency = (float)perf.QuadPart;
	}

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

			default:
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
				break;
			}
			}
		}
		if (input.buttons[BUTTON_UP].is_down)
		{
			squareY = limit(squareY + (float)1000 * delta_time, 0, buffer_height - squareHeight);
		}
		if (input.buttons[BUTTON_DOWN].is_down)
		{
			squareY = limit(squareY - (float)1000 * delta_time, 0, buffer_height - squareHeight);
		}
		if (input.buttons[BUTTON_LEFT].is_down)
		{
			squareX = limit(squareX - (float)1000 * delta_time, 0, buffer_width - squareWidth);
		}
		if (input.buttons[BUTTON_RIGHT].is_down)
		{
			squareX = limit(squareX + (float)1000 * delta_time, 0, buffer_width - squareWidth);
		}
		clearScreen(buffer_memory, buffer_width, buffer_height, backgroundColor);
		drawRect(buffer_memory, buffer_width, buffer_height, squareX, squareY, squareWidth, squareHeight, squareColor);
		StretchDIBits(hdc, 0, 0, buffer_width, buffer_height, 0, 0, buffer_width, buffer_height, buffer_memory, &buffer_bitmap_info, DIB_RGB_COLORS, SRCCOPY);

		LARGE_INTEGER frame_end_time;
		QueryPerformanceCounter(&frame_end_time);
		delta_time = (float)(frame_end_time.QuadPart - frame_begin_time.QuadPart) / performance_frequency;
		frame_begin_time = frame_end_time;
	}
	return 0;
};