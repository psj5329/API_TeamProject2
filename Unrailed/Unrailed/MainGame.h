#pragma once

class Image;
class MainGame
{
	Image* mBackBuffer;	//�ĸ����(�ǵ�������)
public:
	void Init();
	void Release();
	void Update();
	void Render(HDC hdc);
	
	void RenderTime(HDC hdc);

	LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
};

