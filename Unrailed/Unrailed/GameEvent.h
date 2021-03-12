#pragma once

class IEvent
{
public:
	virtual void Start() = 0;
	virtual bool Update() = 0;
	virtual void Render(HDC hdc) = 0;
};

class IChangeCameraTargetEvent : public IEvent
{
	class GameObject* mTarget;
public:
	IChangeCameraTargetEvent(class GameObject* target);

	void Start()override;
	bool Update()override;
};

class IMoveCameraEvent : public IEvent
{
	float mX;
	float mY;
public:
	IMoveCameraEvent(float x, float y);

	void Start()override;
	bool Update()override;
	void Render(HDC hdc)override;
};

class IDelayEvent : public IEvent
{
	float mCurrentTime;
	float mDelayTime;
public:
	IDelayEvent(float delayTime);

	void Start()override;
	bool Update()override;
};

class IScriptEvent : public IEvent
{
	// wstring mScript;
	Image* mImage;
	float mCurrentTime;
	float mDelayTime;
	float mShakeX;
	float mShakeY;
public:
	IScriptEvent(wstring image);

	void Start()override;
	bool Update()override;
	void Render(HDC hdc)override;
};

class IMoveGameObject : public IEvent
{
	class GameObject* mObject;
	float mTargetX;
	float mTargetY;
	float mSpeedX;
	float mSpeedY;
public:
	IMoveGameObject(GameObject* object, GameObject* target);
	IMoveGameObject(GameObject* object, float x, float y);
	IMoveGameObject(GameObject* object, float x, float y, float speedX, float speedY);

	void Start()override;
	bool Update()override;
	void Render(HDC hdc)override;
};

//class IChangeAnimation : public IEvent
//{
//	class GameObject* mObject;
//	class Animation* mAnimation;
//public:
//	IChangeAnimation(GameObject* object, Animation* animation);
//
//	void Start()override;
//	bool Update()override;
//	void Render(HDC hdc)override;
//};

class IObjectStop : public IEvent
{
	bool mIsStop;

public:
	IObjectStop(bool b);

	void Start()override;
	bool Update()override;
	void Render(HDC hdc)override;
};

class IPlaySoundEffect : public IEvent
{
	wstring mName;
	wstring mPath;
	float mDelay;
	float mTime;
	float mVolume;

public:
	IPlaySoundEffect(wstring name, wstring path, float volume, float delay);

	void Start()override;
	bool Update()override;
	void Render(HDC hdc)override;
};