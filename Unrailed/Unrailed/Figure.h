#pragma once

inline RECT RectMake(int x, int y, int width, int height)
{
	return { x,y,x + width, y + height };
}

inline RECT RectMakeCenter(int x, int y, int width, int height)
{
	return { x - width / 2, y - height / 2, x + width / 2, y + height / 2 };
}

inline RECT RectMakeBottom(int x, int y, int width, int height)
{
	RECT result;
	result.left = x - width / 2;
	result.right = x + width / 2; 
	result.top = y - height;
	result.bottom = y;
	return result;
}

inline void RenderRect(HDC hdc, RECT rc)
{
	Rectangle(hdc, rc.left,rc.top,rc.right,rc.bottom);
}

inline void RenderEllipse(HDC hdc, RECT rc)
{
	Ellipse(hdc, rc.left, rc.top, rc.right, rc.bottom);
}

inline void RenderEllipse(HDC hdc,int x, int y, int radius)
{
	Ellipse(hdc, x - radius, y - radius, x + radius, y + radius);
}

inline void RenderLine(HDC hdc,int startX, int startY, int endX, int endY)
{
	MoveToEx(hdc, startX, startY, NULL);
	LineTo(hdc, endX, endY);
}

inline float GetDistance(int aX, int aY, int bX, int bY)
{
	float width = aX - bX;
	float height = aY - bY;

	return sqrtf(width * width + height * height);
}

inline bool IntersectRectToCircle(RECT rc, int x, int y, int radius)
{
	if ((rc.left <= x && x <= rc.right) ||
		(rc.top <= y && y <= rc.bottom))
	{
		RECT newRect = rc;
		newRect.left -= radius;
		newRect.right += radius;
		newRect.top -= radius;
		newRect.bottom += radius;

		if ((newRect.left <= x && x <= newRect.right) &&
			(newRect.top <= y && y <= newRect.bottom))
		{
			return true;
		}
	}
	
	if (radius >= GetDistance(x, y, rc.left, rc.top))return true;
	if (radius >= GetDistance(x, y, rc.right, rc.top))return true;
	if (radius >= GetDistance(x, y, rc.left, rc.bottom))return true;
	if (radius >= GetDistance(x, y, rc.right, rc.bottom))return true;

	return false;
}