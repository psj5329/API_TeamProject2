#include "pch.h"
#include "LoadingScene.h"

#include "Image.h"
#include "Animation.h"

void LoadingScene::AddLoadFunc(const function<void(void)>& func)
{
	mLoadList.push_back(func);
}

void LoadingScene::Init()
{
	IMAGEMANAGER->LoadFromFile(L"LoadingTrain", Resources(L"LoadingTrain"), 850, 70, 5, 1, true);
	mImage = IMAGEMANAGER->FindImage(L"LoadingTrain");

	mAnimation = new Animation;
	mAnimation->InitFrameByStartEnd(0, 0, 5, 0, false);
	mAnimation->SetIsLoop(true);
	mAnimation->SetFrameUpdateTime(0.2f);

	mAnimation->Play();

	mLoadIndex = 0;
	mIsEndLoading = false;
}

void LoadingScene::Release()
{
}

void LoadingScene::Update()
{
	if (mIsEndLoading)
	{
		if (INPUT->GetKeyDown(VK_RETURN))
			SCENEMANAGER->LoadScene(L"Scene2");
	}

	if (mLoadIndex >= mLoadList.size())
	{
		mIsEndLoading = true;
		return;
	}

	function<void(void)> func = mLoadList[mLoadIndex];
	func();
	mLoadIndex++;

	mAnimation->Update();
}

void LoadingScene::Render(HDC hdc)
{
	mImage->ScaleFrameRender(hdc, WINSIZEX / 2 - mImage->GetWidth() / 10, WINSIZEY / 2, mAnimation->GetNowFrameX(), 0, mImage->GetWidth() / 5, mImage->GetHeight());

	if (mIsEndLoading)
	{
		wstring strScene = L"로딩 끝";
		TextOut(hdc, WINSIZEX / 2 - 15, WINSIZEY / 2, strScene.c_str(), strScene.length());
	}
	else
	{
		wstring strScene = L"로딩중..";
		TextOut(hdc, WINSIZEX / 2 - 15, WINSIZEY / 2, strScene.c_str(), strScene.length());
	}
}
