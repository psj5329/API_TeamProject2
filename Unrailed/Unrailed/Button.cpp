#include "pch.h"
#include "Button.h"
#include "Image.h"

Button::Button(Image* image,float x, float y, float sizeX, float sizeY, function<void(void)> func)
{
	mImage = image;
	mX = x;
	mY = y;
	mSizeX = sizeX;
	mSizeY = sizeY;
	mRect = RectMakeCenter((int)mX, (int)mY, (int)mSizeX, (int)mSizeY);
	mFunc = func;
	mState = State::Normal;
}

void Button::Update()
{
	if (mState == State::Normal)
	{
		if (Input::GetInstance()->GetKeyDown(VK_LBUTTON))
		{
			if (PtInRect(&mRect, _mousePosition))
			{
				mState = State::Push;
			}
		}
	}
	else
	{
		if (Input::GetInstance()->GetKeyUp(VK_LBUTTON))
		{
			mState = State::Normal;
			if (mFunc != nullptr)
			{
				mFunc();
			}
		}

	}
}

void Button::Render(HDC hdc)
{
	//if(mState == State::Normal)
	//	RenderRect(hdc, mRect);
	//else
	//{
	//	float sizeX = mSizeX * 0.75f;
	//	float sizeY = mSizeY * 0.75f;
	//	RECT rc = RectMakeCenter(mX, mY, sizeX, sizeY);
	//	RenderRect(hdc, rc);
	//}

	TextOut(hdc, mX - mSizeX / 4, mY - mSizeY / 4, mText.c_str(), (int)mText.length());
	if (mState == State::Normal)
		mImage->ScaleRender(hdc, mRect.left, mRect.top, (int)mSizeX, (int)mSizeY);
	else
	{
		float sizeX = mSizeX * 0.75f;
		float sizeY = mSizeY * 0.75f;
		mImage->ScaleRender(hdc, mRect.left, mRect.top, (int)sizeX, (int)sizeY);
	}
}