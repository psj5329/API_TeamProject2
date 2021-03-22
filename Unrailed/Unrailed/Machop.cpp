#include "pch.h"
#include "Machop.h"
#include "Image.h"
#include "Animation.h"
#include "Tile.h"
#include "Trail.h"
#include "Camera.h"

void Machop::Init()
{
	//IMAGEMANAGER->LoadFromFile(L"Machop", Resources(L"/Train/machop"), 92, 260, 4, 10, true);
	//IMAGEMANAGER->LoadFromFile(L"Machoke", Resources(L"/Train/machoke"), 128, 256, 4, 8, true);
	//IMAGEMANAGER->LoadFromFile(L"Machamp", Resources(L"/Train/machamp"), 124, 320, 4, 10, true);
	//IMAGEMANAGER->LoadFromFile(L"Explode", Resources(L"/Train/explode"), 630, 90, 7, 1, true);
	mExplodeImage = IMAGEMANAGER->FindImage(L"Explode");
	mImage = IMAGEMANAGER->FindImage(L"Machop");

	ReadyAnimation();

	//부모 클래스 (GameObject) 변수
	mX = WINSIZEX / 2 - 135;
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
	mReachTile = false;

	mCurrentImage = mImage;
	mCurrentAnimation = mRightSleep;
}

void Machop::Release()
{
	SafeDelete(mDownMove);
	SafeDelete(mUpMove);
	SafeDelete(mLeftMove);
	SafeDelete(mRightMove);
	SafeDelete(mLeftSleep);
	SafeDelete(mRightSleep);
	SafeDelete(mDownIntercept);
	SafeDelete(mUpIntercept);
	SafeDelete(mLeftIntercept);
	SafeDelete(mRightIntercept);
	SafeDelete(mExplode);
}

void Machop::Update()
{
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
	//if (mTimer >= 5 && mState == State::Sleep)
	//{
	//	mDirection = Direction::Right;
	//	mState = State::Move;
	//	SetAnimation();
	//}


	if (mTDirection == TrailDirection::Down)
	{
		if (mState == State::Move)
		{
			if (mDirection == Direction::Down)
			{
				SetAnimation();
			}
		}
	}
	if (mTDirection == TrailDirection::Up)
	{
		if (mState == State::Move)
		{
			if (mDirection == Direction::Up)
			{
				SetAnimation();
			}
		}
	}
	if (mTDirection == TrailDirection::Left)
	{
		if (mState == State::Move)
		{
			if (mDirection == Direction::Left)
			{
				SetAnimation();
			}
		}
	}
	if (mTDirection == TrailDirection::Right)
	{
		if (mState == State::Move)
		{
			if (mDirection == Direction::Right)
			{
				SetAnimation();
			}
		}
	}

	//움직임
	if (mState == State::Sleep)
	{
		mTimer += Time::GetInstance()->DeltaTime();
	}
	if (mReachTile == false)
	{
		if (CheckTrailDirection() == Direction::Down)
		{
			Move(indexY + 1, indexX, Direction::Down);
		}
		if (CheckTrailDirection() == Direction::Up)
		{
			Move(indexY - 1, indexX, Direction::Up);
		}
		if (CheckTrailDirection() == Direction::Left)
		{
			Move(indexY, indexX - 1, Direction::Left);
		}
		if (CheckTrailDirection() == Direction::Right)
		{
			Move(indexY, indexX + 1, Direction::Right);
		}
	}

	//진화
	switch (mLevel)
	{
	case 1:
		mImage = IMAGEMANAGER->FindImage(L"Machop");
		break;
	case 2:
		mImage = IMAGEMANAGER->FindImage(L"Machoke");
		break;
	case 3:
		mImage = IMAGEMANAGER->FindImage(L"Machamp");
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

	if (mX >= WINSIZEX - 400 && mIsExplode == false)
	{
		mIsExplode = true;
		mState = State::Explode;
		SetAnimation();
	}

	mCurrentAnimation->Update();
	mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);
}

void Machop::Render(HDC hdc)
{
	//RenderRect(hdc, mRect);
	//mCurrentImage->ScaleFrameRender(hdc, mRect.left, mRect.top, mCurrentAnimation->GetNowFrameX(), mCurrentAnimation->GetNowFrameY(), mSizeX, mSizeY);
	CAMERAMANAGER->GetMainCamera()->RenderRectCam(hdc, mRect);
	CAMERAMANAGER->GetMainCamera()->ScaleFrameRender(hdc, mImage, mRect.left, mRect.top, mCurrentAnimation->GetNowFrameX(), mCurrentAnimation->GetNowFrameY(), mSizeX, mSizeY);
}

void Machop::ReadyAnimation()
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

void Machop::SetAnimation()
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

void Machop::EndExplode()
{
	if (mState == State::Explode)
	{
		SetIsDestroy(true);
	}
}

Direction Machop::CheckTrailDirection()
{
	int indexX = mX / TileSize;
	int indexY = mY / TileSize;

	Direction dir = (Direction)mTrailList[indexY][indexX]->GetDirection();
	return dir;
}

void Machop::Move(int indexY, int indexX, Direction dir)
{
	float centerX = (mTrailList[indexY][indexX]->GetRect().left + mTrailList[indexY][indexX]->GetRect().right) / 2;
	float centerY = (mTrailList[indexY][indexX]->GetRect().top + mTrailList[indexY][indexX]->GetRect().bottom) / 2;

	if (dir == Direction::Down)
	{
		if (mY <= centerY)
		{
			mY += mSpeed * Time::GetInstance()->DeltaTime() / 2;
		}
		else
		{
			mReachTile = true;
		}
	}
	if (dir == Direction::Up)
	{
		if (mY >= centerY)
		{
			mY -= mSpeed * Time::GetInstance()->DeltaTime() / 2;
		}
		else
		{
			mReachTile = true;
		}
	}
	if (dir == Direction::Left)
	{
		if (mX >= centerX)
		{
			mX -= mSpeed * Time::GetInstance()->DeltaTime() / 2;
		}
		else
		{
			mReachTile = true;
		}
	}
	if (dir == Direction::Right)
	{
		if (mX <= centerX)
		{
			mX += mSpeed * Time::GetInstance()->DeltaTime() / 2;
		}
		else
		{
			mReachTile = true;
		}
	}
}