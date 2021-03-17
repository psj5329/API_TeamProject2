#pragma once

class Image;
class LoadingScene;
class MainGame
{
	Image* mBackBuffer;
	LoadingScene* mLoadingScene;

public:
	void Init();
	void Release();
	void Update();
	void Render(HDC hdc);
	
public:
	void RenderTime(HDC hdc);
	void AddScene();
	void LoadResources(LoadingScene* scene);

	LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
};

