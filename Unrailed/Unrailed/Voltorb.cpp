#include "pch.h"
#include "Voltorb.h"
#include "Image.h"
#include "Animation.h"
#include "Tile.h"
#include "Trail.h"
#include "Camera.h"

void Voltorb::Init(int x, int y)
{
	mName = "Voltorb";
	mExplodeImage = IMAGEMANAGER->FindImage(L"explode");
	mImage = IMAGEMANAGER->FindImage(L"Voltorb");
	mElectrode = (Electrode*)OBJECTMANAGER->FindObject("Electrode");

	ReadyAnimation();

	//부모 클래스 (GameObject) 변수
	mX = x;
	mY = y;
	mSizeX = mImage->GetFrameWidth() * 2;
	mSizeY = mImage->GetFrameHeight() * 2;
	mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);
	//SetImage(image);

	//Electrode 변수
	mDirection = Direction::Right;
	mState = State::Sleep;

	OBJECTMANAGER->AddObject(ObjectLayer::TRAIN, this);

	mCurrentX = mX / TileSize;
	mCurrentY = mY / TileSize;
	mCurrentImage = mImage;
	mCurrentAnimation = mRightMove;
	mCurrentAnimation->Play();

	mExplosionTimer = 0.3;
	mStartTimer = 30.f;

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


	//시작 쿨타임
	if (mStartTimer == 30)
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


	//올라가있는 기차길의 현재 기차길/타일의 중간오면 방향확인
	//방향이 가리키는 타일의 중간까지이동

	SetSpeed();
	if (mState == State::Move || mState == State::Exploding || mState == State::Hurt)
	{
		mX += mSpeedX * Time::GetInstance()->DeltaTime() / 2;
		mY += mSpeedY * Time::GetInstance()->DeltaTime() / 2;
	}
	if (CheckTile() == true)
	{
		SetTarget();
	}

	//푸린
	if (mIsHurt == false && mElectrode->GetIsHurt() == true && mState != State::Exploding && mState != State::Explode)
	{
		mState = State::Hurt;
		SetAnimation();

		mIsHurt = true;
	}
	else if (mElectrode->GetIsHurt() == false && mState != State::Sleep && mState != State::Exploding && mState != State::Explode)
	{
		mState = State::Move;
		SetAnimation();

		mIsHurt = false;
	}

	//폭발
	if (mState == State::Exploding)
	{
		mExplosionTimer -= TIME->DeltaTime();
		if (mExplosionTimer < 0)
		{
			mIsExplode = true;
			if (mNextTrain != nullptr)
				mNextTrain->SetState(State::Exploding);
		}
	}
	if (GetIsExplode() == true && mState != State::Explode)
	{
		mState = State::Explode;
		SetAnimation();
	}


	mCurrentAnimation->Update();
	mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);
}

void Voltorb::Render(HDC hdc)
{
	CAMERAMANAGER->GetMainCamera()->ScaleFrameRender(hdc, mCurrentImage, mRect.left, mRect.top, mCurrentAnimation->GetNowFrameX(), mCurrentAnimation->GetNowFrameY(), mSizeX, mSizeY);
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

	mDownHurt = new Animation();
	mDownHurt->InitFrameByStartEnd(0, 4, 1, 4, false);
	mDownHurt->SetIsLoop(true);
	mDownHurt->SetFrameUpdateTime(0.2f);

	mUpHurt = new Animation();
	mUpHurt->InitFrameByStartEnd(2, 4, 3, 4, false);
	mUpHurt->SetIsLoop(true);
	mUpHurt->SetFrameUpdateTime(0.2f);

	mLeftHurt = new Animation();
	mLeftHurt->InitFrameByStartEnd(0, 5, 1, 5, false);
	mLeftHurt->SetIsLoop(true);
	mLeftHurt->SetFrameUpdateTime(0.2f);

	mRightHurt = new Animation();
	mRightHurt->InitFrameByStartEnd(2, 5, 3, 5, false);
	mRightHurt->SetIsLoop(true);
	mRightHurt->SetFrameUpdateTime(0.2f);
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

void Voltorb::EndExplode()
{
	if (mState == State::Explode)
	{
		//SetIsDestroy(true); //삭제
		SetIsActive(false); //렌더 안함
	}
}