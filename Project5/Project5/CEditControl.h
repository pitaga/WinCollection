#pragma once
#include "WtlRect.h"
#include "WtlCDC.h"
#include "WtlString.h"
#include <gdiplus.h>

using namespace Gdiplus;


class CEditControl
{
public:
	CEditControl(const Rect& position, const CString& defaultContent, HINSTANCE instance);
	~CEditControl();

	void HandleEvent(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	void Draw(Graphics& graphics);

private:
	void OnChar(WPARAM wParam);
	void OnKeyDown(HWND hwnd, WPARAM wParam);
	void OnKeyUp(HWND hwnd);
	void OnLButtonDown(HWND hwnd, LPARAM lParam);
	void Refresh(HWND hwnd);
	const RectF& GetTextBoundsRect(const Font& font, const StringFormat& format, const CString& content);

private:
	bool mIsActive = false;
	int mCursorPosition = 0;
	HINSTANCE mInstance = NULL;
	ULONG_PTR mToken = {};
	CString mContent = L"";
	CString mDefaultContent = L"";
	Rect mPosition;

};

