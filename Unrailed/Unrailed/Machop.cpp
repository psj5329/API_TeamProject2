#include "pch.h"
#include "Machop.h"
#include "Image.h"
#include "Animation.h"

void Machop::Init()
{
	IMAGEMANAGER->LoadFromFile(L"Machop", Resources(L"/Train/machop"), 92, 260, 4, 10, true);
	IMAGEMANAGER->LoadFromFile(L"Machoke", Resources(L"/Train/machoke"), 128, 256, 4, 8, true);
	IMAGEMANAGER->LoadFromFile(L"Machamp", Resources(L"/Train/machamp"), 124, 320, 4, 10, true);
	mImage = IMAGEMANAGER->FindImage(L"Machop");

	ReadyAnimation();

	//�θ� Ŭ���� (GameObject) ����
	mX = WINSIZEX / 2 - 135;
	mY = WINSIZEY / 2;
	mSizeX = mImage->GetFrameWidth() * 2;
	mSizeY = mImage->GetFrameHeight() * 2;
	mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);

	//Machop ����
	mDirection = Direction::Right;
	mState = State::Sleep;
	mSpeed = 100.f;
	mTimer = 0;
	mStop = false;
	mLevel = 1;

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
}

void Machop::Update()
{
	//�������ϱ�
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

	//������
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

	//��ȭ
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


	mCurrentAnimation->Update();
}

void Machop::Render(HDC hdc)
{
	mImage->ScaleFrameRender(hdc, mX, mY, mCurrentAnimation->GetNowFrameX(), mCurrentAnimation->GetNowFrameY(), mSizeX, mSizeY);
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
	}

	mCurrentAnimation->Play();
}