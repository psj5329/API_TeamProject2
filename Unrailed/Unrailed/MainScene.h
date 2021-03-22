#pragma once
#include "Scene.h"

class Image;
class Button;
class MainScene : public Scene
{
	Image* mBackground;
	Image* mOptionWindow;
	Button* mStartButton;
	Button* mOptionButton;
	Button* mExitButton;
	Button* mXButton;
	bool mIsShowWindow;

public:
	void Init()override;
	void Release()override;
	void Update()override;
	void Render(HDC hdc)override;

public:
	void StartButton();
	void OptionButton();
	void ExitButton();
	void XButton();
};

