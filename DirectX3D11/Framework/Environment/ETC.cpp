#include "Framework.h"

ETC::ETC()
{
}

ETC::~ETC()
{
}

void ETC::Update()
{
	if (KEY_DOWN('C')) CursorSwitch();

	if (KEY_DOWN('V'))
	{
		isBindCursor = !isBindCursor;
		if (isBindCursor) BindCursor();
	}

	if (isBindCursor) ReplaceCursor();
}

void ETC::CursorSwitch()
{
	isShowCursor = !isShowCursor;
	if (isShowCursor) ShowCursor(true);
	else ShowCursor(false);
}

RECT ETC::BindCursor()
{
	RECT rect;
	POINT p1, p2;
	GetClientRect(hWnd, &rect);
	p1.x = rect.left;
	p1.y = rect.top;
	p2.x = rect.right;
	p2.y = rect.bottom;
	ClientToScreen(hWnd, &p1);
	ClientToScreen(hWnd, &p2);
	rect.left = p1.x;
	rect.top = p1.y;
	rect.right = p2.x;
	rect.bottom = p2.y;
	ClipCursor(&rect);

	return rect;
}

void ETC::ReplaceCursor()
{
	if (mousePos.x >= WIN_WIDTH - 1 || mousePos.x <= 0 ||
		mousePos.y >= WIN_HEIGHT - 1 || mousePos.y <= 0)
	{
		RECT rect = BindCursor();
		SetCursorPos(rect.right - CENTER_X, rect.bottom - CENTER_Y);
		isSkipMouseDelta = true;
	}
	else
	{
		isSkipMouseDelta = false;
	}
}
