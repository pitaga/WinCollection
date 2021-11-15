#include <Windows.h>
#include <CommCtrl.h>
#include "resource.h"

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


LRESULT CALLBACK ETProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
	case WM_KEYUP:
	{
		if (wParam == VK_DELETE || wParam == VK_BACK) {
			ShowWindow(hWnd, SW_HIDE);
			ShowWindow(hWnd, SW_SHOW);
			SetFocus(hWnd);
		}
		break;
	}

	}

	return CallWindowProc(EDTProc, hWnd, Msg, wParam, lParam);
}



LRESULT CALLBACK DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		HWND hButtonWnd = CreateWindow(WC_BUTTON, L"按钮", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 100, 100, 200, 25, hwnd, (HMENU)IDC_BUTTON, NULL, NULL);
		HWND hEditWnd = CreateWindow(WC_EDIT, L"用户名", WS_CHILD | WS_VISIBLE | WS_EX_LAYERED, 100, 200, 200, 25, hwnd, (HMENU)IDC_EDIT, NULL, NULL);
		
		// WS_EX_LAYERED		用户名
		// WS_EX_TRANSPARENT	密码

		BTNProc = (WNDPROC)SetWindowLongW(hButtonWnd, GWL_WNDPROC, (LPARAM)(WNDPROC)BTProc);
		EDTProc = (WNDPROC)SetWindowLongW(hEditWnd, GWL_WNDPROC, (LPARAM)(WNDPROC)ETProc);

		ShowWindow(hwnd, SW_SHOW);
		break;
	}

	case WM_CLOSE:
	{
		PostQuitMessage(0);
		break;
	}

	case WM_COMMAND:
	{
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

