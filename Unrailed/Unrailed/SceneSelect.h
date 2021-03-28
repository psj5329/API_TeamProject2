#pragma once
#include "Scene.h"

class Image;
class Button;
class SceneSelect : public Scene
{
	Image* mBackground;
	Image* mStageRound;
	Button* mStageSelectButton;
	Button* mLeftButton;
	Button* mRightButton;
	Button* mBackButton;
	int mRound;		// 몇스테인지
	int mMaxRound;

public:
	void Init()override;
	void Release()override;
	void Update()override;
	void Render(HDC hdc)override;

public:
	void StageSelectButton();
	void LeftButton();
	void RightButton();
	void BackButton();
};

