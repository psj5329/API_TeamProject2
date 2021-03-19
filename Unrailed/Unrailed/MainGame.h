#pragma once

class Image;
class LoadingScene;
class MainGame
{
	Image* mBackBuffer;
	LoadingScene* mLoadingScene;
	IWICImagingFactory* wicFactory;

public:
	void Init();
	void Release();
	void Update();
	void Render(HDC hdc);
	
public:
	void RenderTime(HDC hdc);
	void AddScene();
	void LoadResources(LoadingScene* scene);

	void LoadImageResources(LoadingScene* scene);
	void LoadSoundResources(LoadingScene* scene);

	LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
};

