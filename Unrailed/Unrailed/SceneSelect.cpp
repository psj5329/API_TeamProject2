#include "pch.h"
#include "SceneSelect.h"

#include "Image.h"
#include "Button.h"

void SceneSelect::Init()
{
	mBackground = IMAGEMANAGER->FindImage(L"MainScene");
	mStageRound = IMAGEMANAGER->FindImage(L"SceneSelect1");
	Image* StageSelectButton = IMAGEMANAGER->FindImage(L"StartButton");
	Image* LeftButton = IMAGEMANAGER->FindImage(L"Left");
	Image* RightButton = IMAGEMANAGER->FindImage(L"Right");
	Image* BackButton = IMAGEMANAGER->FindImage(L"XButton");

	mStageSelectButton = new Button(StageSelectButton, WINSIZEX / 2, WINSIZEY / 2 + 260, StageSelectButton->GetWidth(), StageSelectButton->GetHeight(), bind(&SceneSelect::StageSelectButton, this));
	mLeftButton = new Button(LeftButton, WINSIZEX / 2 - 540, WINSIZEY / 2 - 60, LeftButton->GetWidth(), LeftButton->GetHeight(), bind(&SceneSelect::LeftButton, this));
	mRightButton = new Button(RightButton, WINSIZEX / 2 + 540, WINSIZEY / 2 - 60, RightButton->GetWidth(), RightButton->GetHeight(), bind(&SceneSelect::RightButton, this));
	mBackButton = new Button(BackButton, WINSIZEX / 2 + 590, WINSIZEY / 2 - 310, BackButton->GetWidth(), BackButton->GetHeight(), bind(&SceneSelect::BackButton, this));

	mRound = 1;
	mMaxRound = 5;
}

void SceneSelect::Release()
{
	SafeDelete(mLeftButton);
	SafeDelete(mRightButton);
}

void SceneSelect::Update()
{
	if (mStageSelectButton != nullptr)
		mStageSelectButton->Update();
	if (mLeftButton != nullptr)
		mLeftButton->Update();
	if (mRightButton != nullptr)
		mRightButton->Update();
	if (mBackButton != nullptr)
		mBackButton->Update();
}

void SceneSelect::Render(HDC hdc)
{
	mBackground->ScaleRender(hdc, 0, 0, mBackground->GetWidth(), mBackground->GetHeight());
	mStageRound->ScaleRender(hdc, 300, 50, mStageRound->GetWidth(), mStageRound->GetHeight());
	mStageSelectButton->ScaleRender(hdc, 0.9f, 0.9f);
	mLeftButton->ScaleRender(hdc, 0.9f, 0.9f);
	mRightButton->ScaleRender(hdc, 0.9f, 0.9f);
	mBackButton->ScaleRender(hdc, 0.9f, 0.9f);
}

void SceneSelect::StageSelectButton()
{
	wstring name = L"Stage" + to_wstring(mRound);
	SCENEMANAGER->LoadScene(name);
}

void SceneSelect::LeftButton()
{
	mRound--;
	if (mRound <= 0)
		mRound = mMaxRound;
	wstring round = L"SceneSelect" + to_wstring(mRound);
	mStageRound = IMAGEMANAGER->FindImage(round);
}

void SceneSelect::RightButton()
{
	mRound++;
	if (mRound > mMaxRound)
		mRound = 1;
	wstring round = L"SceneSelect" + to_wstring(mRound);
	mStageRound = IMAGEMANAGER->FindImage(round);
}

void SceneSelect::BackButton()
{
	SCENEMANAGER->LoadScene(L"MainScene");
}
