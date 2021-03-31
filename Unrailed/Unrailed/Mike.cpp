#include "pch.h"
#include "Mike.h"
#include "Image.h"
#include "Camera.h"

void Mike::Init()
{
	mName = "Mike";
	mImage = IMAGEMANAGER->FindImage(L"Mic");
	
	mState = MikeState::Jigglypuff;
	mSizeX = mImage->GetFrameWidth() * 2;
	mSizeY = mImage->GetFrameHeight() * 2;
	
	OBJECTMANAGER->AddObject(ObjectLayer::ITEM, this);
	
	mIsActive = false;

}

void Mike::Release()
{

}

void Mike::Update()
{

}

void Mike::Render(HDC hdc)
{
	CAMERAMANAGER->GetMainCamera()->ScaleRender(hdc, mImage, mRect.left, mRect.top, mSizeX, mSizeY);
}

//플레이어가 마이크 놓기
bool Mike::PlaceMikeFloat(float x, float y)
{
	if (mState == MikeState::OnGround)
		return false;

	mX = x + 8;
	mY = y + 8;
	mRect = RectMake(mX, mY, mSizeX, mSizeY);
	mState = MikeState::OnGround;
	mIsActive = true;

	return true;
}

//플레이어가 마이크 놓기
bool Mike::PlaceMike(int indexX, int indexY)
{
	if (mState == MikeState::OnGround)
		return false;

	//mIndexX = (mX + (CAMERAMANAGER->GetMainCamera()->GetX() - WINSIZEX / 2)) / TileSize;
	//mIndexY = (mY + (CAMERAMANAGER->GetMainCamera()->GetY() - WINSIZEY / 2)) / TileSize;

	mX = indexX * TileSize + 8;
	mY = indexY * TileSize + 8;

	mRect = RectMake(mX, mY, mSizeX, mSizeY);
	mState = MikeState::OnGround;
	mIsActive = true;

	return true;
}

//플레이어가 마이크줍기
bool Mike::PickUpMikePlayer(int indexX, int indexY)
{
	if (mState != MikeState::OnGround)
		return false;

	mState = MikeState::Player;
	mIsActive = false;

	return true;
}

//푸린이 마이크줍기
bool Mike::PickUpMikeJigglypff()
{
	if (mState != MikeState::OnGround)
		return false;

	mState = MikeState::Jigglypuff;
	mIsActive = false;

	return true;
}