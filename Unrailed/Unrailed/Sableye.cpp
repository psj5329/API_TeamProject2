#include "pch.h"
#include "Sableye.h"

#include "Image.h"
#include "Camera.h"
#include "Animation.h"

void Sableye::Init()
{
	mImage = IMAGEMANAGER->FindImage(L"Sableye");
	mName = "Sableye";
	mX = WINSIZEX / 2;
	mY = WINSIZEY / 2;
	mSizeX = mImage->GetFrameWidth();
	mSizeY = mImage->GetFrameHeight();
	mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);
	mHitBox = RectMakeCenter(mX, mY, mSizeX, mSizeY);
	mState = EnemyState::IDLE;

	mVecItem = OBJECTMANAGER->GetObjectList(ObjectLayer::ITEM);
	mPlayer = OBJECTMANAGER->GetPlayer();
	mTarget = nullptr;

	mIsExistTarget = false;

	ReadyAnimation();
}

void Sableye::Release()
{
}

void Sableye::Update()
{
	mVecItem = OBJECTMANAGER->GetObjectList(ObjectLayer::ITEM);

	float distance = 0.f;
	mIsExistTarget = false;	// ���� ���� false �صΰ� �� ���Ҵµ� false�� ��ó�� ����
							// ���ٰ� �� ����� ���ο�� ��Ÿ���� �׸��� ����?
	
	for (int i = 0; i < mVecItem.size(); ++i)
	{
		float x = mVecItem[i]->GetX();
		float y = mVecItem[i]->GetY();
		
		if (Math::GetDistance(mX, mY, x, y) < 150.f)	// �׽�Ʈ�� �Ÿ�
		{
			if (!mIsExistTarget)
			{
				distance = Math::GetDistance(mX, mY, x, y);

				mTarget = mVecItem[i];
				mIsExistTarget = true;
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

	if (mTarget != nullptr)	// �켱�� �ӽ÷� �ٷ� �������� �����̱�~
	{
		float angle = Math::GetAngle(mX, mY, mTarget->GetX(), mTarget->GetY());

		mX += cosf(angle) * mSpeed * TIME->DeltaTime();
		mY += -sinf(angle) * mSpeed * TIME->DeltaTime();
	}
	
	mCurrentAnimation->Update();
	mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);
	mHitBox = RectMakeCenter(mX, mY, mSizeX, mSizeY);
}

void Sableye::Render(HDC hdc)
{
	CAMERAMANAGER->GetMainCamera()->ScaleFrameRender(hdc, mImage, mX, mY, mCurrentAnimation->GetNowFrameX()
		, mCurrentAnimation->GetNowFrameY(), mSizeX * 2, mSizeY * 2);
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
