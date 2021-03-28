#include "pch.h"
#include "MapToolReady.h"
#include "Image.h"
#include "Button.h"

void MapToolReady::Init()
{
	mBackgroundImg = IMAGEMANAGER->FindImage(L"MapReady");
	Image* newi = IMAGEMANAGER->FindImage(L"New");
	Image* load = IMAGEMANAGER->FindImage(L"Load");
	Image* numbers = IMAGEMANAGER->FindImage(L"Numbers");
	mSetArrow = IMAGEMANAGER->FindImage(L"RightArrow");
	mBackground = RectMake(0, 0, mBackgroundImg->GetFrameWidth() * 2, mBackgroundImg->GetFrameHeight() * 2);

	mXTileCount = 10;
	mYTileCount = 10;
	mYTens = 1;
	mYOnes = 0;
	mXTens = 1;
	mXOnes = 0;
	mCounter = 0;
	mFinish = false;

	mInputY = L"TileCountY :  " + to_wstring(mYTileCount);
	mInputX = L"TileCountX :  " + to_wstring(mXTileCount);
	mInputBoxY = RectMake(mBackground.left + 60, mBackground.top + 370, 120, 20);
	mInputBoxX = RectMake(mBackground.left + 60, mBackground.top + 410, 120, 20);

	mCurrentInput = true;
	mArrow = RectMake(mBackground.left+30, mBackground.top + 370, 2, 2);

	mSaveButton = new Button(newi, mBackground.right - 150, mBackground.top+ 430, newi->GetFrameWidth(), newi->GetFrameHeight(), bind(&MapToolReady::CreateNew, this));
	mLoadButton = new Button(load, mBackground.right - 70, mBackground.top+ 430, load->GetFrameWidth(), load->GetFrameHeight(), bind(&MapToolReady::Load, this));

	for (int i = 0;i < 10;i++)
	{
		NumberPad numb;
		numb.image = numbers;
		numb.sizeX = numbers->GetFrameWidth()*2;
		numb.sizeY = numbers->GetFrameHeight()*2;
		numb.frameY = 0;
		numb.frameX = i;
		numb.rect = RectMake(mBackground.left + 200 + numb.sizeX * i, mBackground.top + 340, numb.sizeX, numb.sizeY);
		mNumberPad.push_back(numb);
	}
	
}

void MapToolReady::Release()
{
	SafeDelete(mSaveButton);
	SafeDelete(mLoadButton);
}

void MapToolReady::Update()
{
	SetCurrentInput();

	//지우기
	if (INPUT->GetKeyDown(VK_BACK))
	{
		if (mCurrentInput)
		{
			mYTens = 0;
			mYOnes = 0;
		}
		else
		{
			mXTens = 0;
			mXOnes = 0;
		}
		mCounter = 0;
	}

	if (INPUT->GetKeyDown(VK_LBUTTON))
	{
		for (int i = 0;i < mNumberPad.size();i++)
		{
			//숫자누르면
			if (PtInRect(&mNumberPad[i].rect, _mousePosition))
			{
				//Y변경
				if (mCurrentInput)
				{
					if (mCounter == 0)
					{
						mYTens = i;
						mCounter++;
					}
					else if (mCounter == 1)
					{
						mYOnes = i;
						mCounter++;
						SetTileCounts();
					}
				}
				//x변경
				else
				{
					if (mCounter == 0)
					{
						mXTens = i;
						mCounter++;
					}
					else if (mCounter == 1)
					{
						mXOnes = i;
						mCounter++;
						SetTileCounts();
					}
				}
			}
		}

	}




	mInputY = L"TileCountY :  " + to_wstring(mYTens) + to_wstring(mYOnes);
	mInputX = L"TileCountX :  " + to_wstring(mXTens) + to_wstring(mXOnes);

	mSaveButton->Update();
	if (mFinish)
	{
		return;
	}
	mLoadButton->Update();
}

void MapToolReady::Render(HDC hdc)
{
	//뒷배경
	RenderBackBackground(hdc);

	//배경
	mBackgroundImg->ScaleRender(hdc, 0, 0, mBackgroundImg->GetFrameWidth() * 2, mBackgroundImg->GetFrameHeight() * 2);

	//RenderRect(hdc, mInputBoxX);
	//RenderRect(hdc, mInputBoxY);

	//입력칸
	TextOut(hdc, mInputBoxY.left, mInputBoxY.top, mInputY.c_str(), (int)mInputY.length());
	TextOut(hdc, mInputBoxX.left, mInputBoxX.top, mInputX.c_str(), (int)mInputX.length());

	wstring y = L"TileY: " + to_wstring(mYTileCount);
	TextOut(hdc, _mousePosition.x, _mousePosition.y, y.c_str(), (int)y.length());
	wstring x = L"TileX: " + to_wstring(mXTileCount);
	TextOut(hdc, _mousePosition.x, _mousePosition.y + 20, x.c_str(), (int)x.length());


	//화살표
	mSetArrow->ScaleRender(hdc, mArrow.left, mArrow.top, mSetArrow->GetFrameWidth(), mSetArrow->GetFrameHeight());

	//숫자
	for (int i = 0;i < mNumberPad.size();i++)
	{
		mNumberPad[i].image->ScaleFrameRender(hdc, mNumberPad[i].rect.left, mNumberPad[i].rect.top,mNumberPad[i].frameX,mNumberPad[i].frameY,mNumberPad[i].sizeX,mNumberPad[i].sizeY);
	}

	//세이브로드 버튼
	mSaveButton->Render(hdc);
	mLoadButton->Render(hdc);
}

void MapToolReady::Load()
{
	SCENEMANAGER->LoadMapToolScene();

}

void MapToolReady::CreateNew()
{
	mFinish = true;
	//SCENEMANAGER->LoadScene(L"MapToolScene");
	SCENEMANAGER->SetMapToolScene(mYTileCount, mXTileCount);
}

LRESULT MapToolReady::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	int input = wParam;
	mInputX = input;
	return 0;
}

//Y바꿀지 X바꿀지
void MapToolReady::SetCurrentInput()
{
	if (INPUT->GetKeyDown(VK_LBUTTON))
	{
		if (PtInRect(&mInputBoxY, _mousePosition))
		{
			mCurrentInput = true;
			mCounter = 0;
		}
		else if (PtInRect(&mInputBoxX, _mousePosition))
		{
			mCurrentInput = false;
			mCounter = 0;
		}
	}

	if (mCurrentInput)
	{
		mArrow = RectMake(mBackground.left + 30, mBackground.top + 370, 2, 2);
	}
	else
	{
		mArrow = RectMake(mBackground.left + 30, mBackground.top + 410, 2, 2);
	}
}

void MapToolReady::SetTileCounts()
{
	mYTileCount = mYTens * 10 + mYOnes;
	mXTileCount = mXTens * 10 + mXOnes;
}

void MapToolReady::RenderBackBackground(HDC hdc)
{
	//배경
	RECT rc = RectMakeCenter(WINSIZEX / 2, WINSIZEY / 2, WINSIZEX, WINSIZEY);
	HPEN yellowPen = CreatePen(PS_SOLID, 3, RGB(255, 236, 204));
	HPEN prevPen = (HPEN)SelectObject(hdc, yellowPen);
	HBRUSH colorBrush = CreateSolidBrush(RGB(255, 236, 204));
	HBRUSH prevBrush = (HBRUSH)SelectObject(hdc, colorBrush);
	RenderRect(hdc, rc);
	SelectObject(hdc, prevBrush);
	DeleteObject(colorBrush);
	DeleteObject(yellowPen);
}