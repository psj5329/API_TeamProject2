#include "pch.h"
#include "Tile.h"
#include "Image.h"
#include "Camera.h"
#include "Animation.h"

Tile::Tile(Image* image, Image* coverImage, float x, float y, float sizeX, float sizeY, int frameIndexX, int frameIndexY, int type)
{
	mImage = image;
	mCoveImage = coverImage;
	mX = x;
	mY = y;
	mSizeX = sizeX;
	mSizeY = sizeY;
	mRect = RectMake(mX, mY, mSizeX, mSizeY);
	mFrameIndexX = frameIndexX;
	mFrameIndexY = frameIndexY;
	mTileType = (TileType)type;
	mAlpha = 0.1;
	mAlphaRate = 0.5;

	mCurrentFrameTime = 0;
	mFrameUpdateTime = 0.2;
}

void Tile::Render(HDC hdc)
{
	//카메라 안일때만
	UpdateCoverFrame();
	if (IsInCamera())
	{
		if (mImage != nullptr)
		{
			//CAMERAMANAGER->GetMainCamera().
			CAMERAMANAGER->GetMainCamera()->ScaleFrameRender(hdc, mImage, mRect.left, mRect.top, mFrameIndexX, mFrameIndexY, mSizeX, mSizeY);
		}

		if (mCoveImage != nullptr)
		{	
			if (mTileType == TileType::Lava || mTileType == TileType::Water)
			{
				CAMERAMANAGER->GetMainCamera()->ScaleFrameRender(hdc, mCoveImage, mRect.left, mRect.top, mCoverFrameIndexX, mCoverFrameIndexY, mSizeX, mSizeY);
			}
		}

		if (INPUT->GetKey(VK_CONTROL))
		{
			switch (mTileType)
			{
			case TileType::Normal:
				Gizmo::GetInstance()->DrawRectInCamera(hdc, mRect, Gizmo::Color::Green);
				break;

			case TileType::Wall:
				Gizmo::GetInstance()->DrawRectInCamera(hdc, mRect, Gizmo::Color::Black);
				break;

			case TileType::Water:
				Gizmo::GetInstance()->DrawRectInCamera(hdc, mRect, Gizmo::Color::Blue);
				break;

			case TileType::Lava:
				Gizmo::GetInstance()->DrawRectInCamera(hdc, mRect, Gizmo::Color::Red);
				break;

			case TileType::ice:
				Gizmo::GetInstance()->DrawRectInCamera(hdc, mRect, Gizmo::Color::Gray);
				break;
			default:
				break;
			}
		}
	}

	
}

void Tile::SetCoverImage()
{
	if (wcscmp(mImage->GetKeyName().c_str(), L"MagmaCavern") == 0)
	{
		mCoveImage = IMAGEMANAGER->FindImage(L"MagmaCover");
		mMaxFrame = 276;
	}
	else if (wcscmp(mImage->GetKeyName().c_str(), L"LushPrairie") == 0)
	{
		mCoveImage = IMAGEMANAGER->FindImage(L"WaterCover");
		mMaxFrame = 48;
	}
	else
	{
		mCoveImage = nullptr;
	}
	mCoverFrameIndexX = mFrameIndexX;
	mCoverFrameIndexY = mFrameIndexY;
}

void Tile::UpdateCoverFrame()
{

	float deltaTime = Time::GetInstance()->DeltaTime();

	mCurrentFrameTime += deltaTime;

	if (mCurrentFrameTime >= mFrameUpdateTime)
	{
		//손실 다 없애준다
		while (mCurrentFrameTime >= mFrameUpdateTime)
		{
			mCurrentFrameTime -= mFrameUpdateTime;
		}

		mCoverFrameIndexX += 12;
	}

	if (mCoverFrameIndexX >= mMaxFrame)
	{

		mCoverFrameIndexX = mFrameIndexX;

	}
}

bool Tile::IsInCamera()
{
	RECT temp;
	RECT cam = CAMERAMANAGER->GetMainCamera()->GetRect();
	if (IntersectRect(&temp, &cam, &mRect))
	{
		return true;
	}
	else
	{
		return false;
	}
}