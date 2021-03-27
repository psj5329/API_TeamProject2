#pragma once
#include "Scene.h"

class Button;

struct NumberPad
{
	class Image* image;
	int frameX;
	int frameY;
	RECT rect;
	int sizeX;
	int sizeY;
};

class MapToolReady :
	public Scene
{
	class Image* mBackgroundImg;
	RECT mBackground;
	class Image* mSetArrow;
	RECT mArrow;

	int mYTileCount;
	int mXTileCount;

	int mYTens;
	int mYOnes;
	int mXTens;
	int mXOnes;

	int mCounter;

	wstring mInputX;
	wstring mInputY;
	RECT mInputBoxX;
	RECT mInputBoxY;

	Button* mSaveButton;
	Button* mLoadButton;

	vector<NumberPad> mNumberPad;

	bool mCurrentInput; //true ¸é YÀÔ·Â
	bool mFinish;
public:
	void Init()override;
	void Release()override;
	void Update()override;
	void Render(HDC hdc)override;
	void SetCurrentInput();
	void SetTileCounts();

	int GetYTileCount() { return mYTileCount; }
	int GetXTileCOunt() { return mXTileCount; }

	LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)override;

	void CreateNew();
	void Load();

private:
	void RenderBackBackground(HDC hdc);
};

