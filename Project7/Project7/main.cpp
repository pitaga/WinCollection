#include <Windows.h>
#include <stdio.h>


int clientX;
int clientY;


void DrawRectangle(HWND hwnd)
{
	HBRUSH brush;
	HDC hdc;
	RECT rect;

	if (clientX == 0 || clientY == 0)
		return;

	SetRect(&rect, rand() % clientX, rand() % clientY, rand() % clientX, rand() % clientY);
	
	brush = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
	hdc = GetDC(hwnd);
	FillRect(hdc, &rect, brush);
	ReleaseDC(hwnd, hdc);
	DeleteObject(brush);
}


LRESULT CALLBACK Proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_SIZE:
		clientX = LOWORD(lParam);
		clientY = HIWORD(lParam);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}


int WINAPI WinMain(HINSTANCE instance, HINSTANCE preInstance, PSTR commandLine, int commandShow)
{
	static TCHAR appName[] = TEXT("RandRect");
	HWND hwnd;
	MSG msg;
	WNDCLASS wnd;

	wnd.style = CS_HREDRAW | CS_VREDRAW;
	wnd.lpfnWndProc = Proc;
	wnd.cbClsExtra = 0;
	wnd.cbWndExtra = 0;
	wnd.hInstance = instance;
	wnd.hbrBackground = (HBRUSH)GetStockObject(WHITENESS);
	wnd.lpszMenuName = NULL;
	wnd.lpszClassName = appName;
	wnd.hIcon = NULL;
	wnd.hCursor = NULL;

	if (!RegisterClass(&wnd)) {
		MessageBoxA(NULL, "failed create window", "", 0);
		return 0;
	}

	hwnd = CreateWindow(appName, TEXT("random rectangle"), WS_OVERLAPPED,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, instance, NULL
	);

	ShowWindow(hwnd, SW_MAXIMIZE);
	UpdateWindow(hwnd);

	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			DrawRectangle(hwnd);
		}
	}

	return msg.wParam;
}