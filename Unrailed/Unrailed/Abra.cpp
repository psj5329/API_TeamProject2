#include "pch.h"
#include "Abra.h"
#include "Image.h"
#include "Animation.h"

void Abra::Init()
{
	//IMAGEMANAGER->LoadFromFile(L"Abra", Resources(L"/Train/abra"), 96, 200, 4, 8, true);
	//IMAGEMANAGER->LoadFromFile(L"Alakazam", Resources(L"/Train/alakazam"), 124, 232, 4, 8, true);
	//IMAGEMANAGER->LoadFromFile(L"Kadabra", Resources(L"/Train/kadabra"), 120, 256, 4, 8, true);
	//IMAGEMANAGER->LoadFromFile(L"Explode", Resources(L"/Train/explode"), 630, 90, 7, 1, true);
	mExplodeImage = IMAGEMANAGER->FindImage(L"Explode");
	mImage = IMAGEMANAGER->FindImage(L"Abra");

	ReadyAnimation();

	//부모 클래스 (GameObject) 변수
	mX = WINSIZEX / 2 - 180;
	mY = WINSIZEY / 2;
	mSizeX = mImage->GetFrameWidth() * 2;
	mSizeY = mImage->GetFrameHeight() * 2;
	mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);

	//Machop 변수
	mDirection = Direction::Right;
	mState = State::Sleep;
	mSpeed = 100.f;
	mTimer = 0;
	mLevel = 1;

	mCurrentImage = mImage;
	mCurrentAnimation = mRightSleep;
}

void Abra::Release()
{
	SafeDelete(mDownMove);
	SafeDelete(mUpMove);
	SafeDelete(mLeftMove);
	SafeDelete(mRightMove);
	SafeDelete(mLeftSleep);
	SafeDelete(mRightSleep);
	SafeDelete(mDownSynthesis);
	SafeDelete(mUpSynthesis);
	SafeDelete(mLeftSynthesis);
	SafeDelete(mRightSynthesis);
	SafeDelete(mExplode);
}

void Abra::Update()
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

	//진화
	switch (mLevel)
	{
	case 1:
		mImage = IMAGEMANAGER->FindImage(L"Abra");
		break;
	case 2:
		mImage = IMAGEMANAGER->FindImage(L"Alakazam");
		break;
	case 3:
		mImage = IMAGEMANAGER->FindImage(L"Kadabra");
		break;
	}
	if (INPUT->GetKeyDown('A'))
	{
		mLevel = 1;
	}
	if (INPUT->GetKeyDown('S'))
	{
		mLevel = 2;
	}
	if (INPUT->GetKeyDown('D'))
	{
		mLevel = 3;
	}

	//폭발
	if (mX >= WINSIZEX - 400 && mIsExplode == false)
	{
		mIsExplode = true;
		mState = State::Explode;
		SetAnimation();
	}

	mCurrentAnimation->Update();
	mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);
}

void Abra::Render(HDC hdc)
{
	RenderRect(hdc, mRect);
	mCurrentImage->ScaleFrameRender(hdc, mRect.left, mRect.top, mCurrentAnimation->GetNowFrameX(), mCurrentAnimation->GetNowFrameY(), mSizeX, mSizeY);
}

void Abra::ReadyAnimation()
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

	mLeftSleep = new Animation();
	mLeftSleep->InitFrameByStartEnd(0, 5, 1, 5, false);
	mLeftSleep->SetIsLoop(true);
	mLeftSleep->SetFrameUpdateTime(0.2f);

	mRightSleep = new Animation();
	mRightSleep->InitFrameByStartEnd(2, 5, 3, 5, false);
	mRightSleep->SetIsLoop(true);
	mRightSleep->SetFrameUpdateTime(0.2f);

	mExplode = new Animation();
	mExplode->InitFrameByStartEnd(0, 0, 6, 0, false);
	mExplode->SetIsLoop(false);
	mExplode->SetFrameUpdateTime(0.1f);
	mExplode->SetCallbackFunc(bind(&Train::EndExplode, this));
}

void Abra::SetAnimation()
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
		mCurrentImage = mImage;
	}
	if (mState == State::Sleep)
	{
		if (mDirection == Direction::Left)
		{
			mCurrentAnimation = mLeftSleep;
		}
		if (mDirection == Direction::Right)
		{
			mCurrentAnimation = mRightSleep;
		}
		mCurrentImage = mImage;
	}
	if (mState == State::Explode)
	{
		mCurrentAnimation = mExplode;
		mCurrentImage = mExplodeImage;
	}

	mCurrentAnimation->Play();
}

void Abra::EndExplode()
{
	if (mState == State::Explode)
	{
		SetIsDestroy(true);
	}
}