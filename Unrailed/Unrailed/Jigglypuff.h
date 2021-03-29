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

	//��ã��
	vector<vector<Tile*>> mTileList;
	vector<vector<MapObject*>> mMapObjectList;
	vector<Tile*> mPathFinderList;


	//����ũ
	class Mike* mMike;

public:
	void Init() override;
	void Init(int x, int y, bool isUpgraded);
	void Release()override;
	void Update() override;
	void Render(HDC hdc) override;

	void SetTileListPtr(vector<vector<Tile*>> tileList) { mTileList = tileList; }
	void SetMapObjectListPtr(vector<vector<MapObject*>> mapObjectList) { mMapObjectList = mapObjectList; }

	//������ ��
	bool IsInSongRange(RECT rect);

	//�÷��̾�θ�
	void StartSearchingMike(int MikeX, int MikeY);
	bool TakeMike();
	void PlaceMike(int indexX, int indexY);

private:
	void AnimationInit();
	void SetAnimation();
	void MoveToMike();
	
	//�뷡���׶��
	void InitSongRect();
	void UpdateSongRect();
	void RenderSongRect(HDC hdc);

	void UpdateNotes(Note* note);
};