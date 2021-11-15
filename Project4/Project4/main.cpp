#include <Windows.h>
#include <atlimage.h>

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


void TransparentPNG(CImage* image)
{
	for (int i = 0; i < image->GetWidth(); ++i)
	{
		for (int j = 0; j < image->GetHeight(); ++j)
		{
			unsigned char* color = (unsigned char*)(image->GetPixelAddress(i, j));
			color[0] = color[0] * color[3] / 255;
			color[1] = color[1] * color[3] / 255;
			color[2] = color[2] * color[3] / 255;
		}
	}
}


LRESULT CALLBACK DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		CImage image;
		image.Load(L"icon.png");
		TransparentPNG(&image);
		SendDlgItemMessage(hwnd, IDC_STATIC1, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)image.Detach());
		
		image.Load(L"icon_on.png");
		TransparentPNG(&image);
		SendDlgItemMessage(hwnd, IDC_STATIC2, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)image.Detach());
		
		return TRUE;
	}

	case WM_CLOSE:
		PostQuitMessage(0);
		return TRUE;

	}
	return FALSE;
}