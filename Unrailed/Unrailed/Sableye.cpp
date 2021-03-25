#include "pch.h"
#include "Sableye.h"

#include "Image.h"
#include "Camera.h"
#include "Animation.h"
#include "Tile.h"
#include "Unit.h"

void Sableye::Init()
{
	mImage = IMAGEMANAGER->FindImage(L"Sableye");
	mName = "Sableye";
	mX = 48 * 8 + 24;
	mY = 48 * 8 + 24;
	mSizeX = mImage->GetFrameWidth() * 2;
	mSizeY = mImage->GetFrameHeight() * 2;
	mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);
	mHitBox = RectMakeCenter(mX, mY, mSizeX, mSizeY);
	mState = EnemyState::IDLE;
	mSpeed = 100.f;

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

	MoveToOre();

	mCurrentAnimation->Update();
	mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);
	mHitBox = RectMakeCenter(mX, mY, mSizeX, mSizeY);
}

void Sableye::Render(HDC hdc)
{
#ifdef DEBUG
	CAMERAMANAGER->GetMainCamera()->RenderRectCam(hdc, mRect);
#endif

	CAMERAMANAGER->GetMainCamera()->ScaleFrameRender(hdc, mImage, mRect.left, mRect.top
		, mCurrentAnimation->GetNowFrameX(), mCurrentAnimation->GetNowFrameY(), mSizeX, mSizeY);
}

void Sableye::MoveToOre()
{
	float distance = 0.f;
	//mIsExistTarget = false;	// 돌기 전에 false 해두고 다 돌았는데 false면 근처에 없다
							// 가다가 더 가까운 새로운게 나타나면 그리로 가게?

	for (int i = 0; i < mVecItem.size(); ++i)
	{
		float x = mVecItem[i]->GetX();
		float y = mVecItem[i]->GetY();

		if (Math::GetDistance(mX, mY, x, y) < 500.f)	// 테스트용 거리
		{
			if ((int)(mX / 48) == (int)(x / 48) && (int)(mY / 48) == (int)(y / 48))
				break;

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

				if ((int)(mX / 48) > (int)(mPathFinderList[0]->GetX() / 48))
				{
					if (mState != EnemyState::WALK || mDirection != Direction::LEFT)
					{
						mState = EnemyState::WALK;
						mDirection = Direction::LEFT;
						SetAnimation();
					}
				}
				else if ((int)(mX / 48) < (int)(mPathFinderList[0]->GetX() / 48))
				{
					if (mState != EnemyState::WALK || mDirection != Direction::RIGHT)
					{
						mState = EnemyState::WALK;
						mDirection = Direction::RIGHT;
						SetAnimation();
					}
				}

				if ((int)(mY / 48) > (int)(mPathFinderList[0]->GetY() / 48))
				{
					if (mState != EnemyState::WALK || mDirection != Direction::UP)
					{
						mState = EnemyState::WALK;
						mDirection = Direction::UP;
						SetAnimation();
					}
				}
				else if ((int)(mY / 48) < (int)(mPathFinderList[0]->GetY() / 48))
				{
					if (mState != EnemyState::WALK || mDirection != Direction::DOWN)
					{
						mState = EnemyState::WALK;
						mDirection = Direction::DOWN;
						SetAnimation();
					}
				}
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

	if (mIsExistTarget)
	{
		float angle = Math::GetAngle(mX, mY, mPathFinderList[0]->GetX(), mPathFinderList[0]->GetY());

		mX += cosf(angle) * mSpeed * TIME->DeltaTime();
		mY += -sinf(angle) * mSpeed * TIME->DeltaTime();


		if ((int)(mX / 48) == (int)(mPathFinderList[0]->GetX() / 48)
			&& (int)(mY / 48) == (int)(mPathFinderList[0]->GetY() / 48)
			&& Math::GetDistance(mX, mY, mPathFinderList[0]->GetX(), mPathFinderList[0]->GetY()) <= 1.f)
			//if(Math::GetDistance(mX, mY, mPathFinderList[0]->GetX(), mPathFinderList[0]->GetY()) <= 1.f)
		{
			mPathFinderList.erase(mPathFinderList.begin());

			if (mPathFinderList.size() != 0)
			{
				if ((int)(mX / 48) > (int)(mPathFinderList[0]->GetX() / 48))
				{
					if (mState != EnemyState::WALK || mDirection != Direction::LEFT)
					{
						mState = EnemyState::WALK;
						mDirection = Direction::LEFT;
						SetAnimation();
					}
				}
				else if ((int)(mX / 48) < (int)(mPathFinderList[0]->GetX() / 48))
				{
					if (mState != EnemyState::WALK || mDirection != Direction::RIGHT)
					{
						mState = EnemyState::WALK;
						mDirection = Direction::RIGHT;
						SetAnimation();
					}
				}

				if ((int)(mY / 48) > (int)(mPathFinderList[0]->GetY() / 48))
				{
					if (mState != EnemyState::WALK || mDirection != Direction::UP)
					{
						mState = EnemyState::WALK;
						mDirection = Direction::UP;
						SetAnimation();
					}
				}
				else if ((int)(mY / 48) < (int)(mPathFinderList[0]->GetY() / 48))
				{
					if (mState != EnemyState::WALK || mDirection != Direction::DOWN)
					{
						mState = EnemyState::WALK;
						mDirection = Direction::DOWN;
						SetAnimation();
					}
				}
			}
			else
			{
				// 도착했으니까 아이템 훔치게 공격 모션
				if (mState != EnemyState::ATTACK)
				{
					vector<GameObject*>* itemPtr = OBJECTMANAGER->GetObjectListPtr(ObjectLayer::ITEM);

					for (int i = 0; i < (*itemPtr).size(); ++i)
					{
						if ((*itemPtr)[i] == mTarget)
						{
							mItem = mTarget;
							mTarget = nullptr;
							
							(*itemPtr).erase((*itemPtr).begin() + 1);
						}
					}

					mIsExistTarget = false;
					mState = EnemyState::ATTACK;
					SetAnimation();
				}
			}
		}
	}
	else
	{
		//if (mState == EnemyState::ATTACK && mCurrentAnimation->GetNowFrameX() == 0)
		if (!mCurrentAnimation->GetIsPlay())
		{
			mState = EnemyState::IDLE;
			SetAnimation();
		}
	}
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
	mAttackAnimation->InitFrameByStartEnd(0, 0, 1, 0, false);
	mAttackAnimation->SetIsLoop(false);
	mAttackAnimation->SetFrameUpdateTime(0.2f);

	mDeadAnimation = new Animation();
	mDeadAnimation->InitFrameByStartEnd(4, 0, 4, 0, false);
	mDeadAnimation->SetIsLoop(false);
	mDeadAnimation->SetFrameUpdateTime(0.2f);

	mCurrentAnimation = mIdleAnimation;
}

void Sableye::SetAnimation()
{
	mCurrentAnimation->Stop();

	if (mState == EnemyState::IDLE)
	{
		mIdleAnimation->InitFrameByVector(vector<pair<int, int>>());
		if (mDirection == Direction::DOWN)
		{
			mIdleAnimation->InitFrameByStartEnd(0, 0, 0, 0, false);
			mCurrentAnimation = mIdleAnimation;
		}
		else if (mDirection == Direction::UP)
		{
			mIdleAnimation->InitFrameByStartEnd(0, 1, 0, 1, false);
			mCurrentAnimation = mIdleAnimation;
		}
		else if (mDirection == Direction::LEFT)
		{
			mIdleAnimation->InitFrameByStartEnd(0, 2, 0, 2, false);
			mCurrentAnimation = mIdleAnimation;
		}
		else if (mDirection == Direction::RIGHT)
		{
			mIdleAnimation->InitFrameByStartEnd(0, 3, 0, 3, false);
			mCurrentAnimation = mIdleAnimation;
		}
	}
	else if (mState == EnemyState::WALK)
	{
		mWalkAnimation->InitFrameByVector(vector<pair<int, int>>());
		if (mDirection == Direction::DOWN)
		{
			mWalkAnimation->InitFrameByStartEnd(2, 0, 3, 0, false);
			mCurrentAnimation = mWalkAnimation;
		}
		else if (mDirection == Direction::UP)
		{
			mWalkAnimation->InitFrameByStartEnd(2, 1, 3, 1, false);
			mCurrentAnimation = mWalkAnimation;
		}
		else if (mDirection == Direction::LEFT)
		{
			mWalkAnimation->InitFrameByStartEnd(2, 2, 3, 2, false);
			mCurrentAnimation = mWalkAnimation;
		}
		else if (mDirection == Direction::RIGHT)
		{
			mWalkAnimation->InitFrameByStartEnd(2, 3, 3, 3, false);
			mCurrentAnimation = mWalkAnimation;
		}
	}
	else if (mState == EnemyState::ATTACK)
	{
		mAttackAnimation->InitFrameByVector(vector<pair<int, int>>());
		if (mDirection == Direction::DOWN)
		{
			mAttackAnimation->InitFrameByStartEnd(0, 0, 1, 0, false);
			mCurrentAnimation = mAttackAnimation;
		}
		else if (mDirection == Direction::UP)
		{
			mAttackAnimation->InitFrameByStartEnd(0, 1, 1, 1, false);
			mCurrentAnimation = mAttackAnimation;
		}
		else if (mDirection == Direction::LEFT)
		{
			mAttackAnimation->InitFrameByStartEnd(0, 2, 1, 2, false);
			mCurrentAnimation = mAttackAnimation;
		}
		else if (mDirection == Direction::RIGHT)
		{
			mAttackAnimation->InitFrameByStartEnd(0, 3, 1, 3, true);
			mCurrentAnimation = mAttackAnimation;
		}
	}
	else if (mState == EnemyState::DEAD)
	{
		mDeadAnimation->InitFrameByVector(vector<pair<int, int>>());
		if (mDirection == Direction::DOWN)
		{
			mDeadAnimation->InitFrameByStartEnd(4, 0, 4, 0, false);
			mCurrentAnimation = mDeadAnimation;
		}
		else if (mDirection == Direction::UP)
		{
			mDeadAnimation->InitFrameByStartEnd(4, 0, 4, 1, false);
			mCurrentAnimation = mDeadAnimation;
		}
		else if (mDirection == Direction::LEFT)
		{
			mDeadAnimation->InitFrameByStartEnd(4, 0, 4, 2, false);
			mCurrentAnimation = mDeadAnimation;
		}
		else if (mDirection == Direction::RIGHT)
		{
			mDeadAnimation->InitFrameByStartEnd(4, 0, 4, 3, false);
			mCurrentAnimation = mDeadAnimation;
		}
	}

	mCurrentAnimation->Play();
}
