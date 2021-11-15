#include "WtlCDC.h"
#include "CGdiPlusBitmap.h"
#include "CEditControl.h"
#include "resource.h"

#pragma comment(lib, "gdiplus.lib")

HINSTANCE instance;
TCHAR _String[1025] = _T("");
int _StringPosition = _tcslen(_String);

CEditControl* edit;



void UpdateDialog(HWND hwnd)
{
	if (hwnd != NULL) {
		RECT rect;
		GetClientRect(hwnd, &rect);
		InvalidateRect(hwnd, &rect, FALSE);
		UpdateWindow(hwnd);
	}
}


void OnPaint(HWND hwnd)
{
	if (hwnd == NULL) return;

	CPaintDC dc(hwnd);
	CRect rtClient;
	GetClientRect(hwnd, &rtClient);

	Gdiplus::Bitmap* pMemoryBMap = new Gdiplus::Bitmap(rtClient.Width(), rtClient.Height());
	Gdiplus::Graphics* pCanvas = Gdiplus::Graphics::FromImage(pMemoryBMap);

	Gdiplus::Image image(L"back.jpg");
	pCanvas->DrawImage(&image, 0, 0, rtClient.Width(), rtClient.Height());

	edit->Draw(*pCanvas);

	Gdiplus::Graphics* pGraphics = new Gdiplus::Graphics(dc);
	pGraphics->DrawImage(pMemoryBMap, 0, 0);

	delete pMemoryBMap;
	delete pCanvas;
}


LRESULT CALLBACK DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	ULONG_PTR m_gdiplusToken = {};
	Gdiplus::GdiplusStartupInput StartupInput;

	switch (uMsg)
	{
	case WM_INITDIALOG:
	{	
		GdiplusStartup(&m_gdiplusToken, &StartupInput, NULL);
		ShowWindow(hwnd, SW_MAXIMIZE);
		break;
	}

	case WM_PAINT:
	{
		OnPaint(hwnd);
		break;
	}

	case WM_CLOSE:
	{
		Gdiplus::GdiplusShutdown(m_gdiplusToken);
		PostQuitMessage(0);
		break;
	}

	}

	if (edit) {
		edit->HandleEvent(hwnd, uMsg, wParam, lParam);
	}

	return FALSE;
}


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	edit = new CEditControl(Rect(100, 100, 300, 25), L"请输入用户名", instance);

	HWND hwnd = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), GetDesktopWindow(), (DLGPROC)DialogProc);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}
