#include "pch.h"
#include "Abra.h"
#include "Image.h"
#include "Animation.h"
#include "Tile.h"
#include "Trail.h"
#include "Camera.h"

void Abra::Init(int x, int y, int image)
{
	mExplodeImage = IMAGEMANAGER->FindImage(L"Explode");
	mImage = IMAGEMANAGER->FindImage(L"Abra");
	mMachop = (Machop*)OBJECTMANAGER->FindObject("Machop");

	ReadyAnimation();
	SetImage(image);

	//부모 클래스 (GameObject) 변수
	mX = x;
	mY = y;
	mSizeX = mImage->GetFrameWidth() * 2.f;
	mSizeY = mImage->GetFrameHeight() * 2.f;
	mRect = RectMakeCenter((int)mX, (int)mY, (int)mSizeX, (int)mSizeY);

	//Abra 변수
	mDirection = Direction::Right;
	mState = State::Move;
	mSpeed = 100.f;
	mSynthesisCoolTime = 0;
	mIsSynthesis = false;
	mTrailCount = 0;

	OBJECTMANAGER->AddObject(ObjectLayer::TRAIN, this);

	mCurrentX = mX / TileSize;
	mCurrentY = mY / TileSize;
	mCurrentImage = mImage;
	mCurrentAnimation = mRightMove;
	mCurrentAnimation->Play();

	mOreBroken = false;
}

void Abra::Release()
{
	SafeDelete(mDownMove);
	SafeDelete(mUpMove);
	SafeDelete(mLeftMove);
	SafeDelete(mRightMove);
	SafeDelete(mLeftSleep);
	SafeDelete(mRightSleep);
	SafeDelete(mDownSynthesis);
	SafeDelete(mUpSynthesis);
	SafeDelete(mLeftSynthesis);
	SafeDelete(mRightSynthesis);
	SafeDelete(mExplode);
}

void Abra::Update()
{
	int indexX = (int)(mX / TileSize);
	int indexY = (int)(mY / TileSize);

	SynthesisOre();

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

	//움직임
	//if (mState == State::Sleep)
	//{
	//	mTimer += Time::GetInstance()->DeltaTime();
	//}

	SetSpeed();
	if (mState == State::Move || mState == State::Synthesis)
	{
		mX += mSpeedX * Time::GetInstance()->DeltaTime() / 2;
		mY += mSpeedY * Time::GetInstance()->DeltaTime() / 2;
	}
	if (CheckTile() == true)
	{
		SetTarget();
	}

	if (GetIsExplode() == true && mState != State::Explode)
	{
		mState = State::Explode;
		SetAnimation();
	}

	mCurrentAnimation->Update();
	mRect = RectMakeCenter((int)mX, (int)mY, (int)mSizeX, (int)mSizeY);
}

void Abra::Render(HDC hdc)
{
	//RenderRect(hdc, mRect);
	//mCurrentImage->ScaleFrameRender(hdc, mRect.left, mRect.top, mCurrentAnimation->GetNowFrameX(), mCurrentAnimation->GetNowFrameY(), mSizeX, mSizeY);
	CAMERAMANAGER->GetMainCamera()->RenderRectCam(hdc, mRect);
	CAMERAMANAGER->GetMainCamera()->ScaleFrameRender(hdc, mCurrentImage, mRect.left, mRect.top, mCurrentAnimation->GetNowFrameX(), mCurrentAnimation->GetNowFrameY(), mSizeX, mSizeY);

	wstring strTrail;
	for (int i = 0; i < mCreatedTrailList.size(); ++i)
	{
		if (mCreatedTrailList[i]->trailType == ItemType::Green)
			strTrail = L"그린" + to_wstring(mTrailCount);
		else if (mCreatedTrailList[i]->trailType == ItemType::Blue)
			strTrail = L"블루" + to_wstring(mTrailCount);
		else if (mCreatedTrailList[i]->trailType == ItemType::Red)
			strTrail = L"레드" + to_wstring(mTrailCount);

		TextOut(hdc, mX - 20, mY - 40 - i * 15, strTrail.c_str(), strTrail.length());
	}
}

void Abra::ReadyAnimation()
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

	mDownSynthesis = new Animation();
	mDownSynthesis->InitFrameByStartEnd(0, 6, 1, 6, false);
	mDownSynthesis->SetIsLoop(false);
	mDownSynthesis->SetFrameUpdateTime(0.8f);
	mDownSynthesis->SetCallbackFunc(bind(&Abra::EndSynthesis, this));

	mUpSynthesis = new Animation();
	mUpSynthesis->InitFrameByStartEnd(2, 6, 3, 6, false);
	mUpSynthesis->SetIsLoop(false);
	mUpSynthesis->SetFrameUpdateTime(0.8f);
	mUpSynthesis->SetCallbackFunc(bind(&Abra::EndSynthesis, this));

	mLeftSynthesis = new Animation();
	mLeftSynthesis->InitFrameByStartEnd(0, 7, 1, 7, false);
	mLeftSynthesis->SetIsLoop(false);
	mLeftSynthesis->SetFrameUpdateTime(0.8f);
	mLeftSynthesis->SetCallbackFunc(bind(&Abra::EndSynthesis, this));

	mRightSynthesis = new Animation();
	mRightSynthesis->InitFrameByStartEnd(2, 7, 3, 7, false);
	mRightSynthesis->SetIsLoop(false);
	mRightSynthesis->SetFrameUpdateTime(0.8f);
	mRightSynthesis->SetCallbackFunc(bind(&Abra::EndSynthesis, this));
}

void Abra::SetAnimation()
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

	if (mState == State::Synthesis)
	{
		if (mDirection == Direction::Down)
		{
			mCurrentAnimation->Stop();
			mCurrentAnimation = mDownSynthesis;
			mCurrentAnimation->Play();
		}
		if (mDirection == Direction::Up)
		{

			mCurrentAnimation->Stop();
			mCurrentAnimation = mUpSynthesis;
			mCurrentAnimation->Play();
		}
		if (mDirection == Direction::Left)
		{
			mCurrentAnimation->Stop();
			mCurrentAnimation = mLeftSynthesis;
			mCurrentAnimation->Play();

		}
		if (mDirection == Direction::Right)
		{
			mCurrentAnimation->Stop();
			mCurrentAnimation = mRightSynthesis;
			mCurrentAnimation->Play();
		}
	}

	if (mState == State::Sleep)
	{
		if (mDirection == Direction::Right)
		{
			mCurrentAnimation->Stop();
			mCurrentAnimation = mRightSleep;
			mCurrentAnimation->Play();
		}
		if (mDirection == Direction::Left)
		{
			mCurrentAnimation->Stop();
			mCurrentAnimation = mLeftSleep;
			mCurrentAnimation->Play();
		}
	}
	if (mState == State::Explode)
	{
		mCurrentAnimation->Stop();
		mCurrentAnimation = mExplode;
		mCurrentAnimation->Play();
		mCurrentImage = mExplodeImage;
	}
}

void Abra::SynthesisOre()
{
	ItemType type = ItemType::None;
	if (mTrailCount <= 2 && mMachop->GetOreList().size() >= 2 && mIsSynthesis == false && mOreBroken == false)
	{
		mState = State::Synthesis;
		SetAnimation();

		type = mMachop->OreErase();
		mMachop->SetOreCount(mMachop->GetOreCount() - 2);

		mIsSynthesis = true;
		mOreBroken = true;
	}
	if (mIsSynthesis == true)
	{
		mSynthesisCoolTime += TIME->DeltaTime();
	}
	if (mOreBroken == true)
	{
		CreatedTrail* createdTrail = new CreatedTrail;
		createdTrail->trailType = type;
		createdTrail->isCreated = false;
		//createdTrail->mImage = 
		mCreatedTrailList.push_back(createdTrail);

		mOreBroken = false;
	}
	if (mSynthesisCoolTime >= 1.5f && mIsSynthesis == true)
	{
		mIsSynthesis = false;
		mSynthesisCoolTime = 0;

		mTrailCount += 1;
	}
}

void Abra::EndSynthesis()
{
	if (mState == State::Synthesis)
	{
		if (mDirection == Direction::Down)
		{
			mState = State::Move;
			SetAnimation();
		}
		if (mDirection == Direction::Up)
		{
			mState = State::Move;
			SetAnimation();
		}
		if (mDirection == Direction::Left)
		{
			mState = State::Move;
			SetAnimation();
		}
		if (mDirection == Direction::Right)
		{
			mState = State::Move;
			SetAnimation();
		}
	}
}

void Abra::SetImage(int level)
{
	switch (level)
	{
	case 1:
		mCurrentImage = IMAGEMANAGER->FindImage(L"Abra");
		break;
	case 2:
		mCurrentImage = IMAGEMANAGER->FindImage(L"Kadabra");
		break;
	case 3:
		mCurrentImage = IMAGEMANAGER->FindImage(L"Alakazam");
		break;
	}
	if (INPUT->GetKeyDown('Q'))
	{
		level = 1;
	}
	if (INPUT->GetKeyDown('W'))
	{
		level = 2;
	}
	if (INPUT->GetKeyDown('E'))
	{
		level = 3;
	}
}

void Abra::EndExplode()
{
	if (mState == State::Explode)
	{
		SetIsDestroy(true);
	}
}

ItemType Abra::Receive()
{
	if (mCreatedTrailList.size() > 0)
	{
		ItemType type = mCreatedTrailList[0]->trailType;
		//벡터에서 첫번째 제거

		return type;
	}
	else
		return ItemType::None;
}