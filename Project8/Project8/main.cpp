#include <Windows.h>
#include <math.h>

#define TWO_PI (2.0 * 3.14159)


LRESULT CALLBACK Proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HRGN hRgnClip;
	static int clientX, clientY;
	double angle, radius;
	HDC hdc;
	HRGN hRgnTemp[6];
	int i;
	PAINTSTRUCT ps;

	switch (message)
	{
	case WM_SIZE:
		clientX = LOWORD(lParam);
		clientY = HIWORD(lParam);

		if (hRgnClip) {
			DeleteObject(hRgnClip);
		}

		hRgnTemp[0] = CreateEllipticRgn(0, clientY / 3, clientX / 2, 2 * clientY / 3);
		hRgnTemp[1] = CreateEllipticRgn(clientX / 2, clientY / 3, clientX, 2 * clientY / 3);
		hRgnTemp[2] = CreateEllipticRgn(clientX / 3, 0, 2 * clientX / 3, clientY / 2);
		hRgnTemp[3] = CreateEllipticRgn(clientX / 3, clientY / 2, 2 * clientX / 3, clientY);
		hRgnTemp[4] = CreateEllipticRgn(0, 0, 1, 1);
		hRgnTemp[5] = CreateEllipticRgn(0, 0, 1, 1);
		hRgnClip = CreateEllipticRgn(0, 0, 1, 1);

		CombineRgn(hRgnTemp[4], hRgnTemp[0], hRgnTemp[1], RGN_OR);
		CombineRgn(hRgnTemp[5], hRgnTemp[2], hRgnTemp[3], RGN_OR);
		CombineRgn(hRgnClip, hRgnTemp[4], hRgnTemp[5], RGN_XOR);
		for (int i = 0; i < 6; ++i) {
			DeleteObject(hRgnTemp[i]);
		}

		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		SetViewportOrgEx(hdc, clientX / 2.0, clientY / 2.0, NULL);
		SelectClipRgn(hdc, hRgnClip);
		radius = _hypot(clientX / 2.0, clientY / 2.0);
		for (angle = 0.0; angle < TWO_PI; angle += TWO_PI / 360) {
			MoveToEx(hdc, 0, 0, NULL);
			LineTo(hdc, (int)(radius * cos(angle) + 0.5), (int)(radius * sin(angle) + 0.5));
		}
		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		DeleteObject(hRgnClip);
		PostQuitMessage(0);
		return 0;

	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}


int WINAPI WinMain(HINSTANCE instance, HINSTANCE preInstance, PSTR commandLine, int commandShow)
{
	static TCHAR appName[] = TEXT("clover");
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

	hwnd = CreateWindow(appName, TEXT("draw a clover"), WS_OVERLAPPED,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, instance, NULL
	);

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
	
}

