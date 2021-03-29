#include "pch.h"
#include "Jigglypuff.h"
#include "Image.h"
#include "Animation.h"
#include "Camera.h"
#include "Mike.h"
#include "Tile.h"

void Jigglypuff::Init()
{

}

void Jigglypuff::Init(int x, int y, bool isUpgraded)
{

	mX = x;
	mY = y;
	mIsUpgraded = isUpgraded;
	mHoldMike = true;
	mDirection = Direction::Down;
	mState = JigglyState::Sing;
	mSongImage = IMAGEMANAGER->FindImage(L"PinkArea");
	mName = "Jigglypuff";

	if (!mIsUpgraded)
	{
		mImage = IMAGEMANAGER->FindImage(L"Jigglypuff");
		mSpeed = 50;
		mSongRadius = 180;
	}
	else
	{
		mImage = IMAGEMANAGER->FindImage(L"Wigglypuff");
		mSpeed = 30;
		mSongRadius = 200;
	}
	mSizeX = mImage->GetFrameWidth() * 2;
	mSizeY = mImage->GetFrameHeight() * 2;
	mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);

	OBJECTMANAGER->AddObject(ObjectLayer::ITEM, this);

	InitSongRect();

	AnimationInit();
	mCurrentAnimation = mSing;
	mCurrentAnimation->Play();


	//마이크
	mMike = new Mike();
	mMike->Init();
}

void Jigglypuff::Release()
{
	SafeDelete(mIdle);
	SafeDelete(mSing);
	SafeDelete(mUpMove);
	SafeDelete(mDownMove);
	SafeDelete(mLeftMove);
	SafeDelete(mRightMove);
}

void Jigglypuff::Update()
{
	mCurrentAnimation->Update();

	if (mState == JigglyState::Sing)
	{
		UpdateSongRect();
	}
	
	//마이크로가
	if (mState == JigglyState::Move)
	{
		MoveToMike();
	}
}

void Jigglypuff::Render(HDC hdc)
{
	if (mState == JigglyState::Sing)
	{
		CAMERAMANAGER->GetMainCamera()->AlphaScaleRender(hdc, mSongImage, mSongRect.left - 88, mSongRect.top - 88, mSongRadius * 2, mSongRadius * 2, 0.5);
		RenderSongRect(hdc);
	}

	CAMERAMANAGER->GetMainCamera()->ScaleFrameRender(hdc, mImage, mRect.left, mRect.top, mCurrentAnimation->GetNowFrameX(), mCurrentAnimation->GetNowFrameY(), mSizeX, mSizeY);
}


void Jigglypuff::AnimationInit()
{
	mIdle = new Animation();
	mIdle->InitFrameByStartEnd(0, 0, 1, 0, false);
	mIdle->SetIsLoop(true);
	mIdle->SetFrameUpdateTime(0.4f);

	mSing = new Animation();
	mSing->InitFrameByStartEnd(0, 13, 1, 13, false);
	mSing->SetIsLoop(true);
	mSing->SetFrameUpdateTime(0.4f);

	mDownMove = new Animation();
	mDownMove->InitFrameByStartEnd(0, 4, 2, 4, false);
	mDownMove->SetIsLoop(true);
	mDownMove->SetFrameUpdateTime(0.4f);

	mUpMove = new Animation();
	mUpMove->InitFrameByStartEnd(0, 5, 2, 5, false);
	mUpMove->SetIsLoop(true);
	mUpMove->SetFrameUpdateTime(0.4f);

	mLeftMove = new Animation();
	mLeftMove->InitFrameByStartEnd(0, 6, 2, 6, false);
	mLeftMove->SetIsLoop(true);
	mLeftMove->SetFrameUpdateTime(0.4f);

	mRightMove = new Animation();
	mRightMove->InitFrameByStartEnd(0, 7, 2, 7, false);
	mRightMove->SetIsLoop(true);
	mRightMove->SetFrameUpdateTime(0.4f);
}

void Jigglypuff::SetAnimation()
{
	mCurrentAnimation->Stop();

	switch (mState)
	{
	case JigglyState::Sing:
		mCurrentAnimation = mSing;
		break;

	case JigglyState::Search:
		mCurrentAnimation = mIdle;
		break;

	case JigglyState::Idle:
		mCurrentAnimation = mIdle;
		break;

	case JigglyState::Move:
		switch (mDirection)
		{
		case Direction::Down:
			mCurrentAnimation = mDownMove;
			break;
		case Direction::Up:
			mCurrentAnimation = mUpMove;
			break;
		case Direction::Left:
			mCurrentAnimation = mLeftMove;
			break;
		case Direction::Right:
			mCurrentAnimation = mRightMove;
			break;
		default:
			break;
		}

		break;
	default:
		break;
	}
	mCurrentAnimation->Play();
}

void Jigglypuff::InitSongRect()
{
	int size = 14;
	for (int i = 0;i < size;i++)
	{
		Note* tempNote = new Note();
		tempNote->image = IMAGEMANAGER->FindImage(L"Notes");
		tempNote->frameY = 0;
		tempNote->frameX = RANDOM->RandomInt(4);
		tempNote->angle = PI2 / size * i;
		tempNote->sizeX = tempNote->image->GetFrameWidth();
		tempNote->sizeY = tempNote->image->GetFrameHeight();
		mNoteList.push_back(tempNote);
	}

	mSongRect = RectMakeCenter(mX, mY, mSongRadius, mSongRadius);
}

void Jigglypuff::UpdateSongRect()
{
	for (int i = 0;i < mNoteList.size();i++)
	{
		UpdateNotes(mNoteList[i]);
	}
}

void Jigglypuff::RenderSongRect(HDC hdc)
{
	for (int i = 0;i < mNoteList.size();i++)
	{
		Note* temp = mNoteList[i];
		CAMERAMANAGER->GetMainCamera()->ScaleFrameRender(hdc, temp->image, temp->rect.left, temp->rect.top, temp->frameX, temp->frameY, temp->sizeX, temp->sizeY);
	}

	//CAMERAMANAGER->GetMainCamera()->RenderEllipseCam(hdc, mX, mY, mSongRadius);
}

void Jigglypuff::UpdateNotes(Note* note)
{
	note->angle += 0.3f * TIME->DeltaTime();
	note->positionX = mX + cosf(note->angle) * mSongRadius;
	note->positionY = mY - sinf(note->angle) * mSongRadius;
	note->rect = RectMakeCenter(note->positionX, note->positionY, note->sizeX, note->sizeY);
}

bool Jigglypuff::IsInSongRange(RECT rect)
{
	if (mState == JigglyState::Sing)
		return IntersectRectToCircle(rect, mX, mY, mSongRadius);
	else
		return false;
}

bool Jigglypuff::TakeMike()
{
	if (mHoldMike)
	{
		mHoldMike = false;
		mState = JigglyState::Idle;
		SetAnimation();
		return true;
	}
	else
	{
		return false;
	}
}

void Jigglypuff::MoveToMike()
{
	float angle=0;
	if(mPathFinderList.size() != 0)
		angle = Math::GetAngle(mX, mY, mPathFinderList[0]->GetX() + TileSize / 2, mPathFinderList[0]->GetY() + TileSize / 2);

	mX += cosf(angle) * mSpeed * TIME->DeltaTime();
	mY += -sinf(angle) * mSpeed * TIME->DeltaTime();

	if ((int)(mX / TileSize) == (int)(((mPathFinderList[0]->GetX() + TileSize / 2) / TileSize))
		&& (int)(mY / TileSize) == (int)(((mPathFinderList[0]->GetY() + TileSize / 2) / TileSize))
		&& Math::GetDistance(mX, mY, mPathFinderList[0]->GetX() + TileSize / 2, mPathFinderList[0]->GetY() + TileSize / 2) <= 1.f)
	{
		mPathFinderList.erase(mPathFinderList.begin());

		if (mPathFinderList.size() != 0)
		{
			if ((int)(mX / 48) > (int)((mPathFinderList[0]->GetX() + TileSize / 2) / TileSize))
			{
				if (mState != JigglyState::Move || mDirection != Direction::Left)
				{
					mState = JigglyState::Move;
					mDirection = Direction::Left;
					SetAnimation();
				}
			}
			else if ((int)(mX / TileSize) < (int)((mPathFinderList[0]->GetX() + TileSize / 2) / TileSize))
			{
				if (mState != JigglyState::Move || mDirection != Direction::Right)
				{
					mState = JigglyState::Move;
					mDirection = Direction::Right;
					SetAnimation();
				}
			}

			if ((int)(mY / TileSize) > (int)((mPathFinderList[0]->GetY() + TileSize / 2) / TileSize))
			{
				if (mState != JigglyState::Move || mDirection != Direction::Up)
				{
					mState = JigglyState::Move;
					mDirection = Direction::Up;
					SetAnimation();
				}
			}
			else if ((int)(mY / TileSize < (int)((mPathFinderList[0]->GetY() + TileSize / 2) / 48)))
			{
				if (mState != JigglyState::Move || mDirection != Direction::Down)
				{
					mState = JigglyState::Move;
					mDirection = Direction::Down;
					SetAnimation();
				}
			}
		}
		//길이 더 없으면
		else
		{
			mState = JigglyState::Sing;
			mDirection = Direction::Down;
			SetAnimation();
			mSongRect = RectMakeCenter(mX, mY, mSongRadius, mSongRadius);
			mMike->PickUpMikeJigglypff();
			mHoldMike = true;
		}
	}

	mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);

}

//마이크 찾기 시작
void Jigglypuff::StartSearchingMike(int MikeX, int MikeY)
{
	for (int i = 0;i < mPathFinderList.size();i++)
	{
		mPathFinderList.erase(mPathFinderList.begin() + i);
		i--;
	}

	mIndexX = (mX + (CAMERAMANAGER->GetMainCamera()->GetX() - WINSIZEX / 2)) / TileSize;
	mIndexY = (mY + (CAMERAMANAGER->GetMainCamera()->GetY() - WINSIZEY / 2)) / TileSize;
	GameObject* mike = OBJECTMANAGER->FindObject("Mike");
	mPathFinderList = PATHFINDER->FindPath(mTileList, mMapObjectList, mIndexX, mIndexY, MikeX, MikeY);


	if ((int)(mX / 48) > (int)((mPathFinderList[0]->GetX() + TileSize / 2) / TileSize))
	{
		if (mState != JigglyState::Move || mDirection != Direction::Left)
		{
			mState = JigglyState::Move;
			mDirection = Direction::Left;
			SetAnimation();
		}
	}
	else if ((int)(mX / TileSize) < (int)((mPathFinderList[0]->GetX() + TileSize / 2) / TileSize))
	{
		if (mState != JigglyState::Move || mDirection != Direction::Right)
		{
			mState = JigglyState::Move;
			mDirection = Direction::Right;
			SetAnimation();
		}
	}

	if ((int)(mY / TileSize) > (int)((mPathFinderList[0]->GetY() + TileSize / 2) / TileSize))
	{
		if (mState != JigglyState::Move || mDirection != Direction::Up)
		{
			mState = JigglyState::Move;
			mDirection = Direction::Up;
			SetAnimation();
		}
	}
	else if ((int)(mY / TileSize < (int)((mPathFinderList[0]->GetY() + TileSize / 2) / 48)))
	{
		if (mState != JigglyState::Move || mDirection != Direction::Down)
		{
			mState = JigglyState::Move;
			mDirection = Direction::Down;
			SetAnimation();
		}
	}
	
}

void Jigglypuff::PlaceMike(int indexX, int indexY)
{
	mMike->PlaceMike(indexX, indexY);
	StartSearchingMike(indexX, indexY);
}