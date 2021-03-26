#pragma once

class Button;
class Scene
{
protected:
	Image* mPauseWindow;
	Image* mPauseOptionWindow;
	Button* mContinueButton;
	Button* mOptionButton;
	Button* mMainButton;
	Button* mXButton;
	bool mIsPause;
	bool mIsOption;

	// 사운드 조절 옵션
	Button* mVolumeEffectBar;
	Button* mVolumeEffectButton;
	Button* mVolumeBackgroundBar;
	Button* mVolumeBackgroundButton;
	static float mVolumeEffect;
	static float mVolumeBackground;

public:
	virtual void Init() = 0;
	virtual void Release() = 0;
	virtual void Update() = 0;
	virtual void Render(HDC hdc) = 0;

	virtual void SetMapSize(int y,int x) {};
	virtual void OpenLoadWindow() {};
	virtual LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) { return 0; }

public:
	void WindowInit();
	void ContinueButton();
	void OptionButton();
	void MainButton();
	void XButton();
	void VolumeEffectButton();
	void VolumeEffectBar();
	void VolumeBackgroundButton();
	void VolumeBackgroundBar();
};

