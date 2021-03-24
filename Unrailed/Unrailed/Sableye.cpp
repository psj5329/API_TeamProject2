#include "pch.h"
#include "Sableye.h"

#include "Image.h"
#include "Camera.h"
#include "Animation.h"
#include "Tile.h"

void Sableye::Init()
{
	mImage = IMAGEMANAGER->FindImage(L"Sableye");
	mName = "Sableye";
	mX = 0;
	mY = 0;
	mSizeX = mImage->GetFrameWidth();
	mSizeY = mImage->GetFrameHeight();
	mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);
	mHitBox = RectMakeCenter(mX, mY, mSizeX, mSizeY);
	mState = EnemyState::IDLE;

	mVecItem = OBJECTMANAGER->GetObjectList(ObjectLayer::ITEM);
	mPlayer = OBJECTMANAGER->GetPlayer();
	mTarget = nullptr;

	mIsExistTarget = false;

	mPathFinderList.clear();

	ReadyAnimation();
}

void Sableye::Release()
{
}

void Sableye::Update()
{
	mVecItem = OBJECTMANAGER->GetObjectList(ObjectLayer::ITEM);

	float distance = 0.f;
	mIsExistTarget = false;	// 돌기 전에 false 해두고 다 돌았는데 false면 근처에 없다
							// 가다가 더 가까운 새로운게 나타나면 그리로 가게?
	
	for (int i = 0; i < mVecItem.size(); ++i)
	{
		float x = mVecItem[i]->GetX();
		float y = mVecItem[i]->GetY();
		
		if (Math::GetDistance(mX, mY, x, y) < 5000.f)	// 테스트용 거리
		{
			if (!mIsExistTarget)
			{
				distance = Math::GetDistance(mX, mY, x, y);

				mTarget = mVecItem[i];
				mIsExistTarget = true;

				int x1 = mX / TileSize;
				int y1 = mY / TileSize;
				int x2 = x / TileSize;
				int y2 = y / TileSize;

				mPathFinderList = PATHFINDER->FindPath(mTileList, mMapObjectList, x1, y1, x2, y2);
			}
			else
			{
				if (distance > Math::GetDistance(mX, mY, x, y))
				{
					distance = Math::GetDistance(mX, mY, x, y);

					mTarget = mVecItem[i];
					mIsExistTarget = true;
				}
			}
		}
	}

	if (mTarget != nullptr)	// 우선은 임시로 바로 직선으로 움직이기~
	{
		float angle = Math::GetAngle(mX, mY, mPathFinderList[0]->GetX(), mPathFinderList[0]->GetY());

		mX += cosf(angle) * mSpeed * TIME->DeltaTime();
		mY += -sinf(angle) * mSpeed * TIME->DeltaTime();

		if (Math::GetDistance(mX, mY, mPathFinderList[0]->GetX(), mPathFinderList[0]->GetY()) <= 5.f)
			mPathFinderList.erase(mPathFinderList.begin());
	}
	
	mCurrentAnimation->Update();
	mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);
	mHitBox = RectMakeCenter(mX, mY, mSizeX, mSizeY);
}

void Sableye::Render(HDC hdc)
{
	CAMERAMANAGER->GetMainCamera()->ScaleFrameRender(hdc, mImage, mRect.left, mRect.top
		, mCurrentAnimation->GetNowFrameX(), mCurrentAnimation->GetNowFrameY(), mSizeX * 2, mSizeY * 2);
}

void Sableye::ReadyAnimation()
{
	mIdleAnimation = new Animation();
	mIdleAnimation->InitFrameByStartEnd(0, 0, 0, 0, false);
	mIdleAnimation->SetIsLoop(true);
	mIdleAnimation->SetFrameUpdateTime(0.2f);

	mWalkAnimation = new Animation();
	mWalkAnimation->InitFrameByStartEnd(2, 0, 3, 0, false);
	mWalkAnimation->SetIsLoop(true);
	mWalkAnimation->SetFrameUpdateTime(0.2f);

	mAttackAnimation = new Animation();
	mAttackAnimation->InitFrameByStartEnd(1, 0, 1, 0, false);
	mAttackAnimation->SetIsLoop(true);
	mAttackAnimation->SetFrameUpdateTime(0.2f);

	mDeadAnimation = new Animation();
	mDeadAnimation->InitFrameByStartEnd(4, 0, 4, 0, false);
	mDeadAnimation->SetIsLoop(true);
	mDeadAnimation->SetFrameUpdateTime(0.2f);

	mCurrentAnimation = mIdleAnimation;
}

void Sableye::SetAnimation()
{
	mCurrentAnimation->Stop();

	if (mState == EnemyState::IDLE)
	{
		if (mDirection == Direction::DOWN)
		{
			mIdleAnimation->InitFrameByStartEnd(0, 0, 0, 0, false);
			mCurrentAnimation = mIdleAnimation;
		}
		else if (mDirection == Direction::UP)
		{
			mIdleAnimation->InitFrameByStartEnd(0, 0, 0, 1, false);
			mCurrentAnimation = mIdleAnimation;
		}
		else if (mDirection == Direction::LEFT)
		{
			mIdleAnimation->InitFrameByStartEnd(0, 0, 0, 2, false);
			mCurrentAnimation = mIdleAnimation;
		}
		else if (mDirection == Direction::RIGHT)
		{
			mIdleAnimation->InitFrameByStartEnd(0, 0, 0, 3, false);
			mCurrentAnimation = mIdleAnimation;
		}
	}
	else if (mState == EnemyState::WALK)
	{
		if (mDirection == Direction::DOWN)
		{
			mWalkAnimation->InitFrameByStartEnd(2, 0, 3, 0, false);
			mCurrentAnimation = mIdleAnimation;
		}
		else if (mDirection == Direction::UP)
		{
			mWalkAnimation->InitFrameByStartEnd(2, 0, 3, 1, false);
			mCurrentAnimation = mIdleAnimation;
		}
		else if (mDirection == Direction::LEFT)
		{
			mWalkAnimation->InitFrameByStartEnd(2, 0, 3, 2, false);
			mCurrentAnimation = mIdleAnimation;
		}
		else if (mDirection == Direction::RIGHT)
		{
			mWalkAnimation->InitFrameByStartEnd(2, 0, 3, 3, false);
			mCurrentAnimation = mIdleAnimation;
		}
	}
	else if (mState == EnemyState::ATTACK)
	{
		if (mDirection == Direction::DOWN)
		{
			mAttackAnimation->InitFrameByStartEnd(1, 0, 1, 0, false);
			mCurrentAnimation = mIdleAnimation;
		}
		else if (mDirection == Direction::UP)
		{
			mAttackAnimation->InitFrameByStartEnd(1, 0, 1, 1, false);
			mCurrentAnimation = mIdleAnimation;
		}
		else if (mDirection == Direction::LEFT)
		{
			mAttackAnimation->InitFrameByStartEnd(1, 0, 1, 2, false);
			mCurrentAnimation = mIdleAnimation;
		}
		else if (mDirection == Direction::RIGHT)
		{
			mAttackAnimation->InitFrameByStartEnd(1, 0, 1, 3, false);
			mCurrentAnimation = mIdleAnimation;
		}
	}
	else if (mState == EnemyState::DEAD)
	{
		if (mDirection == Direction::DOWN)
		{
			mDeadAnimation->InitFrameByStartEnd(4, 0, 4, 0, false);
			mCurrentAnimation = mIdleAnimation;
		}
		else if (mDirection == Direction::UP)
		{
			mDeadAnimation->InitFrameByStartEnd(4, 0, 4, 1, false);
			mCurrentAnimation = mIdleAnimation;
		}
		else if (mDirection == Direction::LEFT)
		{
			mDeadAnimation->InitFrameByStartEnd(4, 0, 4, 2, false);
			mCurrentAnimation = mIdleAnimation;
		}
		else if (mDirection == Direction::RIGHT)
		{
			mDeadAnimation->InitFrameByStartEnd(4, 0, 4, 3, false);
			mCurrentAnimation = mIdleAnimation;
		}
	}

	mCurrentAnimation->Play();
}
