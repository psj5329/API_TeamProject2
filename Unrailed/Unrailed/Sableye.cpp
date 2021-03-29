#include "pch.h"
#include "Sableye.h"

#include "Image.h"
#include "Camera.h"
#include "Animation.h"
#include "Tile.h"
#include "Player.h"

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

	mDustImage = IMAGEMANAGER->FindImage(L"Dust");

	mVecItem = OBJECTMANAGER->GetObjectList(ObjectLayer::ITEM);
	mPlayer = OBJECTMANAGER->GetPlayer();
	mTarget = nullptr;

	mIsExistTarget = false;
	mIsRunAway = false;

	mPathFinderList.clear();

	ReadyAnimation();

	mItemCount = 0;
	mDustFrameX = 0;
	mDustFrameTime = 0.f;

	mIsDead = false;

	mHp = 3;

	mAlpha = 1.f;
}

void Sableye::Release()
{
}

void Sableye::Update()
{
	mVecItem = OBJECTMANAGER->GetObjectList(ObjectLayer::ITEM);
	mItemCount = mVecItem.size();

	if (mIsHit)
	{
		if (mHp <= 0)
		{
			mIsDead = true;
			mState = EnemyState::DEAD;
			SetAnimation();
		}
		else
		{
			mDustFrameTime += TIME->DeltaTime();

			if (mDustFrameTime >= 0.1f)
			{
				mDustFrameTime = 0.f;
				mAlpha -= 0.15f;

				if(mAlpha <= 0.5f)
					mIsHit = false;
			}

		}
	}
	else
	{
		if (mAlpha < 1.f)
		{
			mAlpha += 0.15;

			if (mAlpha >= 1.f)
				mAlpha = 1.f;
		}
	}

	if (mIsDead)
	{
		mDustFrameTime += TIME->DeltaTime();

		if (mDustFrameTime >= 0.1f)
		{
			mDustFrameX++;
			mDustFrameTime = 0.f;
			mAlpha -= 0.15f;

			if (mAlpha <= 0.f)
				mAlpha = 0.f;
		}

		if (mDustFrameX >= 7)
		{
			mDustFrameTime = 0;
			mIsDestroy = true;
		}
	}
	else
	{
		MoveToOre();
		StealOre();
	}

	mCurrentAnimation->Update();
	mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);
	mHitBox = RectMakeCenter(mX, mY, mSizeX, mSizeY);
}

void Sableye::Render(HDC hdc)
{
#ifdef DEBUG
	CAMERAMANAGER->GetMainCamera()->RenderRectCam(hdc, mRect);
#endif

	CAMERAMANAGER->GetMainCamera()->AlphaScaleFrameRender(hdc, mImage, mRect.left, mRect.top
		, mCurrentAnimation->GetNowFrameX(), mCurrentAnimation->GetNowFrameY(), mSizeX, mSizeY, mAlpha);

	if (mIsDead)
	{
		float width = mDustImage->GetWidth() / 7 * 2;
		float height = mDustImage->GetHeight() * 2;
		CAMERAMANAGER->GetMainCamera()->ScaleFrameRender(hdc, mDustImage, mRect.left - width / 4
			, mRect.top - height / 4, mDustFrameX, 0, width, height);
	}
}

void Sableye::MoveToOre()
{
	if (mTarget != nullptr)
		mDistance = Math::GetDistance(mX, mY, mTarget->GetX(), mTarget->GetY());

	if (mItem == nullptr)	// 아이템 안훔쳤을 때
	{
		for (int i = 0; i < mItemCount; ++i)
		{
			float x = mVecItem[i]->GetX();
			float y = mVecItem[i]->GetY();

			if (Math::GetDistance(mX, mY, x, y) < TileSize * 5)	// 테스트용 거리
			{
				if ((int)(mX / 48) == (int)(x / 48) && (int)(mY / 48) == (int)(y / 48))
					break;

				if (!mIsExistTarget)
				{
					mDistance = Math::GetDistance(mX, mY, x, y);

					mTarget = mVecItem[i];
					mIsExistTarget = true;

					int x1 = mX / TileSize;
					int y1 = mY / TileSize;
					int x2 = x / TileSize;
					int y2 = y / TileSize;

					mPathFinderList = PATHFINDER->FindPath(mTileList, mMapObjectList, x1, y1, x2, y2);

					if ((int)(mX / 48) > (int)(mPathFinderList[0]->GetX() / 48))
					{
						if (mState != EnemyState::WALK || mDirection != UnitDirection::LEFT)
						{
							mState = EnemyState::WALK;
							mDirection = UnitDirection::LEFT;
							SetAnimation();
						}
					}
					else if ((int)(mX / 48) < (int)(mPathFinderList[0]->GetX() / 48))
					{
						if (mState != EnemyState::WALK || mDirection != UnitDirection::RIGHT)
						{
							mState = EnemyState::WALK;
							mDirection = UnitDirection::RIGHT;
							SetAnimation();
						}
					}

					if ((int)(mY / 48) > (int)(mPathFinderList[0]->GetY() / 48))
					{
						if (mState != EnemyState::WALK || mDirection != UnitDirection::UP)
						{
							mState = EnemyState::WALK;
							mDirection = UnitDirection::UP;
							SetAnimation();
						}
					}
					else if ((int)(mY / 48) < (int)(mPathFinderList[0]->GetY() / 48))
					{
						if (mState != EnemyState::WALK || mDirection != UnitDirection::DOWN)
						{
							mState = EnemyState::WALK;
							mDirection = UnitDirection::DOWN;
							SetAnimation();
						}
					}
				}
				else
				{
					if (mDistance > Math::GetDistance(mX, mY, x, y) && mTarget != mVecItem[i])
					{
						mDistance = Math::GetDistance(mX, mY, x, y);

						mPathFinderList.clear();
						mPathFinderList = PATHFINDER->FindPath(mTileList, mMapObjectList, mX / TileSize, mY / TileSize, x / TileSize, y / TileSize);
						mTarget = mVecItem[i];
						mIsExistTarget = true;
					}
				}
			}
		}

		if (mIsExistTarget || mIsRunAway)
		{
			float angle = Math::GetAngle(mX, mY, mPathFinderList[0]->GetX() + TileSize / 2, mPathFinderList[0]->GetY() + TileSize / 2);

			mX += cosf(angle) * mSpeed * TIME->DeltaTime();
			mY += -sinf(angle) * mSpeed * TIME->DeltaTime();

			if ((int)(mX / 48) == (int)(((mPathFinderList[0]->GetX() + TileSize / 2) / 48))
				&& (int)(mY / 48) == (int)(((mPathFinderList[0]->GetY() + TileSize / 2) / 48))
				&& Math::GetDistance(mX, mY, mPathFinderList[0]->GetX() + TileSize / 2, mPathFinderList[0]->GetY() + TileSize / 2) <= 1.f)
				//if(Math::GetDistance(mX, mY, mPathFinderList[0]->GetX(), mPathFinderList[0]->GetY()) <= 1.f)
			{
				mPathFinderList.erase(mPathFinderList.begin());

 				if (mPathFinderList.size() != 0)
				{
					if ((int)(mX / 48) > (int)((mPathFinderList[0]->GetX() + TileSize / 2) / 48))
					{
						if (mState != EnemyState::WALK || mDirection != UnitDirection::LEFT)
						{
							mState = EnemyState::WALK;
							mDirection = UnitDirection::LEFT;
							SetAnimation();
						}
					}
					else if ((int)(mX / 48) < (int)((mPathFinderList[0]->GetX() + TileSize / 2) / 48))
					{
						if (mState != EnemyState::WALK || mDirection != UnitDirection::RIGHT)
						{
							mState = EnemyState::WALK;
							mDirection = UnitDirection::RIGHT;
							SetAnimation();
						}
					}

					if ((int)(mY / 48) > (int)((mPathFinderList[0]->GetY() + TileSize / 2) / 48))
					{
						if (mState != EnemyState::WALK || mDirection != UnitDirection::UP)
						{
							mState = EnemyState::WALK;
							mDirection = UnitDirection::UP;
							SetAnimation();
						}
					}
					else if ((int)(mY / 48) < (int)((mPathFinderList[0]->GetY() + TileSize / 2) / 48))
					{
						if (mState != EnemyState::WALK || mDirection != UnitDirection::DOWN)
						{
							mState = EnemyState::WALK;
							mDirection = UnitDirection::DOWN;
							SetAnimation();
						}
					}
				}
				else
				{
					// 도착했으니까 아이템 훔치게 공격 모션
					if (mState != EnemyState::ATTACK)
					{
						/*vector<GameObject*>* itemPtr = OBJECTMANAGER->GetObjectListPtr(ObjectLayer::ITEM);

						for (int i = 0; i < (*itemPtr).size(); ++i)
						{
							if ((*itemPtr)[i] == mTarget)
							{
								mItem = mTarget;
								mTarget = nullptr;

								(*itemPtr).erase((*itemPtr).begin() + i);
							}
						}*/

						mIsExistTarget = false;
						mState = EnemyState::ATTACK;
						SetAnimation();
					}
				}
			}
		}
	}

	// 플레이어가 일정 범위 안에 들어오면 도망가자
	if (Math::GetDistance(mX, mY, OBJECTMANAGER->GetPlayer()->GetX(), OBJECTMANAGER->GetPlayer()->GetY()) <= TileSize * 2 && !mIsRunAway)
	{
		mTarget = nullptr;
		mIsExistTarget = false;
		mIsRunAway = true;
		mPathFinderList.clear();
		float x = mX + (mX - OBJECTMANAGER->GetPlayer()->GetX());		// 해당 위치가 맵 크기 밖이면 안돼!
		float y = mY + (mY - OBJECTMANAGER->GetPlayer()->GetY());
		mPathFinderList = PATHFINDER->FindPath(mTileList, mMapObjectList, mX / TileSize, mY / TileSize, x / TileSize, y / TileSize);
	}
	else if (Math::GetDistance(mX, mY, OBJECTMANAGER->GetPlayer()->GetX(), OBJECTMANAGER->GetPlayer()->GetY()) > TileSize * 2 && mIsRunAway)
	{
		if (!mIsExistTarget)
			mPathFinderList.clear();
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
		if (mDirection == UnitDirection::DOWN)
		{
			mIdleAnimation->InitFrameByStartEnd(0, 0, 0, 0, false);
			mCurrentAnimation = mIdleAnimation;
		}
		else if (mDirection == UnitDirection::UP)
		{
			mIdleAnimation->InitFrameByStartEnd(0, 1, 0, 1, false);
			mCurrentAnimation = mIdleAnimation;
		}
		else if (mDirection == UnitDirection::LEFT)
		{
			mIdleAnimation->InitFrameByStartEnd(0, 2, 0, 2, false);
			mCurrentAnimation = mIdleAnimation;
		}
		else if (mDirection == UnitDirection::RIGHT)
		{
			mIdleAnimation->InitFrameByStartEnd(0, 3, 0, 3, false);
			mCurrentAnimation = mIdleAnimation;
		}
	}
	else if (mState == EnemyState::WALK)
	{
		mWalkAnimation->InitFrameByVector(vector<pair<int, int>>());
		if (mDirection == UnitDirection::DOWN)
		{
			mWalkAnimation->InitFrameByStartEnd(2, 0, 3, 0, false);
			mCurrentAnimation = mWalkAnimation;
		}
		else if (mDirection == UnitDirection::UP)
		{
			mWalkAnimation->InitFrameByStartEnd(2, 1, 3, 1, false);
			mCurrentAnimation = mWalkAnimation;
		}
		else if (mDirection == UnitDirection::LEFT)
		{
			mWalkAnimation->InitFrameByStartEnd(2, 2, 3, 2, false);
			mCurrentAnimation = mWalkAnimation;
		}
		else if (mDirection == UnitDirection::RIGHT)
		{
			mWalkAnimation->InitFrameByStartEnd(2, 3, 3, 3, false);
			mCurrentAnimation = mWalkAnimation;
		}
	}
	else if (mState == EnemyState::ATTACK)
	{
		mAttackAnimation->InitFrameByVector(vector<pair<int, int>>());
		if (mDirection == UnitDirection::DOWN)
		{
			mAttackAnimation->InitFrameByStartEnd(0, 0, 1, 0, false);
			mCurrentAnimation = mAttackAnimation;
		}
		else if (mDirection == UnitDirection::UP)
		{
			mAttackAnimation->InitFrameByStartEnd(0, 1, 1, 1, false);
			mCurrentAnimation = mAttackAnimation;
		}
		else if (mDirection == UnitDirection::LEFT)
		{
			mAttackAnimation->InitFrameByStartEnd(0, 2, 1, 2, false);
			mCurrentAnimation = mAttackAnimation;
		}
		else if (mDirection == UnitDirection::RIGHT)
		{
			mAttackAnimation->InitFrameByStartEnd(0, 3, 1, 3, true);
			mCurrentAnimation = mAttackAnimation;
		}
	}
	else if (mState == EnemyState::DEAD)
	{
		mDeadAnimation->InitFrameByVector(vector<pair<int, int>>());
		if (mDirection == UnitDirection::DOWN)
		{
			mDeadAnimation->InitFrameByStartEnd(4, 0, 4, 0, false);
			mCurrentAnimation = mDeadAnimation;
		}
		else if (mDirection == UnitDirection::UP)
		{
			mDeadAnimation->InitFrameByStartEnd(4, 0, 4, 1, false);
			mCurrentAnimation = mDeadAnimation;
		}
		else if (mDirection == UnitDirection::LEFT)
		{ 
			mDeadAnimation->InitFrameByStartEnd(4, 0, 4, 2, false);
			mCurrentAnimation = mDeadAnimation;
		}
		else if (mDirection == UnitDirection::RIGHT)
		{
			mDeadAnimation->InitFrameByStartEnd(4, 0, 4, 3, false);
			mCurrentAnimation = mDeadAnimation;
		}
	}

	mCurrentAnimation->Play();
}

void Sableye::SetAnimation(EnemyState state)
{
	mCurrentAnimation->Stop();

	if (mState == EnemyState::IDLE)
	{
		mIdleAnimation->InitFrameByVector(vector<pair<int, int>>());
		if (mDirection == UnitDirection::DOWN)
		{
			mIdleAnimation->InitFrameByStartEnd(0, 0, 0, 0, false);
			mCurrentAnimation = mIdleAnimation;
		}
		else if (mDirection == UnitDirection::UP)
		{
			mIdleAnimation->InitFrameByStartEnd(0, 1, 0, 1, false);
			mCurrentAnimation = mIdleAnimation;
		}
		else if (mDirection == UnitDirection::LEFT)
		{
			mIdleAnimation->InitFrameByStartEnd(0, 2, 0, 2, false);
			mCurrentAnimation = mIdleAnimation;
		}
		else if (mDirection == UnitDirection::RIGHT)
		{
			mIdleAnimation->InitFrameByStartEnd(0, 3, 0, 3, false);
			mCurrentAnimation = mIdleAnimation;
		}
	}
	else if (mState == EnemyState::WALK)
	{
		mWalkAnimation->InitFrameByVector(vector<pair<int, int>>());
		if (mDirection == UnitDirection::DOWN)
		{
			mWalkAnimation->InitFrameByStartEnd(2, 0, 3, 0, false);
			mCurrentAnimation = mWalkAnimation;
		}
		else if (mDirection == UnitDirection::UP)
		{
			mWalkAnimation->InitFrameByStartEnd(2, 1, 3, 1, false);
			mCurrentAnimation = mWalkAnimation;
		}
		else if (mDirection == UnitDirection::LEFT)
		{
			mWalkAnimation->InitFrameByStartEnd(2, 2, 3, 2, false);
			mCurrentAnimation = mWalkAnimation;
		}
		else if (mDirection == UnitDirection::RIGHT)
		{
			mWalkAnimation->InitFrameByStartEnd(2, 3, 3, 3, false);
			mCurrentAnimation = mWalkAnimation;
		}
	}
	else if (mState == EnemyState::ATTACK)
	{
		mAttackAnimation->InitFrameByVector(vector<pair<int, int>>());
		if (mDirection == UnitDirection::DOWN)
		{
			mAttackAnimation->InitFrameByStartEnd(0, 0, 1, 0, false);
			mCurrentAnimation = mAttackAnimation;
		}
		else if (mDirection == UnitDirection::UP)
		{
			mAttackAnimation->InitFrameByStartEnd(0, 1, 1, 1, false);
			mCurrentAnimation = mAttackAnimation;
		}
		else if (mDirection == UnitDirection::LEFT)
		{
			mAttackAnimation->InitFrameByStartEnd(0, 2, 1, 2, false);
			mCurrentAnimation = mAttackAnimation;
		}
		else if (mDirection == UnitDirection::RIGHT)
		{
			mAttackAnimation->InitFrameByStartEnd(0, 3, 1, 3, true);
			mCurrentAnimation = mAttackAnimation;
		}
	}
	else if (mState == EnemyState::DEAD)
	{
		mDeadAnimation->InitFrameByVector(vector<pair<int, int>>());
		if (mDirection == UnitDirection::DOWN)
		{
			mDeadAnimation->InitFrameByStartEnd(4, 0, 4, 0, false);
			mCurrentAnimation = mDeadAnimation;
		}
		else if (mDirection == UnitDirection::UP)
		{
			mDeadAnimation->InitFrameByStartEnd(4, 0, 4, 1, false);
			mCurrentAnimation = mDeadAnimation;
		}
		else if (mDirection == UnitDirection::LEFT)
		{
			mDeadAnimation->InitFrameByStartEnd(4, 0, 4, 2, false);
			mCurrentAnimation = mDeadAnimation;
		}
		else if (mDirection == UnitDirection::RIGHT)
		{
			mDeadAnimation->InitFrameByStartEnd(4, 0, 4, 3, false);
			mCurrentAnimation = mDeadAnimation;
		}
	}

	mCurrentAnimation->Play();
}

void Sableye::StealOre()
{
	if (!mCurrentAnimation->GetIsPlay())
	{
		if (mState == EnemyState::ATTACK)
		{
			vector<GameObject*>* itemPtr = OBJECTMANAGER->GetObjectListPtr(ObjectLayer::ITEM);

			for (int i = 0; i < (*itemPtr).size(); ++i)
			{
				if ((*itemPtr)[i] == mTarget)
				{
					mItem = mTarget;
					mTarget = nullptr;

					(*itemPtr).erase((*itemPtr).begin() + i);
				}
			}
			mIsDead = true;
		}

		mState = EnemyState::IDLE;
		SetAnimation();
	}
}
