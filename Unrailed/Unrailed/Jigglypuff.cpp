#include "pch.h"
#include "Jigglypuff.h"
#include "Image.h"
#include "Animation.h"
#include "Camera.h"

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
	mState = JigglyState::Idle;


	if (!mIsUpgraded)
	{
		mImage = IMAGEMANAGER->FindImage(L"Jigglypuff");
		mSizeX = mImage->GetFrameWidth() * 2;
		mSizeY = mImage->GetFrameHeight() * 2;
		mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);
	}

	OBJECTMANAGER->AddObject(ObjectLayer::ITEM, this);

	InitSongRect();

	AnimationInit();
	mCurrentAnimation = mSing;
	mCurrentAnimation->Play();

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

	UpdateSongRect();
}

void Jigglypuff::Render(HDC hdc)
{
	CAMERAMANAGER->GetMainCamera()->ScaleFrameRender(hdc, mImage, mRect.left, mRect.top, mCurrentAnimation->GetNowFrameX(), mCurrentAnimation->GetNowFrameY(), mSizeX, mSizeY);
	
	RenderSongRect(hdc);
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
	int size = 12;
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

	mSongRadius = 140;
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
	return IntersectRectToCircle(rect, mX, mY, mSongRadius);
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

void Jigglypuff::SearchMike()
{

}

void Jigglypuff::StartSearchingMike()
{

}