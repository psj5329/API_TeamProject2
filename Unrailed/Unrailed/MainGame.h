#pragma once

class Image;
class MainGame
{
	Image* mBackBuffer;	//후면버퍼(건들지마라)
public:
	void Init();
	void Release();
	void Update();
	void Render(HDC hdc);
	
	void RenderTime(HDC hdc);

	LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
};

