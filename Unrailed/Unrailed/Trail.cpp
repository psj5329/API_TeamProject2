#include "pch.h"
#include "Trail.h"
#include "Image.h"
#include "Animation.h"
#include "Camera.h"

void Trail::Init(int x, int y, int type, int direction)
{
    mTrailType = (TrailType)type;
    mDirection = (TrailDirection)direction;

    //이미지정하기
    if (mTrailType == TrailType::Green)
    {
        mImage = IMAGEMANAGER->FindImage(L"GreenTrail");
    }
    else if (mTrailType == TrailType::Blue)
    {
        mImage = IMAGEMANAGER->FindImage(L"BlueTrail");
    }
    else 
    {
        mImage = IMAGEMANAGER->FindImage(L"RedTrail");
    }

        
    mX = x;
    mY = y;
    mSizeX = mImage->GetFrameWidth() * 2;
    mSizeY = mImage->GetFrameHeight() * 2;
    mRect = RectMake(mX, mY, mSizeX, mSizeY);
    mIsConnected = false;
    mIsTail = false;

    AnimationInit();

    if (mDirection == TrailDirection::Down)
    {
        mCurrentAnimation = mDown;
    }
    else if (mDirection == TrailDirection::Up)
    {
        mCurrentAnimation = mUp;
    }
    else if (mDirection == TrailDirection::Left)
    {
        mCurrentAnimation = mLeft;
    }
    else
    {
        mCurrentAnimation = mRight;
    }
    mCurrentAnimation->Play();
}

void Trail::Release()
{
    SafeDelete(mUp);
    SafeDelete(mDown);
    SafeDelete(mLeft);
    SafeDelete(mRight);
    SafeDelete(mCurrentAnimation);
}

void Trail::Update()
{

    //연결된 친구들만 애니메이션 움직이게
    if (mIsConnected)
    {
        mCurrentAnimation->Update();
    }
}

void Trail::Render(HDC hdc)
{
    if(mTrailType != TrailType::None)
	CAMERAMANAGER->GetMainCamera()->ScaleFrameRender(hdc, mImage, mRect.left, mRect.top, mCurrentAnimation->GetNowFrameX(), mCurrentAnimation->GetNowFrameY(), mSizeX, mSizeY);
}

//플레이어의 돌리기
void Trail::Turn()
{
	switch (mDirection)
	{
    case TrailDirection::Down:
        mDirection = TrailDirection::Left;
        mCurrentAnimation->Stop();
        mCurrentAnimation = mLeft;
        mCurrentAnimation->Play();
        break;
    case TrailDirection::Up:
        mDirection = TrailDirection::Right;
        mCurrentAnimation->Stop();
        mCurrentAnimation = mRight;
        mCurrentAnimation->Play();
        break;
    case TrailDirection::Left:
        mDirection = TrailDirection::Up;
        mCurrentAnimation->Stop();
        mCurrentAnimation = mUp;
        mCurrentAnimation->Play();
        break;
    case TrailDirection::Right:
        mDirection = TrailDirection::Down;
        mCurrentAnimation->Stop();
        mCurrentAnimation = mDown;
        mCurrentAnimation->Play();	
        break;
    default:
        break;
	}

}

void Trail::AnimationInit()
{
    mDown = new Animation();
    mDown->InitFrameByStartEnd(0, 1, 3, 1, false);
    mDown->SetIsLoop(true);
    mDown->SetFrameUpdateTime(0.2f);

    mUp = new Animation();
    mUp->InitFrameByStartEnd(0, 0, 3, 0, false);
    mUp->SetIsLoop(true);
    mUp->SetFrameUpdateTime(0.2f);

    mLeft = new Animation();
    mLeft->InitFrameByStartEnd(0, 3, 3, 3, false);
    mLeft->SetIsLoop(true);
    mLeft->SetFrameUpdateTime(0.2f);

    mRight = new Animation();
    mRight->InitFrameByStartEnd(0, 2, 3, 2, false);
    mRight->SetIsLoop(true);
    mRight->SetFrameUpdateTime(0.2f);

}

int Trail::PickUp()
{
    mIsActive = false;
    mIsDestroy = true;
    return (int)mTrailType;
}

//속성바꾸기
void Trail::SetTrailType(int type) 
{ 
    mTrailType = (TrailType)type; 
    //이미지정하기
    if (mTrailType == TrailType::Green)
    {
        mImage = IMAGEMANAGER->FindImage(L"GreenTrail");
    }
    else if (mTrailType == TrailType::Blue)
    {
        mImage = IMAGEMANAGER->FindImage(L"BlueTrail");
    }
    else
    {
        mImage = IMAGEMANAGER->FindImage(L"RedTrail");
    }
}

//방향바꾸기
void Trail::SetDirection(int dir) 
{ 
    mDirection = (TrailDirection)dir; 

    mCurrentAnimation->Stop();
    switch (mDirection)
    {
    case TrailDirection::Down:
        mCurrentAnimation = mDown;
        break;
    case TrailDirection::Up:
        mCurrentAnimation = mUp;
        break;
    case TrailDirection::Left:
        mCurrentAnimation = mLeft;
        break;
    case TrailDirection::Right:
        mCurrentAnimation = mRight;
        break;
    default:
        break;
    }
    mCurrentAnimation->Play();

}