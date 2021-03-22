#include "pch.h"
#include "Voltorb.h"
#include "Image.h"
#include "Animation.h"
#include "Tile.h"
#include "Trail.h"
#include "Camera.h"

void Voltorb::Init()
{
	mExplodeImage = IMAGEMANAGER->FindImage(L"Explode");
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
	mState = State::Move;
	mSpeed = 100.f;
	mTimer = 0;

	mCurrentImage = mImage;
	mCurrentAnimation = mRightMove;
	mCurrentAnimation->Play();
}

void Voltorb::Release()
{
	SafeDelete(mDownMove);
	SafeDelete(mUpMove);
	SafeDelete(mLeftMove);
	SafeDelete(mRightMove);
	SafeDelete(mSleep);
	SafeDelete(mExplode);
}

void Voltorb::Update()
{
	//인덱스 가져오기
	int indexX = mX / TileSize;
	int indexY = mY / TileSize;

	//상태정하기
	//if (mTimer == 0)
	//{
	//	if (mState == State::Sleep)
	//	{
	//		SetAnimation();
	//	}
	//}
	//if (mTimer >= 2 && mState == State::Sleep)
	//{
	//	mDirection = Direction::Right;
	//	mState = State::Move;
	//	SetAnimation();
	//
	//	mTimer = 0;
	//}

	if (mState == State::Move)
	{
		if (mDirection == Direction::Down)
		{
			SetAnimation();
		}
		else if (mDirection == Direction::Up)
		{
			SetAnimation();
		}
		else if (mDirection == Direction::Left)
		{
			SetAnimation();
		}
		else if (mDirection == Direction::Right)
		{
			SetAnimation();
		}
	}
	//움직임
	if (mState == State::Sleep)
	{
		mTimer += Time::GetInstance()->DeltaTime();
	}

	//올라가있는 기차길의 현재 기차길/타일의 중간오면 방향확인
	//방향이 가리키는 타일의 중간까지이동
	SetSpeed();
	if (mState == State::Move)
	{
		mX += mSpeedX * Time::GetInstance()->DeltaTime() / 2;
		mY += mSpeedY * Time::GetInstance()->DeltaTime() / 2;
	}
	if (CheckTile() == true)
	{
		SetTarget();
	}

	//폭발
	//if (mX >= WINSIZEX - 400 && mIsExplode == false)
	//{
	//	mIsExplode = true;
	//	mState = State::Explode;
	//	SetAnimation();
	//}

	mCurrentAnimation->Update();
	mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);
}

void Voltorb::Render(HDC hdc)
{
	//RenderRect(hdc, mRect);
	//mCurrentImage->ScaleFrameRender(hdc, mRect.left, mRect.top, mCurrentAnimation->GetNowFrameX(), mCurrentAnimation->GetNowFrameY(), mSizeX, mSizeY);
	CAMERAMANAGER->GetMainCamera()->RenderRectCam(hdc, mRect);
	CAMERAMANAGER->GetMainCamera()->ScaleFrameRender(hdc, mImage, mRect.left, mRect.top, mCurrentAnimation->GetNowFrameX(), mCurrentAnimation->GetNowFrameY(), mSizeX, mSizeY);
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

	mExplode = new Animation();
	mExplode->InitFrameByStartEnd(0, 0, 6, 0, false);
	mExplode->SetIsLoop(false);
	mExplode->SetFrameUpdateTime(0.1f);
	mExplode->SetCallbackFunc(bind(&Train::EndExplode, this));
}

void Voltorb::SetAnimation()
{
	if (mState == State::Move)
	{
		if (mDirection == Direction::Down)
		{
			if (mCurrentAnimation != mDownMove)
			{
				mCurrentAnimation->Stop();
				mCurrentAnimation = mDownMove;
				mCurrentAnimation->Play();
			}
		}
		if (mDirection == Direction::Up)
		{
			if (mCurrentAnimation != mUpMove)
			{
				mCurrentAnimation->Stop();
				mCurrentAnimation = mUpMove;
				mCurrentAnimation->Play();
			}
		}
		if (mDirection == Direction::Left)
		{

			if (mCurrentAnimation != mLeftMove)
			{
				mCurrentAnimation->Stop();
				mCurrentAnimation = mLeftMove;
				mCurrentAnimation->Play();
			}
		}
		if (mDirection == Direction::Right)
		{
			if (mCurrentAnimation != mRightMove)
			{
				mCurrentAnimation->Stop();
				mCurrentAnimation = mRightMove;
				mCurrentAnimation->Play();

			}
		}
	}

	if (mState == State::Sleep)
	{
		mCurrentAnimation->Stop();
		mCurrentAnimation = mSleep;
		mCurrentAnimation->Play();
	}
	if (mState == State::Explode)
	{
		mCurrentAnimation->Stop();
		mCurrentAnimation = mExplode;
		mCurrentAnimation->Play();
		mCurrentImage = mExplodeImage;
	}
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

void Voltorb::EndExplode()
{
	if (mState == State::Explode)
	{
		SetIsDestroy(true);
	}
}