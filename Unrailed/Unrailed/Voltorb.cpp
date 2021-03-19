#include "pch.h"
#include "Voltorb.h"
#include "Image.h"
#include "Animation.h"
#include "Camera.h"
#include "Tile.h"
#include "Trail.h"

void Voltorb::Init()
{
	//IMAGEMANAGER->LoadFromFile(L"Voltorb", Resources(L"/Train/voltorb"), 96, 210, 3, 7, true);
	//IMAGEMANAGER->LoadFromFile(L"Electrode", Resources(L"/Train/electrode"), 105, 224, 3, 7, true);
	//IMAGEMANAGER->LoadFromFile(L"Explode", Resources(L"/Train/explode"), 630, 90, 7, 1, true);
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
	mState = State::Sleep;
	mSpeed = 100.f;
	mTimer = 0;

	mCurrentImage = mImage;
	mCurrentAnimation = mSleep;
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
	if (mTimer == 0)
	{
		if (mState == State::Sleep)
		{
			SetAnimation();
		}
	}
	if (mTimer >= 2 && mState == State::Sleep)
	{
		mDirection = Direction::Right;
		mState = State::Move;
		SetAnimation();

		mTimer = 0;
	}
	//계속 들어오지 않게 수정 요망
	if (mState == State::Move)
	{
		if (mDirection == Direction::Down)
		{
			SetAnimation();
		}
		if (mDirection == Direction::Up)
		{
			SetAnimation();
		}
		if (mDirection == Direction::Left)
		{
			SetAnimation();
		}
		if (mDirection == Direction::Right)
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

void Voltorb::Render(HDC hdc)
{
	RenderRect(hdc, mRect);
	mCurrentImage->ScaleFrameRender(hdc, mRect.left, mRect.top, mCurrentAnimation->GetNowFrameX(), mCurrentAnimation->GetNowFrameY(), mSizeX, mSizeY);
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
	if (mState == State::Explode)
	{
		mCurrentAnimation = mExplode;
		mCurrentImage = mExplodeImage;
	}

	mCurrentAnimation->Play();
}

void Voltorb::SetImage(int i)
{
	if (i == 0) // 0은 false
	{
		mCurrentImage = IMAGEMANAGER->FindImage(L"Electrode");
	}
	else
	{
		mCurrentImage = IMAGEMANAGER->FindImage(L"Voltorb");
	}
}

void Voltorb::EndExplode()
{
	if (mState == State::Explode)
	{
		SetIsDestroy(true);
	}
}

Direction Voltorb::CheckTrailDirection()
{
	int indexX = mX / TileSize;
	int indexY = mY / TileSize;

	Direction dir = (Direction)mTrailList[indexY][indexX]->GetDirection();
	return dir;
}

void Voltorb::Move(int indexY, int indexX, Direction dir)
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

