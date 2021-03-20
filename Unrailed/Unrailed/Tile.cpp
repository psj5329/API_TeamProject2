#include "pch.h"
#include "Tile.h"
#include "Image.h"
#include "Camera.h"

Tile::Tile(Image * image, float x, float y, float sizeX, float sizeY, int frameIndexX, int frameIndexY,int type)
{
	mImage = image;
	mX = x;
	mY = y;
	mSizeX = sizeX;
	mSizeY = sizeY;
	mRect = RectMake(mX, mY, mSizeX, mSizeY);
	mFrameIndexX = frameIndexX;
	mFrameIndexY = frameIndexY;
	mTileType = (TileType)type;
}

void Tile::Render(HDC hdc)
{

	if (mImage != nullptr)
		CAMERAMANAGER->GetMainCamera()->ScaleFrameRender(hdc, mImage, mRect.left, mRect.top, mFrameIndexX, mFrameIndexY, mSizeX, mSizeY);
		//mImage->ScaleFrameRender(hdc, mRect.left, mRect.top,mFrameIndexX, mFrameIndexY, mSizeX, mSizeY);

	switch (mTileType)
	{
	case TileType::Normal:
		Gizmo::GetInstance()->DrawRect(hdc, mRect, Gizmo::Color::Green);
		break;

	case TileType::Wall:
		Gizmo::GetInstance()->DrawRect(hdc, mRect, Gizmo::Color::Black);
		break;

	case TileType::Water:
		Gizmo::GetInstance()->DrawRect(hdc, mRect, Gizmo::Color::Blue);
		break;

	case TileType::Lava:
		Gizmo::GetInstance()->DrawRect(hdc, mRect, Gizmo::Color::Red);
		break;

	case TileType::ice:
		Gizmo::GetInstance()->DrawRect(hdc, mRect, Gizmo::Color::Gray);
		break;
	default:
		break;
	}
}
