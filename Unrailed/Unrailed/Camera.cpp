#include "pch.h"
#include "Camera.h"

#include "Image.h"
#include "Scene.h"
#include "Electrode.h"

void Camera::Init()
{
	mMode = Mode::Free;
	mSizeX = WINSIZEX;
	mSizeY = WINSIZEY;
	mRect = RectMakeCenter((int)mX, (int)mY, (int)mSizeX, (int)mSizeY);
	mMoveSpeed = 5.f;
}

void Camera::Release()
{
}

void Camera::Update()
{
	switch (mMode)
	{
	case Camera::Mode::Follow:
		if (mTarget)
		{
			//mX = Math::Lerp(mX, mTarget->GetX(), 2.f * Time::GetInstance()->DeltaTime());
			//mY = Math::Lerp(mY, mTarget->GetY(), 2.f * Time::GetInstance()->DeltaTime());

			float x = SCENEMANAGER->GetCurrentScene()->GetSizeX();
			float y = SCENEMANAGER->GetCurrentScene()->GetSizeY();

			// 오른쪽으로
			if (dynamic_cast<Electrode*>(mTarget)->GetDirection() == Direction::Right)
			{
				if (mTarget->GetX() <= WINSIZEX / 2 - WINSIZEX / 10)
					mX = Math::Lerp(mX, WINSIZEX / 2, 2.f * Time::GetInstance()->DeltaTime());
				else if (mTarget->GetX() >= x - WINSIZEX / 2 - WINSIZEX / 10)
					mX = Math::Lerp(mX, x - WINSIZEX / 2, 2.f * Time::GetInstance()->DeltaTime());
				else	mX = Math::Lerp(mX, mTarget->GetX() + WINSIZEX / 10, 2.f * Time::GetInstance()->DeltaTime());
			}
			else if (dynamic_cast<Electrode*>(mTarget)->GetDirection() == Direction::Left) // 왼쪽
			{
				if (mTarget->GetX() <= WINSIZEX / 2 + WINSIZEX / 10)
					mX = Math::Lerp(mX, WINSIZEX / 2, 2.f * Time::GetInstance()->DeltaTime());
				else if (mTarget->GetX() >= x - WINSIZEX / 2 + WINSIZEX / 10)
					mX = Math::Lerp(mX, x - WINSIZEX / 2, 2.f * Time::GetInstance()->DeltaTime());
				else
					mX = Math::Lerp(mX, mTarget->GetX() - WINSIZEX / 10, 2.f * Time::GetInstance()->DeltaTime());
			}

			// 위쪽으로
			//if (dynamic_cast<Electrode*>(mTarget)->GetDirection() == Direction::Up)
			{
				if (mTarget->GetY() <= WINSIZEY / 2 - WINSIZEY / 10)
					mY = Math::Lerp(mY, WINSIZEY / 2, 2.f * Time::GetInstance()->DeltaTime());
				else if (mTarget->GetY() >= y - WINSIZEY / 2 - WINSIZEY / 10)
					mY = Math::Lerp(mY, y - WINSIZEY / 2, 2.f * Time::GetInstance()->DeltaTime());
				else
					mY = Math::Lerp(mY, mTarget->GetY() + WINSIZEY / 10, 2.f * Time::GetInstance()->DeltaTime());
			}
			//else if (dynamic_cast<Electrode*>(mTarget)->GetDirection() == Direction::Down)	// 아래쪽
			{
				if (mTarget->GetY() <= WINSIZEY / 2 + WINSIZEY / 10)
					mY = Math::Lerp(mY, WINSIZEY / 2, 2.f * Time::GetInstance()->DeltaTime());
				else if (mTarget->GetY() >= y - WINSIZEY / 2 + WINSIZEY / 10)
					mY = Math::Lerp(mY, y - WINSIZEY / 2, 2.f * Time::GetInstance()->DeltaTime());
				else
					mY = Math::Lerp(mY, mTarget->GetY() - WINSIZEY / 10, 2.f * Time::GetInstance()->DeltaTime());
			}

			mRect = RectMakeCenter((int)mX, (int)mY, (int)mSizeX, (int)mSizeY);
		}
		break;
	case Camera::Mode::Free:
		if (Input::GetInstance()->GetKey('A'))mX -= mMoveSpeed;
		if (Input::GetInstance()->GetKey('D'))mX += mMoveSpeed;
		if (Input::GetInstance()->GetKey('W'))mY -= mMoveSpeed;
		if (Input::GetInstance()->GetKey('S'))mY += mMoveSpeed;
		mRect = RectMakeCenter((int)mX, (int)mY, (int)mSizeX, (int)mSizeY);
		break;
	}
}

void Camera::Render(HDC hdc)
{
}

void Camera::Render(HDC hdc, Image * image, int x, int y)
{
	image->Render(hdc, x - mRect.left, y - mRect.top);
}

void Camera::Render(HDC hdc, Image * image, int x, int y, int tempX, int tempY, int tempWidth, int tempHeight)
{
	image->Render(hdc, x - mRect.left, y - mRect.top, tempX, tempY, tempWidth, tempHeight);
}

void Camera::FrameRender(HDC hdc, Image * image, int x, int y, int frameX, int frameY)
{
	image->FrameRender(hdc, x - mRect.left, y - mRect.top, frameX, frameY);
}

void Camera::AlphaRender(HDC hdc, Image * image, int x, int y, float alpha)
{
	image->AlphaRender(hdc, x - mRect.left, y - mRect.top, alpha);
}

void Camera::AlphaFrameRender(HDC hdc, Image * image, int x, int y, int frameX, int frameY, float alpha)
{
	image->AlphaFrameRender(hdc, x - mRect.left, y - mRect.top, frameX, frameY, alpha);
}

void Camera::ScaleRender(HDC hdc, Image * image, int x, int y, int width, int height)
{
	image->ScaleRender(hdc, x - mRect.left, y - mRect.top, width, height);
}

void Camera::ScaleFrameRender(HDC hdc, Image * image, int x, int y, int frameX, int frameY, int width, int height)
{
	image->ScaleFrameRender(hdc, x - mRect.left, y - mRect.top, frameX, frameY, width, height);
}

void Camera::AlphaScaleRender(HDC hdc, Image * image, int x, int y, int width, int height, float alpha)
{
	image->AlphaScaleRender(hdc, x - mRect.left, y - mRect.top, width, height, alpha);
}

void Camera::AlphaScaleFrameRender(HDC hdc, Image * image, int x, int y, int frameX, int frameY, int width, int height, float alpha)
{
	image->AlphaScaleFrameRender(hdc, x - mRect.left, y - mRect.top, frameX, frameY, width, height, alpha);
}

void Camera::RenderRectCam(HDC hdc, RECT rc)
{
	rc.left -= mRect.left;
	rc.right -= mRect.left;
	rc.top -= mRect.top;
	rc.bottom -= mRect.top;
	RenderRect(hdc, rc);
}

void Camera::RenderEllipseCam(HDC hdc, float x, float y, float radius)
{
	RenderEllipse(hdc, x - mRect.left, y - mRect.top, radius);
}

bool Camera::IsInCameraArea(float x, float y, float width, float height)
{
	RECT rc = RectMakeCenter((int)x, (int)y, (int)width, (int)height);
	return IsInCameraArea(rc);
}

bool Camera::IsInCameraArea(RECT rc)
{
	if (rc.right < mRect.left)return false;
	if (rc.left > mRect.right)return false;
	if (rc.bottom < mRect.top)return false;
	if (rc.top > mRect.bottom)return false;

	return true;
}
