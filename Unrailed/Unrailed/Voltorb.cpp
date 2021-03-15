#include "pch.h"
#include "Voltorb.h"
#include "Image.h"
#include "Animation.h"
#include "Camera.h"

void Voltorb::Init()
{
	IMAGEMANAGER->LoadFromFile(L"Voltorb", Resources(L"/Train/voltorb"), 96, 210, 3, 7, true);
	IMAGEMANAGER->LoadFromFile(L"Electrode", Resources(L"/Train/electrode"), 105, 224, 3, 7, true);
	mImage = IMAGEMANAGER->FindImage(L"Voltorb");

	ReadyAnimation();

	//부모 클래스 (GameObject) 변수
	mX = WINSIZEX / 2;
	mY = WINSIZEY / 2;
	mSizeX = mImage->GetFrameWidth() * 2;
	mSizeY = mImage->GetFrameHeight() * 2;
	mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);

	//Electrode 변수
	mDirection = Direction::Right;
	mState = State::Sleep;
	mSpeed = 100.f;
	mTimer = 0;
	mStop = false;

	mCurrentAnimation = mSleep;
}

void Voltorb::Release()
{
	SafeDelete(mDownMove);
	SafeDelete(mUpMove);
	SafeDelete(mLeftMove);
	SafeDelete(mRightMove);
	SafeDelete(mSleep);
}

void Voltorb::Update()
{
	//상태정하기
	if (mTimer == 0)
	{
		if (mState == State::Sleep)
		{
			SetAnimation();
		}
	}
	if (mTimer >= 5 && mState == State::Sleep)
	{
		mDirection = Direction::Right;
		mState = State::Move;
		SetAnimation();
	}

	//움직임
	if (mState == State::Sleep)
	{
		mTimer += Time::GetInstance()->DeltaTime();
	}
	if (mState == State::Move)
	{
		if (mDirection == Direction::Down)
		{
			mY += mSpeed * Time::GetInstance()->DeltaTime();
		}
		if (mDirection == Direction::Up)
		{
			mY -= mSpeed * Time::GetInstance()->DeltaTime();
		}
		if (mDirection == Direction::Left)
		{
			mX -= mSpeed * Time::GetInstance()->DeltaTime();
		}
		if (mDirection == Direction::Right)
		{
			mX += mSpeed * Time::GetInstance()->DeltaTime();
		}
	}




	mCurrentAnimation->Update();
}

void Voltorb::Render(HDC hdc)
{
	mImage->ScaleFrameRender(hdc, mX, mY, mCurrentAnimation->GetNowFrameX(), mCurrentAnimation->GetNowFrameY(), mSizeX, mSizeY);
}

void Voltorb::ReadyAnimation()
{
	mDownMove = new Animation();
	mDownMove->InitFrameByStartEnd(0, 0, 2, 0, false);
	mDownMove->SetIsLoop(true);
	mDownMove->SetFrameUpdateTime(0.2f);

	mUpMove = new Animation();
	mUpMove->InitFrameByStartEnd(0, 1, 2, 1, false);
	mUpMove->SetIsLoop(true);
	mUpMove->SetFrameUpdateTime(0.2f);

	mLeftMove = new Animation();
	mLeftMove->InitFrameByStartEnd(0, 2, 2, 2, false);
	mLeftMove->SetIsLoop(true);
	mLeftMove->SetFrameUpdateTime(0.2f);

	mRightMove = new Animation();
	mRightMove->InitFrameByStartEnd(0, 3, 2, 3, false);
	mRightMove->SetIsLoop(true);
	mRightMove->SetFrameUpdateTime(0.2f);

	mSleep = new Animation();
	mSleep->InitFrameByStartEnd(0, 6, 1, 6, false);
	mSleep->SetIsLoop(true);
	mSleep->SetFrameUpdateTime(0.2f);

}

void Voltorb::SetAnimation()
{
	mCurrentAnimation->Stop();
	if (mState == State::Move)
	{
		if (mDirection == Direction::Down)
		{
			mCurrentAnimation = mDownMove;
		}
		if (mDirection == Direction::Up)
		{
			mCurrentAnimation = mUpMove;
		}
		if (mDirection == Direction::Left)
		{
			mCurrentAnimation = mLeftMove;
		}
		if (mDirection == Direction::Right)
		{
			mCurrentAnimation = mRightMove;
		}
	}
	if (mState == State::Sleep)
	{
		mCurrentAnimation = mSleep;
	}

	mCurrentAnimation->Play();
}

void Voltorb::SetImage(int i)
{
	if (i == 0) // 0은 false
	{
		mImage = IMAGEMANAGER->FindImage(L"Electrode");
	}
	else
	{
		mImage = IMAGEMANAGER->FindImage(L"Voltorb");
	}
}
