#include "pch.h"
#include "Tile.h"
#include "Image.h"
#include "Camera.h"

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
}

void Tile::Render(HDC hdc)
{

	if (mImage != nullptr)
		CAMERAMANAGER->GetMainCamera()->ScaleFrameRender(hdc, mImage, mRect.left, mRect.top, mFrameIndexX, mFrameIndexY, mSizeX, mSizeY);
		//mImage->ScaleFrameRender(hdc, mRect.left, mRect.top,mFrameIndexX, mFrameIndexY, mSizeX, mSizeY);

	if (mCoveImage != nullptr)
	{

		
		CAMERAMANAGER->GetMainCamera()->AlphaScaleFrameRender(hdc, mCoveImage, mRect.left, mRect.top, mFrameIndexX, mFrameIndexY, mSizeX, mSizeY,mAlpha);

		mAlpha += mAlphaRate * TIME->DeltaTime();
		if (mAlpha > 1)
			mAlpha = 0;
		
	}


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


void Tile::SetCoverImage()
{
	if (wcscmp(mImage->GetKeyName().c_str(), L"MagmaCavern") == 0)
	{
		mCoveImage = IMAGEMANAGER->FindImage(L"MagmaCavernCover");
	}
	else if (wcscmp(mImage->GetKeyName().c_str(), L"LushPrairie") == 0)
	{
		mCoveImage = IMAGEMANAGER->FindImage(L"LushPrairieCover");
	}
	else
	{
		mCoveImage = nullptr;
	}
}