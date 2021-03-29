#pragma once
#include "GameObject.h"

enum class JigglyState :int
{
	Sing,
	Search,
	Idle,
	Move
};

struct Note
{
	Image* image;
	float positionX;
	float positionY;
	float sizeX;
	float sizeY;
	float speed;
	int frameX;
	int frameY;
	float angle;
	RECT rect;
};

class Animation;
class Image;
class Jigglypuff : public GameObject
{
protected:

	JigglyState mState;
	bool mHoldMike;
	bool mIsUpgraded;

	int mIndexX;
	int mIndexY;

	vector <Note*> mNoteList;
	RECT mSongRect;
	float mSongRadius;
	Image* mImage;
	Image* mSongImage;
	Direction mDirection;

	Animation* mIdle;
	Animation* mSing;
	Animation* mDownMove;
	Animation* mUpMove;
	Animation* mLeftMove;
	Animation* mRightMove;
	Animation* mCurrentAnimation;

	float mSpeed;

	//길찾기
	vector<vector<Tile*>> mTileList;
	vector<vector<MapObject*>> mMapObjectList;
	vector<Tile*> mPathFinderList;


	//마이크
	class Mike* mMike;

public:
	void Init() override;
	void Init(int x, int y, bool isUpgraded);
	void Release()override;
	void Update() override;
	void Render(HDC hdc) override;

	void SetTileListPtr(vector<vector<Tile*>> tileList) { mTileList = tileList; }
	void SetMapObjectListPtr(vector<vector<MapObject*>> mapObjectList) { mMapObjectList = mapObjectList; }

	//기차가 쓸
	bool IsInSongRange(RECT rect);

	//플레이어가부를
	void StartSearchingMike(int MikeX, int MikeY);
	bool TakeMike();
	void PlaceMike(int indexX, int indexY);

private:
	void AnimationInit();
	void SetAnimation();
	void MoveToMike();
	
	//노래동그라미
	void InitSongRect();
	void UpdateSongRect();
	void RenderSongRect(HDC hdc);

	void UpdateNotes(Note* note);
};