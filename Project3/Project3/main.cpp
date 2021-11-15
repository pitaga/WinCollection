/// list control

#include <Windows.h>
#include <CommCtrl.h>
#include <tchar.h>
#include "resource.h"


LRESULT CALLBACK DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	HWND hwnd = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), GetDesktopWindow(), (DLGPROC)DialogProc);

	if (hwnd == NULL)
	{
		return 0;
	}

	ShowWindow(hwnd, SW_SHOW);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}


LRESULT CALLBACK DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		LVCOLUMN lvc;
		lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
		lvc.fmt = LVCFMT_CENTER;
		lvc.cx = 100;
		lvc.pszText = (LPWSTR)_T("Ãû³Æ");
		SendDlgItemMessage(hwnd, IDC_LIST1, LVM_INSERTCOLUMN, 0, (LPARAM)&lvc);

		LVITEM item;
		item.mask = LVCF_TEXT;
		item.iItem = 0;
		item.iSubItem = 0;
		item.pszText = (LPTSTR)_T("ÍõÎå");
		SendDlgItemMessage(hwnd, IDC_LIST1, LVM_SETITEM, 1, (LPARAM)&lvc);
		
		break;

	case WM_CLOSE:
		PostQuitMessage(0);
		break;

	default:
		break;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

