#include "pch.h"
#include "Scene.h"

#include "Image.h"
#include "Button.h"
#include "Camera.h"

void Scene::WindowInit()
{
	mPauseWindow = IMAGEMANAGER->FindImage(L"PauseWindow");	// 일시정지 테스트
	mPauseOptionWindow = IMAGEMANAGER->FindImage(L"PauseOptionWindow");

	Image* ContinueButton = IMAGEMANAGER->FindImage(L"PauseContinueButton");
	Image* OptionButton = IMAGEMANAGER->FindImage(L"PauseOptionButton");
	Image* MainButton = IMAGEMANAGER->FindImage(L"PauseMainButton");
	Image* XButton = IMAGEMANAGER->FindImage(L"XButton");
	Image* VolumeButton = IMAGEMANAGER->FindImage(L"VolumeButton");
	Image* VolumeBar = IMAGEMANAGER->FindImage(L"VolumeBar");

	mContinueButton = new Button(ContinueButton, WINSIZEX / 2, WINSIZEY / 2 - 40, ContinueButton->GetWidth(), ContinueButton->GetHeight(), bind(&Scene::ContinueButton, this));
	mOptionButton = new Button(OptionButton, WINSIZEX / 2, WINSIZEY / 2 + 40, OptionButton->GetWidth(), OptionButton->GetHeight(), bind(&Scene::OptionButton, this));
	mMainButton = new Button(MainButton, WINSIZEX / 2, WINSIZEY / 2 + 120, MainButton->GetWidth(), MainButton->GetHeight(), bind(&Scene::MainButton, this));
	mXButton = new Button(XButton, WINSIZEX / 2 + 200, 210, XButton->GetWidth(), XButton->GetHeight(), bind(&Scene::XButton, this));
	mVolumeEffectBar = new Button(VolumeBar, WINSIZEX / 2 + 75, WINSIZEY / 2 - 55, VolumeBar->GetWidth(), VolumeBar->GetHeight(), bind(&Scene::VolumeEffectBar, this));
	mVolumeEffectButton = new Button(VolumeButton, WINSIZEX / 2 + 75, WINSIZEY / 2 - 55, VolumeButton->GetWidth(), VolumeButton->GetHeight(), bind(&Scene::VolumeEffectButton, this));
	mVolumeBackgroundBar = new Button(VolumeBar, WINSIZEX / 2 + 75, WINSIZEY / 2 + 35, VolumeBar->GetWidth(), VolumeBar->GetHeight(), bind(&Scene::VolumeBackgroundBar, this));
	mVolumeBackgroundButton = new Button(VolumeButton, WINSIZEX / 2 + 75, WINSIZEY / 2 + 35, VolumeButton->GetWidth(), VolumeButton->GetHeight(), bind(&Scene::VolumeBackgroundButton, this));
}

void Scene::ContinueButton()
{
	mIsPause = false;
}

void Scene::OptionButton()
{
	mIsOption = true;
}

void Scene::MainButton()
{
	mIsPause = false;
	mIsOption = false;
	SCENEMANAGER->LoadScene(L"MainScene");
}

void Scene::XButton()
{
	if (mIsOption)
		mIsOption = false;
}

void Scene::VolumeEffectButton()
{
	mVolumeEffectButton->SetState(Button::State::EffectScroll);
	mVolumeEffectButton->SetIsEffectScroll(true);

	if (Button::GetIsEffectScroll())
	{
		mVolumeEffectButton->SetX(_mousePosition.x);

		if (mVolumeEffectButton->GetX() <= mVolumeEffectBar->GetX() - mVolumeEffectBar->GetSizeX() / 2 + mVolumeEffectButton->GetSizeX() / 2)
			mVolumeEffectButton->SetX(mVolumeEffectBar->GetX() - mVolumeEffectBar->GetSizeX() / 2 + mVolumeEffectButton->GetSizeX() / 2);
		else if (mVolumeEffectButton->GetX() >= mVolumeEffectBar->GetX() + mVolumeEffectBar->GetSizeX() / 2 - mVolumeEffectButton->GetSizeX() / 2)
			mVolumeEffectButton->SetX(mVolumeEffectBar->GetX() + mVolumeEffectBar->GetSizeX() / 2 - mVolumeEffectButton->GetSizeX() / 2);
	}
}

void Scene::VolumeEffectBar()
{
	mVolumeEffectBar->SetState(Button::State::EffectScroll);
	mVolumeEffectBar->SetIsEffectScroll(true);

	if (Button::GetIsEffectScroll())
	{
		mVolumeEffectButton->SetX(_mousePosition.x);

		if (mVolumeEffectButton->GetX() <= mVolumeEffectBar->GetX() - mVolumeEffectBar->GetSizeX() / 2 + mVolumeEffectButton->GetSizeX() / 2)
			mVolumeEffectButton->SetX(mVolumeEffectBar->GetX() - mVolumeEffectBar->GetSizeX() / 2 + mVolumeEffectButton->GetSizeX() / 2);
		else if (mVolumeEffectButton->GetX() >= mVolumeEffectBar->GetX() + mVolumeEffectBar->GetSizeX() / 2 - mVolumeEffectButton->GetSizeX() / 2)
			mVolumeEffectButton->SetX(mVolumeEffectBar->GetX() + mVolumeEffectBar->GetSizeX() / 2 - mVolumeEffectButton->GetSizeX() / 2);
	}
}

void Scene::VolumeBackgroundButton()
{
	mVolumeBackgroundButton->SetState(Button::State::BackgroundScroll);
	mVolumeBackgroundButton->SetIsBackgroundScroll(true);

	if (Button::GetIsBackgroundScroll())
	{
		mVolumeBackgroundButton->SetX(_mousePosition.x);

		if (mVolumeBackgroundButton->GetX() <= mVolumeBackgroundBar->GetX() - mVolumeBackgroundBar->GetSizeX() / 2 + mVolumeBackgroundButton->GetSizeX() / 2)
			mVolumeBackgroundButton->SetX(mVolumeBackgroundBar->GetX() - mVolumeBackgroundBar->GetSizeX() / 2 + mVolumeBackgroundButton->GetSizeX() / 2);
		else if (mVolumeBackgroundButton->GetX() >= mVolumeBackgroundBar->GetX() + mVolumeBackgroundBar->GetSizeX() / 2 - mVolumeBackgroundButton->GetSizeX() / 2)
			mVolumeBackgroundButton->SetX(mVolumeBackgroundBar->GetX() + mVolumeBackgroundBar->GetSizeX() / 2 - mVolumeBackgroundButton->GetSizeX() / 2);
	}
}

void Scene::VolumeBackgroundBar()
{
	mVolumeBackgroundBar->SetState(Button::State::BackgroundScroll);
	mVolumeBackgroundBar->SetIsBackgroundScroll(true);

	if (Button::GetIsBackgroundScroll())
	{
		mVolumeBackgroundButton->SetX(_mousePosition.x);

		if (mVolumeBackgroundButton->GetX() <= mVolumeBackgroundBar->GetX() - mVolumeBackgroundBar->GetSizeX() / 2 + mVolumeBackgroundButton->GetSizeX() / 2)
			mVolumeBackgroundButton->SetX(mVolumeBackgroundBar->GetX() - mVolumeBackgroundBar->GetSizeX() / 2 + mVolumeBackgroundButton->GetSizeX() / 2);
		else if (mVolumeBackgroundButton->GetX() >= mVolumeBackgroundBar->GetX() + mVolumeBackgroundBar->GetSizeX() / 2 - mVolumeBackgroundButton->GetSizeX() / 2)
			mVolumeBackgroundButton->SetX(mVolumeBackgroundBar->GetX() + mVolumeBackgroundBar->GetSizeX() / 2 - mVolumeBackgroundButton->GetSizeX() / 2);
	}
}

