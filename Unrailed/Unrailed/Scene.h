#pragma once
class Scene
{
public:
	virtual void Init() = 0;
	virtual void Release() = 0;
	virtual void Update() = 0;
	virtual void Render(HDC hdc) = 0;

	virtual void SetMapSize(int y,int x) {};
	virtual void OpenLoadWindow() {};
	virtual LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) { return 0; }
};

