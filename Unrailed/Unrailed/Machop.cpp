#include "pch.h"
#include "Machop.h"
#include "Image.h"
#include "Animation.h"
#include "Tile.h"
#include "Trail.h"
#include "Camera.h"

void Machop::Init(int x, int y, int image)
{
	mName = "Machop";
	mExplodeImage = IMAGEMANAGER->FindImage(L"Explode");
	mImage = IMAGEMANAGER->FindImage(L"Machop");

	ReadyAnimation();
	SetImage(image);

	//부모 클래스 (GameObject) 변수
	mX = x;
	mY = y;
	mSizeX = mImage->GetFrameWidth() * 2;
	mSizeY = mImage->GetFrameHeight() * 2;
	mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);

	//Machop 변수
	mDirection = Direction::Right;
	mState = State::Move;
	mSpeed = 100.f;
	mOreCount = 0;

	OBJECTMANAGER->AddObject(ObjectLayer::TRAIN, this);

	mCurrentX = mX / TileSize;
	mCurrentY = mY / TileSize;
	mCurrentImage = mImage;
	mCurrentAnimation = mRightMove;
	mCurrentAnimation->Play();
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
	SafeDelete(mExplode);
}

void Machop::Update()
{
	int indexX = mX / TileSize;
	int indexY = mY / TileSize;

	InterceptOre();

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
	if (mState == State::Move || mState == State::Intercept)
	{
		mX += mSpeedX * Time::GetInstance()->DeltaTime() / 2;
		mY += mSpeedY * Time::GetInstance()->DeltaTime() / 2;
	}
	if (CheckTile() == true)
	{
		SetTarget();
	}


	//폭발
	if (GetIsExplode() == true && mState != State::Explode)
	{
		mState = State::Explode;
		SetAnimation();
	}

	mCurrentAnimation->Update();
	mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);
}

void Machop::Render(HDC hdc)
{
	//RenderRect(hdc, mRect);
	//mCurrentImage->ScaleFrameRender(hdc, mRect.left, mRect.top, mCurrentAnimation->GetNowFrameX(), mCurrentAnimation->GetNowFrameY(), mSizeX, mSizeY);
	CAMERAMANAGER->GetMainCamera()->RenderRectCam(hdc, mRect);
	CAMERAMANAGER->GetMainCamera()->ScaleFrameRender(hdc, mCurrentImage, mRect.left, mRect.top, mCurrentAnimation->GetNowFrameX(), mCurrentAnimation->GetNowFrameY(), mSizeX, mSizeY);

	wstring strOre;
	for (int i = 0; i < mOreList.size(); ++i)
	{
		if (mOreList[i]->GetOreType() == ItemType::Green)
			strOre = L"그린" + to_wstring(mOreCount);
		else if (mOreList[i]->GetOreType() == ItemType::Blue)
			strOre = L"블루" + to_wstring(mOreCount);
		else if (mOreList[i]->GetOreType() == ItemType::Red)
			strOre = L"레드" + to_wstring(mOreCount);

		TextOut(hdc, mX - 20, mY - 40 - i * 15, strOre.c_str(), strOre.length());
	}

/*	wstring strBlue = L"블루:" + to_wstring(mOreCount);
	TextOut(hdc, mX - 20, mY - 55, strBlue.c_str(), strBlue.length());

	wstring strRed = L"레드:" + to_wstring(mOreCount);
	TextOut(hdc, mX - 20, mY - 70, strRed.c_str(), strRed.length());*/
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

	mExplode = new Animation();
	mExplode->InitFrameByStartEnd(0, 0, 6, 0, false);
	mExplode->SetIsLoop(false);
	mExplode->SetFrameUpdateTime(0.1f);
	mExplode->SetCallbackFunc(bind(&Train::EndExplode, this));

	mDownIntercept = new Animation();
	mDownIntercept->InitFrameByStartEnd(0, 6, 2, 6, false);
	mDownIntercept->SetIsLoop(false);
	mDownIntercept->SetFrameUpdateTime(0.15f);
	mDownIntercept->SetCallbackFunc(bind(&Machop::EndIntercept, this));

	mUpIntercept = new Animation();
	mUpIntercept->InitFrameByStartEnd(0, 7, 2, 7, false);
	mUpIntercept->SetIsLoop(false);
	mUpIntercept->SetFrameUpdateTime(0.15f);
	mUpIntercept->SetCallbackFunc(bind(&Machop::EndIntercept, this));

	mLeftIntercept = new Animation();
	mLeftIntercept->InitFrameByStartEnd(0, 8, 2, 8, false);
	mLeftIntercept->SetIsLoop(false);
	mLeftIntercept->SetFrameUpdateTime(0.15f);
	mLeftIntercept->SetCallbackFunc(bind(&Machop::EndIntercept, this));

	mRightIntercept = new Animation();
	mRightIntercept->InitFrameByStartEnd(0, 9, 2, 9, false);
	mRightIntercept->SetIsLoop(false);
	mRightIntercept->SetFrameUpdateTime(0.15f);
	mRightIntercept->SetCallbackFunc(bind(&Machop::EndIntercept, this));
}

void Machop::SetAnimation()
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

	if (mState == State::Intercept)
	{
		if (mDirection == Direction::Down)
		{
			mCurrentAnimation->Stop();
			mCurrentAnimation = mDownIntercept;
			mCurrentAnimation->Play();
		}
		if (mDirection == Direction::Up)
		{

			mCurrentAnimation->Stop();
			mCurrentAnimation = mUpIntercept;
			mCurrentAnimation->Play();
		}
		if (mDirection == Direction::Left)
		{
			mCurrentAnimation->Stop();
			mCurrentAnimation = mLeftIntercept;
			mCurrentAnimation->Play();

		}
		if (mDirection == Direction::Right)
		{
			mCurrentAnimation->Stop();
			mCurrentAnimation = mRightIntercept;
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

void Machop::InterceptOre()
{
	if (mOreCount <= 5)
	{
		if (INPUT->GetKeyDown('A'))
		{
			Ore* ore = new Ore;

			mOreCount++;

			ore->SetOreType(ItemType::Green);//그린

			mOreList.push_back(ore);

			mState = State::Intercept;
			SetAnimation();
		}
		if (INPUT->GetKeyDown('S'))
		{
			Ore* ore = new Ore;

			mOreCount++;

			ore->SetOreType(ItemType::Blue);//블루

			mOreList.push_back(ore);

			mState = State::Intercept;
			SetAnimation();
		}
		if (INPUT->GetKeyDown('D'))
		{
			Ore* ore = new Ore;

			mOreCount++;

			ore->SetOreType(ItemType::Red);//레드

			mOreList.push_back(ore);

			mState = State::Intercept;
			SetAnimation();
		}
	}
}

void Machop::InterceptOre(ItemType itemType)
{
	if (mOreCount <= 5)
	{
		Ore* ore = new Ore();
		++mOreCount;
		ore->SetOreType(itemType);
		mOreList.push_back(ore);
		mState = State::Intercept;
		SetAnimation();
	}
}

void Machop::EndIntercept()
{
	if (mState == State::Intercept)
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

void Machop::SetImage(int level)
{
	switch (level)
	{
	case 1:
		mCurrentImage = IMAGEMANAGER->FindImage(L"Machop");
		break;
	case 2:
		mCurrentImage = IMAGEMANAGER->FindImage(L"Machoke");
		break;
	case 3:
		mCurrentImage = IMAGEMANAGER->FindImage(L"Machamp");
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

void Machop::EndExplode()
{
	if (mState == State::Explode)
	{
		SetIsDestroy(true);
	}
}

ItemType Machop::OreErase()
{
	ItemType type = (ItemType)mOreList[0]->GetOreTypeInt();
	 mOreList.erase(mOreList.begin(), mOreList.begin() + 2); 
	 
	 return type;
}