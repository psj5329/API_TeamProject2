#include "pch.h"
#include "GameEvent.h"

#include "Camera.h"
#include "Image.h"

IChangeCameraTargetEvent::IChangeCameraTargetEvent(GameObject * target)
{
	mTarget = target;
}

void IChangeCameraTargetEvent::Start()
{
	CameraManager::GetInstance()->GetMainCamera()->SetTarget(mTarget);
}

bool IChangeCameraTargetEvent::Update()
{
	float x = CameraManager::GetInstance()->GetMainCamera()->GetX();
	float y = CameraManager::GetInstance()->GetMainCamera()->GetY();

	if (Math::GetDistance(x, y, mTarget->GetX(), mTarget->GetY()) <= 5.0f)
	{
		return true;
	}

	return false;
}

IDelayEvent::IDelayEvent(float delayTime)
{
	mDelayTime = delayTime;
	mCurrentTime = 0.f;
}

void IDelayEvent::Start()
{
}

bool IDelayEvent::Update()
{
	mCurrentTime += Time::GetInstance()->DeltaTime();

	if (mCurrentTime >= mDelayTime)
	{
		return true;
	}

	return false;
}

IScriptEvent::IScriptEvent(wstring image)
{
	mImage = IMAGEMANAGER->FindImage(image);
	mDelayTime = 2.f;
	mCurrentTime = 0.f;
}

void IScriptEvent::Start()
{
}

bool IScriptEvent::Update()
{
	mCurrentTime += Time::GetInstance()->DeltaTime();

	mShakeX = rand() % (2 * 2 + 1) - 2;
	mShakeY = rand() % (2 * 2 + 1) - 2;

	if (mCurrentTime >= mDelayTime)// || */INPUT->GetKeyDown(VK_SPACE))
	{
		return true;
	}

	return false;
}

void IScriptEvent::Render(HDC hdc)
{
	CameraManager::GetInstance()->GetMainCamera()->Render(hdc, mImage
		, WINSIZEX / 2 - mImage->GetWidth() / 2 + CameraManager::GetInstance()->GetMainCamera()->GetRect().left - mShakeX
		, WINSIZEY / 2 - mImage->GetHeight() / 2 + CameraManager::GetInstance()->GetMainCamera()->GetRect().top - mShakeY);
}

IMoveGameObject::IMoveGameObject(GameObject* object, GameObject * target)
{
	mObject = object;
	mTargetX = target->GetX() + target->GetSizeX() + mObject->GetSizeX();
	mTargetY = target->GetY();
	mSpeedX = (mTargetX - mObject->GetX()) * 0.1f * Time::GetInstance()->DeltaTime();
	mSpeedY = (mTargetY - mObject->GetY()) * 0.1f * Time::GetInstance()->DeltaTime();
}

IMoveGameObject::IMoveGameObject(GameObject* object, float x, float y)
{
	mObject = object;
	mTargetX = x;
	mTargetY = y;
	mSpeedX = (mTargetX - mObject->GetX()) * 0.1f * Time::GetInstance()->DeltaTime();
	mSpeedY = (mTargetY - mObject->GetY()) * 0.1f * Time::GetInstance()->DeltaTime();
}

IMoveGameObject::IMoveGameObject(GameObject * object, float x, float y, float speedX, float speedY)
{
	mObject = object;
	mTargetX = x;
	mTargetY = y;
	mSpeedX = speedX * Time::GetInstance()->DeltaTime();
	mSpeedY = speedY * Time::GetInstance()->DeltaTime();
}

void IMoveGameObject::Start()
{
}

bool IMoveGameObject::Update()
{
	mObject->SetX(mObject->GetX() + mSpeedX);
	mObject->SetY(mObject->GetY() + mSpeedY);

	if (Math::GetDistance(mObject->GetX(), mObject->GetY(), mTargetX, mTargetY) <= 5.0f)
	{
		return true;
	}

	return false;
}

void IMoveGameObject::Render(HDC hdc)
{
}

IObjectStop::IObjectStop(bool b)
{
	mIsStop = b;
}

void IObjectStop::Start()
{
}

bool IObjectStop::Update()
{
	/*OBJECTMANAGER->GetPlayer()->SetEvent(mIsStop);
	((Boss*)OBJECTMANAGER->FindObject("Boss"))->SetEvent(mIsStop);*/
	return true;
}

void IObjectStop::Render(HDC hdc)
{
}

IPlaySoundEffect::IPlaySoundEffect(wstring name, wstring path, float volume, float delay)
{
	mName = name;
	mPath = path;
	mDelay = delay;
	mTime = 0.f;
	mVolume = volume;
}

void IPlaySoundEffect::Start()
{
}

bool IPlaySoundEffect::Update()
{
	mTime += TIME->DeltaTime();

	if (mTime >= mDelay)
	{
		SOUNDMANAGER->Play(mName, mVolume);

		return true;
	}

	return false;
}

void IPlaySoundEffect::Render(HDC hdc)
{
}

IClearStage::IClearStage()
{
}

void IClearStage::Start()
{
	mImage = IMAGEMANAGER->FindImage(L"Clear");
	mX = WINSIZEX / 2 - mImage->GetWidth() / 2;
	mY = WINSIZEY / 2 - mImage->GetHeight() / 2;
	mDelayTime = 0.f;
	mCount = 0;
}

bool IClearStage::Update()
{
	mDelayTime += TIME->DeltaTime();
	
	if (mDelayTime >= 0.3f)
	{
		mDelayTime = 0.f;
		// 효과음 내고시파 한 글자마다
		mCount++;

		if (mCount >= 7)
			mCount = 5;
	}

	//mCount++;

	if (INPUT->GetKeyDown(VK_SPACE) && mCount >= 5)
		return true;

	return false;
}

void IClearStage::Render(HDC hdc)
{
	mImage->FrameRender(hdc, mX, mY, 0, 0, mCount, 1);
}

IGameoverStage::IGameoverStage()
{
}

void IGameoverStage::Start()
{
	mImage = IMAGEMANAGER->FindImage(L"Gameover");
	mX = WINSIZEX / 2 - mImage->GetWidth() / 2;
	mY = WINSIZEY / 2 - mImage->GetHeight() / 2;
	mDelayTime = 0.f;
	mCount = 0;
}

bool IGameoverStage::Update()
{
	mDelayTime += TIME->DeltaTime();

	if (mDelayTime >= 0.2f)
	{
		mDelayTime = 0.f;
		// 효과음 내고시파 한 글자마다
		mCount++;

		if (mCount >= 9)
			mCount = 8;
	}

	//mCount++;

	if (INPUT->GetKeyDown(VK_SPACE) && mCount >= 8)
		return true;

	return false;
}

void IGameoverStage::Render(HDC hdc)
{
	mImage->FrameRender(hdc, mX, mY, 0, 0, mCount, 1);
}
