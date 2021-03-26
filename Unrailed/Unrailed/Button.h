#pragma once

class Image;
class Button final
{
public:
	enum class State : int
	{
		Normal, Push, EffectScroll, BackgroundScroll
	};

private:
	function<void(void)> mFunc;	//버튼눌렸을때 실행될 함수
	float mX;
	float mY;
	float mSizeX;
	float mSizeY;
	RECT mRect;
	State mState;
	wstring mText;
	Image* mImage;
	
	static bool mIsEffectScroll;
	static bool mIsBackgroundScroll;

public:
	Button(Image* image,float x, float y, float sizeX, float sizeY, function<void(void)> func);

	void Update();
	void Render(HDC hdc);
	void ScaleRender(HDC hdc, float x, float y);	// 배율

	void SetX(float x) { mX = x; }
	void SetY(float y) { mY = y; }
	void SetState(State s) { mState = s; }

	float GetX() { return mX; }
	float GetY() { return mY; }
	float GetSizeX() { return mSizeX; }
	float GetSizeY() { return mSizeY; }
	
	static void SetIsEffectScroll(bool b) { mIsEffectScroll = b; }
	static void SetIsBackgroundScroll(bool b) { mIsBackgroundScroll = b; }

	static bool GetIsEffectScroll() { return mIsEffectScroll; }
	static bool GetIsBackgroundScroll() { return mIsBackgroundScroll; }
};
