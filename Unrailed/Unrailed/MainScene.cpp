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
	Image* VolumeButton = IMAGEMANAGER->FindImage(L"VolumeButton");
	Image* VolumeBar = IMAGEMANAGER->FindImage(L"VolumeBar");

	mStartButton = new Button(StartButton, WINSIZEX / 2, WINSIZEY / 2, StartButton->GetWidth(), StartButton->GetHeight(), bind(&MainScene::StartButton, this));
	mOptionButton = new Button(OptionButton, WINSIZEX / 2, WINSIZEY / 2 + 120, OptionButton->GetWidth(), OptionButton->GetHeight(), bind(&MainScene::OptionButton, this));
	mExitButton = new Button(ExitButton, WINSIZEX / 2, WINSIZEY / 2 + 240, ExitButton->GetWidth(), ExitButton->GetHeight(), bind(&MainScene::ExitButton, this));
	mXButton = new Button(XButton, WINSIZEX / 2 + 460, 75, XButton->GetWidth(), XButton->GetHeight(), bind(&MainScene::XButton, this));
	mVolumeEffectBar = new Button(VolumeBar, WINSIZEX / 2 + 120, WINSIZEY / 2 + 60, VolumeBar->GetWidth() * 2, VolumeBar->GetHeight() * 2, bind(&Scene::VolumeEffectBar, this));
	mVolumeEffectButton = new Button(VolumeButton, WINSIZEX / 2 + 120, WINSIZEY / 2 + 60, VolumeButton->GetWidth() * 2, VolumeButton->GetHeight() * 2, bind(&Scene::VolumeEffectButton, this));
	mVolumeBackgroundBar = new Button(VolumeBar, WINSIZEX / 2 + 120, WINSIZEY / 2 - 90, VolumeBar->GetWidth() * 2, VolumeBar->GetHeight() * 2, bind(&Scene::VolumeBackgroundBar, this));
	mVolumeBackgroundButton = new Button(VolumeButton, WINSIZEX / 2 + 120, WINSIZEY / 2 - 90, VolumeButton->GetWidth() * 2, VolumeButton->GetHeight() * 2, bind(&Scene::VolumeBackgroundButton, this));

	mIsShowWindow = false;

	SOUNDMANAGER->Play(L"Roll Mart", 0.05f);
}

void MainScene::Release()
{
	SafeDelete(mStartButton);
	SafeDelete(mOptionButton);
	SafeDelete(mExitButton);
	SafeDelete(mXButton);
	SOUNDMANAGER->Stop(L"Roll Mart");
}

void MainScene::Update()
{
	SOUNDMANAGER->Update();
	if (SOUNDMANAGER->GetPosition(L"Roll Mart") >= SOUNDMANAGER->GetWholePosition(L"Roll Mart"))
	{
		SOUNDMANAGER->Stop(L"Roll Mart");
		SOUNDMANAGER->Play(L"Roll Mart", 0.05f);
	}

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
		if (mVolumeEffectBar != nullptr)
		{
			mVolumeEffectBar->Update();
			mVolumeEffectButton->Update();
		}
		if (mVolumeEffectButton != nullptr)
			mVolumeEffectButton->Update();
		if (mVolumeBackgroundBar != nullptr)
		{
			mVolumeBackgroundBar->Update();
			mVolumeBackgroundButton->Update();
		}
		if (mVolumeBackgroundButton != nullptr)
			mVolumeBackgroundButton->Update();
	}
}

void MainScene::Render(HDC hdc)
{
	mBackground->ScaleRender(hdc, 0, 0, mBackground->GetWidth(), mBackground->GetHeight());
	mStartButton->ScaleRender(hdc, 0.9f, 0.9f);
	mOptionButton->ScaleRender(hdc, 0.9f, 0.9f);
	mExitButton->ScaleRender(hdc, 0.9f, 0.9f);

	if (mIsShowWindow)
	{
		mOptionWindow->ScaleRender(hdc, 140, 35, mOptionWindow->GetWidth(), mOptionWindow->GetHeight());
		mXButton->ScaleRender(hdc, 0.9f, 0.9f);
		mVolumeEffectBar->Render(hdc);
		mVolumeEffectButton->Render(hdc);
		mVolumeBackgroundBar->Render(hdc);
		mVolumeBackgroundButton->Render(hdc);
	}
}

void MainScene::StartButton()
{
	// 우선은 씬 넘어가자
	// 나중에 창 띄워서 버트ㅡㄴ 할거얌
	SCENEMANAGER->LoadScene(L"SceneSelect");
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
