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

	Image* mImage;
	Animation* mAnimation;

public:
	void AddLoadFunc(const function<void(void)>& func);

	void Init()override;
	void Release()override;
	void Update()override;
	void Render(HDC hdc)override;

	bool GetIsEndLoading()const { return mIsEndLoading; }
};

