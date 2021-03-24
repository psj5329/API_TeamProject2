#include "pch.h"
#include "MainScene.h"

#include "Image.h"
#include "Button.h"

void MainScene::Init()
{
	mBackground = IMAGEMANAGER->FindImage(L"MainScene");
	mOptionWindow = IMAGEMANAGER->FindImage(L"OptionWindow");
	Image* StartButton = IMAGEMANAGER->FindImage(L"StartButton");
	Image* OptionButton = IMAGEMANAGER->FindImage(L"OptionButton");
	Image* ExitButton = IMAGEMANAGER->FindImage(L"ExitButton");
	Image* XButton = IMAGEMANAGER->FindImage(L"XButton");

	mStartButton = new Button(StartButton, WINSIZEX / 2, WINSIZEY / 2, StartButton->GetWidth(), StartButton->GetHeight(), bind(&MainScene::StartButton, this));
	mOptionButton = new Button(OptionButton, WINSIZEX / 2, WINSIZEY / 2 + 120, OptionButton->GetWidth(), OptionButton->GetHeight(), bind(&MainScene::OptionButton, this));
	mExitButton = new Button(ExitButton, WINSIZEX / 2, WINSIZEY / 2 + 240, ExitButton->GetWidth(), ExitButton->GetHeight(), bind(&MainScene::ExitButton, this));
	mXButton = new Button(XButton, WINSIZEX / 2 + 460, 75, XButton->GetWidth(), XButton->GetHeight(), bind(&MainScene::XButton, this));

	mIsShowWindow = false;
}

void MainScene::Release()
{
	SafeDelete(mStartButton);
	SafeDelete(mOptionButton);
	SafeDelete(mExitButton);
	SafeDelete(mXButton);
}

void MainScene::Update()
{
	if (!mIsShowWindow)
	{
		if (mStartButton != nullptr)
			mStartButton->Update();
		if (mOptionButton != nullptr)
			mOptionButton->Update();
		if (mExitButton != nullptr)
			mExitButton->Update();
	}
	else
	{
		if (mXButton != nullptr)
			mXButton->Update();
	}
}

void MainScene::Render(HDC hdc)
{
	mBackground->ScaleRender(hdc, 0, 0, mBackground->GetWidth(), mBackground->GetHeight());
	mStartButton->Render(hdc);
	mOptionButton->Render(hdc);
	mExitButton->Render(hdc);

	if (mIsShowWindow)
	{
		mOptionWindow->ScaleRender(hdc, 140, 35, mOptionWindow->GetWidth(), mOptionWindow->GetHeight());
		mXButton->Render(hdc);
	}
}

void MainScene::StartButton()
{
	// 우선은 씬 넘어가자
	// 나중에 창 띄워서 버트ㅡㄴ 할거얌
	SCENEMANAGER->LoadScene(L"Scene4");
}

void MainScene::OptionButton()
{
	if (!mIsShowWindow)
		mIsShowWindow = true;
}

void MainScene::ExitButton()
{
	PostQuitMessage(0);
}

void MainScene::XButton()
{
	if (mIsShowWindow)
		mIsShowWindow = false;
}
