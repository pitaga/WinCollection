#include "CEditControl.h"
#include "CGdiPlusBitmap.h"
#include "resource.h"


CEditControl::CEditControl(const Rect& position, const CString& defaultContent, HINSTANCE instance)
	: mInstance(instance)
	, mPosition(position)
	, mDefaultContent(defaultContent)
{
	GdiplusStartupInput startupInput;
	GdiplusStartup(&mToken, &startupInput, NULL);
}


CEditControl::~CEditControl()
{
	GdiplusShutdown(mToken);
}


void CEditControl::HandleEvent(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CHAR:
	{
		OnChar(wParam);
		break;
	}

	case WM_KEYDOWN:
	{
		OnKeyDown(hwnd, wParam);
		break;
	}

	case WM_KEYUP:
	{
		OnKeyUp(hwnd);
		break;
	}

	case WM_LBUTTONDOWN:
	{
		OnLButtonDown(hwnd, lParam);
		break;
	}

	}
}


void CEditControl::Draw(Graphics& graphics)
{
	constexpr int padding = 5;

	// 文字样式
	FontFamily fontFamily(L"Times New Roman");
	Font font(&fontFamily, 20, FontStyleRegular, UnitPixel);
	StringFormat stringFormat;
	stringFormat.SetLineAlignment(StringAlignmentCenter);

	// 绘制输入框内容
	RectF rect = RectF(mPosition.X + padding, mPosition.Y, mPosition.Width - padding * 2, mPosition.Height);
	CString content = mIsActive || !mContent.IsEmpty() ? mContent : mDefaultContent;
	SolidBrush solidBrush(Color(200, 0, 0, 0));
	graphics.DrawString(content, -1, &font, rect, &stringFormat, &solidBrush);

	// 绘制输入时的游标
	CGdiPlusBitmapResource resource;
	if (mIsActive && resource.Load(IDB_PNG_CURSOR, L"PNG", mInstance)) {
		Bitmap* bitmap = (Bitmap*)resource;
		RectF rtContent = GetTextBoundsRect(font, stringFormat, content.Left(mCursorPosition));
		graphics.DrawImage(bitmap, RectF(rect.X + rtContent.Width + padding / 2, rect.Y, 4, rect.Height));
	}

	// 绘制输入框前的图标
	constexpr int size = 28;
	if (resource.Load(IDB_PNG1, L"PNG", mInstance)) {
		Bitmap* bitmap = (Bitmap*)resource;
		graphics.DrawImage(bitmap, RectF(mPosition.X - size, mPosition.Y, size, size));
	}

	// 绘制文字下面的线
	Pen* pen = new Pen(Color(250, 129, 190, 235), 2);
	graphics.DrawLine(pen, PointF(mPosition.X, mPosition.Y + mPosition.Height), PointF(mPosition.X + mPosition.Width, mPosition.Y + mPosition.Height));
	delete pen;
}


void CEditControl::OnChar(WPARAM wParam)
{
	TCHAR code = (TCHAR)wParam;
	if (code > (TCHAR)' ') {
		mContent.Insert(mCursorPosition++, code);
	}
}


void CEditControl::OnKeyDown(HWND hwnd, WPARAM wParam)
{
	switch (wParam)
	{
	case VK_LEFT:
		if (mCursorPosition > 0)
			mCursorPosition--;
		break;

	case VK_RIGHT:
		if (mCursorPosition < mContent.GetLength())
			mCursorPosition++;
		break;

	case VK_END:
		mCursorPosition = mContent.GetLength();
		break;

	case VK_BACK:
		if (mCursorPosition > 0) {
			mContent.Delete(mCursorPosition - 1);
			mCursorPosition--;
		}
		break;

	case VK_DELETE:
		if (mCursorPosition < mContent.GetLength()) {
			mContent.Delete(mCursorPosition);
		}
		break;

	}
	Refresh(hwnd);
}


void CEditControl::OnKeyUp(HWND hwnd)
{
	Refresh(hwnd);
}


void CEditControl::OnLButtonDown(HWND hwnd, LPARAM lParam)
{
	mIsActive = mPosition.Contains(Point(LOWORD(lParam), HIWORD(lParam)));
	Refresh(hwnd);
}


void CEditControl::Refresh(HWND hwnd)
{
	if (hwnd)
	{
		RECT rect;
		GetClientRect(hwnd, &rect);
		InvalidateRect(hwnd, &rect, FALSE);
		UpdateWindow(hwnd);
	}
}


const RectF& CEditControl::GetTextBoundsRect(const Font& font, const StringFormat& format, const CString& content)
{
	GraphicsPath graphics;
	FontFamily fontFamily;
	font.GetFamily(&fontFamily);
	graphics.AddString(content, -1, &fontFamily, font.GetStyle(), font.GetSize(), PointF(0, 0), &format);

	RectF rect;
	graphics.GetBounds(&rect);
	return rect;
}

