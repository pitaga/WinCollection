#include "WtlCDC.h"
#include "WtlRect.h"
#include <GdiPlus.h>
#include "CGdiPlusBitmap.h"
#include "resource.h"

#pragma comment(lib, "gdiplus.lib")

#define IDC_BUTTON	10
#define IDC_EDIT	11


WNDPROC BTNProc;
WNDPROC EDTProc;


LRESULT CALLBACK BTProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		HRGN hrgn = CreateRoundRectRgn(0, 0, 200, 25, 15, 15);
		SelectClipRgn(hdc, hrgn);

		HBRUSH brush = CreateSolidBrush(RGB(60, 60, 60));
		SelectObject(hdc, brush);
		Rectangle(hdc, 0, 0, 200, 25);
		DeleteObject(brush);

		ReleaseDC(hWnd, hdc);
		EndPaint(hWnd, &ps);
		break;
	}

	}

	return CallWindowProc(BTNProc, hWnd, Msg, wParam, lParam);
}



LRESULT CALLBACK DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	ULONG_PTR token = {};

	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		Gdiplus::GdiplusStartupInput start;
		Gdiplus::GdiplusStartup(&token, &start, NULL);

		// WS_EX_LAYERED	WS_EX_TRANSPARENT
		CreateWindow(WC_EDIT, L"用户名", WS_CHILD | WS_VISIBLE | WS_EX_LAYERED, 100, 200, 200, 25, hwnd, (HMENU)IDC_EDIT, NULL, NULL);

		HWND hButtonWnd = CreateWindow(WC_BUTTON, L"按钮", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 100, 100, 200, 25, hwnd, (HMENU)IDC_BUTTON, NULL, NULL);
		BTNProc = (WNDPROC)SetWindowLongW(hButtonWnd, GWL_WNDPROC, (LPARAM)(WNDPROC)BTProc);

		ShowWindow(hwnd, SW_SHOW);
		break;
	}

	case WM_CLOSE:
	{
		Gdiplus::GdiplusShutdown(token);
		PostQuitMessage(0);
		break;
	}

	case WM_COMMAND:
	{
		if (HIWORD(wParam) == EN_CHANGE && LOWORD(wParam) == IDC_EDIT) {
			RECT rect = { 100, 200, 300, 225 };
			InvalidateRect(hwnd, &rect, FALSE);
		}
		if (LOWORD(wParam) == IDC_BUTTON) {
			MessageBoxA(NULL, "Button Pressed", "", 0);
		}
		break;
	}

	case WM_CTLCOLOREDIT:
	{
		HDC hdc = (HDC)wParam;
		SetTextColor(hdc, RGB(0, 0, 0));
		SetBkMode(hdc, TRANSPARENT);
		return (LRESULT)GetStockObject(NULL_BRUSH);
	}

	case WM_PAINT:
	{
		CRect rect;
		GetClientRect(hwnd, &rect);

		Gdiplus::Bitmap* memory = new Gdiplus::Bitmap(rect.Width(), rect.Height());
		Gdiplus::Graphics* canvas = Gdiplus::Graphics::FromImage(memory);

		Gdiplus::Image image(L"back.jpg");
		canvas->DrawImage(&image, 0, 0, rect.Width(), rect.Height());

		CPaintDC dc(hwnd);
		Gdiplus::Graphics* graphics = new Gdiplus::Graphics(dc);
		graphics->DrawImage(memory, 0, 0);

		delete memory;
		delete canvas;

		break;
	}

	}

	return FALSE;
}



int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	HWND hwnd = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), GetDesktopWindow(), (DLGPROC)DialogProc);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

