#pragma once
#include "Scene.h"
#include <functional>

class Image;
class Animation;
class LoadingScene : public Scene
{
	vector<function<void(void)>> mLoadList;
	int mLoadIndex;
	bool mIsEndLoading;

	Image* mLoadingBarImage;
	Image* mImage;
	Animation* mAnimation;
	int mFontColor;
	bool mIsColorChange;

public:
	void AddLoadFunc(const function<void(void)>& func);

	void Init()override;
	void Release()override;
	void Update()override;
	void Render(HDC hdc)override;

public:
	void LoadingImageThread();

public:
	bool GetIsEndLoading()const { return mIsEndLoading; }
};

