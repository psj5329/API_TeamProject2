#pragma once

// GameObject 직접적으로 상속
enum ObjectType { UNITTYPE, ITEMTYPE, UITYPE, TRAILTYPE, MAPOBJECTTYPE, OBJECTTYPEEND };

class GameObject
{
protected:
	string mName;
	float mX;
	float mY;
	float mSizeX;
	float mSizeY;
	RECT mRect;
	ObjectType mObjectType;

	bool mIsActive;
	bool mIsDestroy;

	bool mIsPlayEvent;	// 이벤트 중일 떄 오브젝트들 애니메이션 제외 멈춤

public:
	GameObject();
	GameObject(const string& name, float x, float y);

	virtual void Init() = 0;
	virtual void Release() = 0;
	virtual void Update() = 0;
	virtual void Render(HDC hdc) = 0;

public:
	// Get
	inline string GetName() { return mName; }
	inline float GetX() { return mX; }
	inline float GetY() { return mY; }
	inline float GetSizeX() { return mSizeX; }
	inline float GetSizeY() { return mSizeY; }
	inline RECT GetRect() { return mRect; }

	inline bool GetIsActive() { return mIsActive; }
	inline bool GetIsDestroy() { return mIsDestroy; }
	inline bool GetIsPlayEvent() { return mIsPlayEvent; }

	// Set
	inline void SetName(string name) { mName = name; }
	inline void SetX(float x) { mX = x; }
	inline void SetY(float y) { mY = y; }
	inline void SetSizeX(float x) { mSizeX = x; }
	inline void SetSizeY(float y) { mSizeY = y; }
	inline void SetRect(RECT rc) { mRect = rc; }

	inline void SetIsActive(bool b) { mIsActive = b; }
	inline void SetIsDestroy(bool b) { mIsDestroy = b; }
	inline void SetIsPlayEvent(bool b) { mIsPlayEvent = b; }
};

