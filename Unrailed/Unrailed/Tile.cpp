#include "pch.h"
#include "Tile.h"
#include "Image.h"

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
		mImage->ScaleFrameRender(hdc, mRect.left, mRect.top,mFrameIndexX, mFrameIndexY, mSizeX, mSizeY);
	if (mTileType == TileType::Normal)
	{
		Gizmo::GetInstance()->DrawRect(hdc, mRect, Gizmo::Color::Blue);
	}
	else if(mTileType==TileType::Wall)
	{
		Gizmo::GetInstance()->DrawRect(hdc, mRect, Gizmo::Color::Red);
	}
	else
	{
		Gizmo::GetInstance()->DrawRect(hdc, mRect, Gizmo::Color::Green);
	}
	//RenderRect(hdc, mRect);
}
