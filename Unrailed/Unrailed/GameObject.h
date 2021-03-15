#pragma once

// GameObject 직접적으로 상속
// Unit, Item, UI, Trail(설치 타일), MapObject(부실 수 있는 것, 없는 것)

//enum Direction { Left, Right, DirEnd };
// 아이템 등에서는 필요없을거라 게임오브젝트 아래 유닛 클래스 만들어놔서
// 해당 클래스 상속받는 것만 넣어둔다면?

class GameObject
{
protected:
	string mName;
	float mX;
	float mY;
	float mSizeX;
	float mSizeY;
	RECT mRect;

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

