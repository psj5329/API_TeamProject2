#include "pch.h"
#include "Electrode.h"
#include "Image.h"
#include "Animation.h"
#include "Tile.h"
#include "Trail.h"
#include "Camera.h"
#include "Voltorb.h"

void Electrode::Init(int x, int y)
{
	mName = "Electrode";
	mExplodeImage = IMAGEMANAGER->FindImage(L"explode");
	mImage = IMAGEMANAGER->FindImage(L"Electrode");

	ReadyAnimation();

	//�θ� Ŭ���� (GameObject) ����
	mX = x;
	mY = y;
	mSizeX = mImage->GetFrameWidth() * 2;
	mSizeY = mImage->GetFrameHeight() * 2;
	mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);

	//Electrode ����
	mDirection = Direction::Right;
	mState = State::Sleep;
	mSpeed = 100.f;

	OBJECTMANAGER->AddObject(ObjectLayer::TRAIN, this);

	mCurrentX = mX / TileSize;
	mCurrentY = mY / TileSize;
	mCurrentImage = mImage;
	mCurrentAnimation = mRightMove;
	mCurrentAnimation->Play();


	mStartTimer = 3.5f;
	mSleepTimer = 10.f;
}

void Electrode::Release()
{
	SafeDelete(mDownMove);
	SafeDelete(mUpMove);
	SafeDelete(mLeftMove);
	SafeDelete(mRightMove);
	SafeDelete(mSleep);
	SafeDelete(mExplode);
}

void Electrode::Update()
{
	//�ε��� ��������
	int indexX = mX / TileSize;
	int indexY = mY / TileSize;

	//���� ��Ÿ��
	if (mStartTimer == 3.5)
	{
		if (mState == State::Sleep)
		{
			SetAnimation();
		}
	}
	if (mState == State::Sleep)
	{
		mStartTimer -= TIME->DeltaTime();
	}
	if (mStartTimer < 0 && mState == State::Sleep)
	{
		mState = State::Move;
		SetAnimation();
	}

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

	//�ö��ִ� �������� ���� ������/Ÿ���� �߰����� ����Ȯ��
	//������ ����Ű�� Ÿ���� �߰������̵�
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

	//����
	if (CheckTileEdge() == true)
	{
		//���� �༮�� Ʈ�������� �ƴ��� �Ұ��� ��� �Լ�
		//false�� isexplode	
		if (CheckNextTrailType() == false)
		{
			mNextTrain->SetState(State::Exploding);

			SetIsExplode(true);
		}

		//ispassed�� true�� ���ִ� �Լ�
		//���� (�Ѿ����) Ʈ���Ͽ� ispassed�� üũ�ϴ� �Լ�
		//�� ���� �Լ��� true�� ���¸� isexplode��
		if (CheckNextIsPassed() == true)
		{
			mNextTrain->SetState(State::Exploding);

			SetIsExplode(true);
		}
	}

	//Ǫ��
	if (CheckJigglypuff() == true)
	{
		mSleepTimer = 10.f;
	}
	else
	{
		mSleepTimer -= TIME->DeltaTime();
	}
	if (mSleepTimer <= 5)
	{
		mState = State::Hurt;
		SetAnimation();
	}
	if (mSleepTimer <= 0)
	{
		mState = State::Explode;
		SetAnimation();

		mNextTrain->SetState(State::Exploding);

		SetIsExplode(true);
	}

	if (GetIsExplode() == true && mState != State::Explode)
	{
		mState = State::Explode;
		SetAnimation();
	}


	mCurrentAnimation->Update();
	mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);
}

void Electrode::Render(HDC hdc)
{
	//RenderRect(hdc, mRect);
	//mCurrentImage->ScaleFrameRender(hdc, mRect.left, mRect.top, mCurrentAnimation->GetNowFrameX(), mCurrentAnimation->GetNowFrameY(), mSizeX, mSizeY);
	//CAMERAMANAGER->GetMainCamera()->RenderRectCam(hdc, mRect);
	CAMERAMANAGER->GetMainCamera()->ScaleFrameRender(hdc, mCurrentImage, mRect.left, mRect.top, mCurrentAnimation->GetNowFrameX(), mCurrentAnimation->GetNowFrameY(), mSizeX, mSizeY);

	GIZMO->DrawRectInCamera(hdc, mTrailList[mTargetY][mTargetX]->GetRect(), Gizmo::Color::Blue);
}

void Electrode::ReadyAnimation()
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

	mDownHurt = new Animation();
	mDownHurt->InitFrameByStartEnd(0, 4, 0, 4, false);
	mDownHurt->SetIsLoop(false);
	mDownHurt->SetFrameUpdateTime(0.8f);

	mUpHurt = new Animation();
	mUpHurt->InitFrameByStartEnd(1, 4, 1, 4, false);
	mUpHurt->SetIsLoop(false);
	mUpHurt->SetFrameUpdateTime(0.8f);

	mLeftHurt = new Animation();
	mLeftHurt->InitFrameByStartEnd(0, 5, 0, 5, false);
	mLeftHurt->SetIsLoop(false);
	mLeftHurt->SetFrameUpdateTime(0.8f);

	mRightHurt = new Animation();
	mRightHurt->InitFrameByStartEnd(1, 5, 1, 5, false);
	mRightHurt->SetIsLoop(false);
	mRightHurt->SetFrameUpdateTime(0.8f);
}

void Electrode::SetAnimation()
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

	if (mState == State::Hurt)
	{
		if (mDirection == Direction::Down)
		{
			mCurrentAnimation->Stop();
			mCurrentAnimation = mDownHurt;
			mCurrentAnimation->Play();
		}
		if (mDirection == Direction::Up)
		{

			mCurrentAnimation->Stop();
			mCurrentAnimation = mUpHurt;
			mCurrentAnimation->Play();
		}
		if (mDirection == Direction::Left)
		{
			mCurrentAnimation->Stop();
			mCurrentAnimation = mLeftHurt;
			mCurrentAnimation->Play();

		}
		if (mDirection == Direction::Right)
		{
			mCurrentAnimation->Stop();
			mCurrentAnimation = mRightHurt;
			mCurrentAnimation->Play();
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

//void Electrode::SetImage(int i)
//{
//	if (i == 0) // 0�� false
//	{
//		mCurrentImage = IMAGEMANAGER->FindImage(L"Electrode");
//	}
//	else
//	{
//		mCurrentImage = IMAGEMANAGER->FindImage(L"Voltorb");
//	}
//}

void Electrode::EndExplode()
{
	if (mState == State::Explode)
	{
		SetIsDestroy(true);
	}
}