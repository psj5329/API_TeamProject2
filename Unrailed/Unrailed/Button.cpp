#include "pch.h"
#include "Button.h"
#include "Image.h"

bool Button::mIsEffectScroll = false;
bool Button::mIsBackgroundScroll = false;

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
	else if(mState == State::Push)
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
	else if (mState == State::BackgroundScroll || mState == State::EffectScroll)
	{
		if (Input::GetInstance()->GetKeyDown(VK_LBUTTON) || Input::GetInstance()->GetKey(VK_LBUTTON))
		{
			if (PtInRect(&mRect, _mousePosition))
			{
				if (mState == State::BackgroundScroll)
					mIsBackgroundScroll = true;
				else if (mState == State::EffectScroll)
					mIsEffectScroll = true;

				if (mFunc != nullptr)
				{
					mFunc();
				}
			}
		}
		else if (Input::GetInstance()->GetKeyUp(VK_LBUTTON))
		{
			//mState = State::Normal;
			mIsBackgroundScroll = false;
			mIsEffectScroll = false;
		}
	}

	mRect = RectMakeCenter((int)mX, (int)mY, (int)mSizeX, (int)mSizeY);
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

	mImage->ScaleRender(hdc, mRect.left, mRect.top, (int)mSizeX, (int)mSizeY);

}

void Button::ScaleRender(HDC hdc, float x, float y)
{
	if (mState == State::Normal)
		mImage->ScaleRender(hdc, mRect.left, mRect.top, (int)mSizeX, (int)mSizeY);
	else
	{
		float sizeX = mSizeX * x;
		float sizeY = mSizeY * y;
		mImage->ScaleRender(hdc, mRect.left + mSizeX * (1.f - x) / 2.f, mRect.top + mSizeY * (1.f - y) / 2.f, (int)sizeX, (int)sizeY);
	}
}
