#include "pch.h"
#include "Sableye.h"

#include "Image.h"
#include "Camera.h"
#include "Animation.h"
#include "Tile.h"
#include "Player.h"
#include "MapObject.h"

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
	mSpeed = 90.f;

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

	if (!mIsRunAway)	// 플레이어한테 쫓길 때에는 아이템 훔치러 안가!
	{
		//if (mItem == nullptr)// && mState != EnemyState::ATTACK)	// 아이템 안훔쳤고 훔치는 모션도 아닐때
		{
			for (int i = 0; i < mItemCount; ++i)
			{
				float x = mVecItem[i]->GetX();
				float y = mVecItem[i]->GetY();

				if (Math::GetDistance(mX, mY, x, y) < TileSize * 5)	// 테스트용 거리
				{
					/*if ((int)(mX / 48) == (int)(x / 48) && (int)(mY / 48) == (int)(y / 48))
						break;*/

					if (!mIsExistTarget)		// 처음 목표를 발견!
					{
						mDistance = Math::GetDistance(mX, mY, x, y);

						mTarget = mVecItem[i];
						mIsExistTarget = true;

						int x1 = mX / TileSize;
						int y1 = mY / TileSize;
						int x2 = x / TileSize;
						int y2 = y / TileSize;

						if (x1 != x2 && y1 != y2)	// 같은 자리면 안돼
						{
							mPathFinderList = PATHFINDER->FindPath(mTileList, mMapObjectList, x1, y1, x2, y2);

							NextDir();
						}
						else	// 이미 그 자리인걸
						{
							if (mState != EnemyState::ATTACK && !mIsRunAway)
							{
								int x1 = mX / TileSize;
								int y1 = mY / TileSize;
								int x2 = mTarget->GetX() / TileSize;
								int y2 = mTarget->GetY() / TileSize;

								if (x1 == x2 && y1 == y2)
								{
									//mIsExistTarget = false;
									mState = EnemyState::ATTACK;
									SetAnimation();
									return;
								}
								else
									mIsExistTarget = false;
							}
						}
					}
					else		// 목표는 있지만 아직 줍지 않아서 가장 가까운거 갱신할 수 잇음
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
		}
	}

	// 플레이어가 일정 범위 안에 들어오면 도망가자
	if (Math::GetDistance(mX, mY, OBJECTMANAGER->GetPlayer()->GetX(), OBJECTMANAGER->GetPlayer()->GetY()) <= TileSize * 3 && !mIsRunAway)
	{
		mTarget = nullptr;
		mIsExistTarget = false;
		mIsRunAway = true;
		mPathFinderList.clear();
		
		while (1)
		{
			// 랜덤 칸 고르기
			int x = rand() % 7 - 3;
			int y = rand() % 7 - 3;

			if (x == -3 || x == 3 || y == -3 || y == 3)	// 가생이 칸 선택
			{
				x += mX / TileSize;
				y += mY / TileSize;

				if (x < 0 || x >= TileCountX || y < 0 || y >= TileCountY)
					continue;

				if (abs(OBJECTMANAGER->GetPlayer()->GetX() / TileSize - x) + abs(OBJECTMANAGER->GetPlayer()->GetY() / TileSize - y) <= 3)
					continue;

				// 갈 수 있는 타일 && 오브젝트 없음인 상태인 것만 목표 지점으로 설정할 수 잇다
				if (mTileList[y][x]->GetTileType() == TileType::Normal && mMapObjectList[y][x]->GetIntType() == 0)
				{
					mPathFinderList = PATHFINDER->FindPath(mTileList, mMapObjectList, mX / TileSize, mY / TileSize, x, y);
					NextDir();
					break;
				}
			}
		}
	}
	else if (Math::GetDistance(mX, mY, OBJECTMANAGER->GetPlayer()->GetX(), OBJECTMANAGER->GetPlayer()->GetY()) > TileSize * 3 && mIsRunAway)
	{
		mIsRunAway = false;
		mIsExistTarget = false;
		mPathFinderList.clear();
	}

	// 아이템 벡터 다 돌고 타겟이 있으니까 움직여야지? 또는 도망쳐야해서 도착지점이 있어서 움직여야하지?
	if ((mIsExistTarget || mIsRunAway) && mState != EnemyState::ATTACK)
	{
		float angle = Math::GetAngle(mX, mY, mPathFinderList[0]->GetX() + TileSize / 2, mPathFinderList[0]->GetY() + TileSize / 2);

		mX += cosf(angle) * mSpeed * TIME->DeltaTime();
		mY += -sinf(angle) * mSpeed * TIME->DeltaTime();

		// 목표 지점 벡터의 0번칸에 도착했으면 지우고 한 칸씩 앞으로
		if ((int)(mX / 48) == (int)(((mPathFinderList[0]->GetX() + TileSize / 2) / 48))
			&& (int)(mY / 48) == (int)(((mPathFinderList[0]->GetY() + TileSize / 2) / 48)))
			//&& Math::GetDistance(mX, mY, mPathFinderList[0]->GetX() + TileSize / 2, mPathFinderList[0]->GetY() + TileSize / 2) <= 1.f)
		{
			mPathFinderList.erase(mPathFinderList.begin());

			if (mPathFinderList.size() != 0)
			{
				NextDir();
			}
			else
			{
				// 도착했으니까 아이템 훔치게 공격 모션
				if (mState != EnemyState::ATTACK && !mIsRunAway && mTarget != nullptr)
				{
					int x1 = mX / TileSize;
					int y1 = mY / TileSize;
					int x2 = mTarget->GetX() / TileSize;
					int y2 = mTarget->GetY() / TileSize;

					if (x1 == x2 && y1 == y2)
					{
						//mIsExistTarget = false;
						mState = EnemyState::ATTACK;
						SetAnimation();
					}
					else
						mIsExistTarget = false;
				}

				// 도망치던 상태에서는 사라지지 않고 새로운 목표로!
				if (mIsRunAway)
					mIsRunAway = false;
			}
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
		if (mDirection == Direction::Down)
		{
			mIdleAnimation->InitFrameByStartEnd(0, 0, 0, 0, false);
			mCurrentAnimation = mIdleAnimation;
		}
		else if (mDirection == Direction::Up)
		{
			mIdleAnimation->InitFrameByStartEnd(0, 1, 0, 1, false);
			mCurrentAnimation = mIdleAnimation;
		}
		else if (mDirection == Direction::Left)
		{
			mIdleAnimation->InitFrameByStartEnd(0, 2, 0, 2, false);
			mCurrentAnimation = mIdleAnimation;
		}
		else if (mDirection == Direction::Right)
		{
			mIdleAnimation->InitFrameByStartEnd(0, 3, 0, 3, false);
			mCurrentAnimation = mIdleAnimation;
		}
	}
	else if (mState == EnemyState::WALK)
	{
		mWalkAnimation->InitFrameByVector(vector<pair<int, int>>());
		if (mDirection == Direction::Down)
		{
			mWalkAnimation->InitFrameByStartEnd(2, 0, 3, 0, false);
			mCurrentAnimation = mWalkAnimation;
		}
		else if (mDirection == Direction::Up)
		{
			mWalkAnimation->InitFrameByStartEnd(2, 1, 3, 1, false);
			mCurrentAnimation = mWalkAnimation;
		}
		else if (mDirection == Direction::Left)
		{
			mWalkAnimation->InitFrameByStartEnd(2, 2, 3, 2, false);
			mCurrentAnimation = mWalkAnimation;
		}
		else if (mDirection == Direction::Right)
		{
			mWalkAnimation->InitFrameByStartEnd(2, 3, 3, 3, false);
			mCurrentAnimation = mWalkAnimation;
		}
	}
	else if (mState == EnemyState::ATTACK)
	{
		mAttackAnimation->InitFrameByVector(vector<pair<int, int>>());
		if (mDirection == Direction::Down)
		{
			mAttackAnimation->InitFrameByStartEnd(0, 0, 1, 0, false);
			mCurrentAnimation = mAttackAnimation;
		}
		else if (mDirection == Direction::Up)
		{
			mAttackAnimation->InitFrameByStartEnd(0, 1, 1, 1, false);
			mCurrentAnimation = mAttackAnimation;
		}
		else if (mDirection == Direction::Left)
		{
			mAttackAnimation->InitFrameByStartEnd(0, 2, 1, 2, false);
			mCurrentAnimation = mAttackAnimation;
		}
		else if (mDirection == Direction::Right)
		{
			mAttackAnimation->InitFrameByStartEnd(0, 3, 1, 3, true);
			mCurrentAnimation = mAttackAnimation;
		}
	}
	else if (mState == EnemyState::DEAD)
	{
		mDeadAnimation->InitFrameByVector(vector<pair<int, int>>());
		if (mDirection == Direction::Down)
		{
			mDeadAnimation->InitFrameByStartEnd(4, 0, 4, 0, false);
			mCurrentAnimation = mDeadAnimation;
		}
		else if (mDirection == Direction::Up)
		{
			mDeadAnimation->InitFrameByStartEnd(4, 0, 4, 1, false);
			mCurrentAnimation = mDeadAnimation;
		}
		else if (mDirection == Direction::Left)
		{ 
			mDeadAnimation->InitFrameByStartEnd(4, 0, 4, 2, false);
			mCurrentAnimation = mDeadAnimation;
		}
		else if (mDirection == Direction::Right)
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
		if (mDirection == Direction::Down)
		{
			mIdleAnimation->InitFrameByStartEnd(0, 0, 0, 0, false);
			mCurrentAnimation = mIdleAnimation;
		}
		else if (mDirection == Direction::Up)
		{
			mIdleAnimation->InitFrameByStartEnd(0, 1, 0, 1, false);
			mCurrentAnimation = mIdleAnimation;
		}
		else if (mDirection == Direction::Left)
		{
			mIdleAnimation->InitFrameByStartEnd(0, 2, 0, 2, false);
			mCurrentAnimation = mIdleAnimation;
		}
		else if (mDirection == Direction::Right)
		{
			mIdleAnimation->InitFrameByStartEnd(0, 3, 0, 3, false);
			mCurrentAnimation = mIdleAnimation;
		}
	}
	else if (mState == EnemyState::WALK)
	{
		mWalkAnimation->InitFrameByVector(vector<pair<int, int>>());
		if (mDirection == Direction::Down)
		{
			mWalkAnimation->InitFrameByStartEnd(2, 0, 3, 0, false);
			mCurrentAnimation = mWalkAnimation;
		}
		else if (mDirection == Direction::Up)
		{
			mWalkAnimation->InitFrameByStartEnd(2, 1, 3, 1, false);
			mCurrentAnimation = mWalkAnimation;
		}
		else if (mDirection == Direction::Left)
		{
			mWalkAnimation->InitFrameByStartEnd(2, 2, 3, 2, false);
			mCurrentAnimation = mWalkAnimation;
		}
		else if (mDirection == Direction::Right)
		{
			mWalkAnimation->InitFrameByStartEnd(2, 3, 3, 3, false);
			mCurrentAnimation = mWalkAnimation;
		}
	}
	else if (mState == EnemyState::ATTACK)
	{
		mAttackAnimation->InitFrameByVector(vector<pair<int, int>>());
		if (mDirection == Direction::Down)
		{
			mAttackAnimation->InitFrameByStartEnd(0, 0, 1, 0, false);
			mCurrentAnimation = mAttackAnimation;
		}
		else if (mDirection == Direction::Up)
		{
			mAttackAnimation->InitFrameByStartEnd(0, 1, 1, 1, false);
			mCurrentAnimation = mAttackAnimation;
		}
		else if (mDirection == Direction::Left)
		{
			mAttackAnimation->InitFrameByStartEnd(0, 2, 1, 2, false);
			mCurrentAnimation = mAttackAnimation;
		}
		else if (mDirection == Direction::Right)
		{
			mAttackAnimation->InitFrameByStartEnd(0, 3, 1, 3, true);
			mCurrentAnimation = mAttackAnimation;
		}
	}
	else if (mState == EnemyState::DEAD)
	{
		mDeadAnimation->InitFrameByVector(vector<pair<int, int>>());
		if (mDirection == Direction::Down)
		{
			mDeadAnimation->InitFrameByStartEnd(4, 0, 4, 0, false);
			mCurrentAnimation = mDeadAnimation;
		}
		else if (mDirection == Direction::Up)
		{
			mDeadAnimation->InitFrameByStartEnd(4, 0, 4, 1, false);
			mCurrentAnimation = mDeadAnimation;
		}
		else if (mDirection == Direction::Left)
		{
			mDeadAnimation->InitFrameByStartEnd(4, 0, 4, 2, false);
			mCurrentAnimation = mDeadAnimation;
		}
		else if (mDirection == Direction::Right)
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
					//mItem = mTarget;
					mTarget = nullptr;
					mIsExistTarget = false;

					(*itemPtr).erase((*itemPtr).begin() + i);
				}
			}
			mIsDead = true;
		}

		mState = EnemyState::IDLE;
		SetAnimation();
	}
}

void Sableye::NextDir()
{
	if ((int)(mX / 48) > (int)((mPathFinderList[0]->GetX() + TileSize / 2) / 48))
	{
		if (mState != EnemyState::WALK || mDirection != Direction::Left)
		{
			mState = EnemyState::WALK;
			mDirection = Direction::Left;
			SetAnimation();
		}
	}
	else if ((int)(mX / 48) < (int)((mPathFinderList[0]->GetX() + TileSize / 2) / 48))
	{
		if (mState != EnemyState::WALK || mDirection != Direction::Right)
		{
			mState = EnemyState::WALK;
			mDirection = Direction::Right;
			SetAnimation();
		}
	}

	if ((int)(mY / 48) > (int)((mPathFinderList[0]->GetY() + TileSize / 2) / 48))
	{
		if (mState != EnemyState::WALK || mDirection != Direction::Up)
		{
			mState = EnemyState::WALK;
			mDirection = Direction::Up;
			SetAnimation();
		}
	}
	else if ((int)(mY / 48) < (int)((mPathFinderList[0]->GetY() + TileSize / 2) / 48))
	{
		if (mState != EnemyState::WALK || mDirection != Direction::Down)
		{
			mState = EnemyState::WALK;
			mDirection = Direction::Down;
			SetAnimation();
		}
	}
}
