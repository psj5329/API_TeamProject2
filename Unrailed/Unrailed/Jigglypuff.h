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

	

public:
	void Init() override;
	void Init(int x, int y, bool isUpgraded);
	void Release()override;
	void Update() override;
	void Render(HDC hdc) override;

	bool TakeMike();
	bool IsInSongRange(RECT rect);

	void StartSearchingMike();

private:
	void AnimationInit();
	void SetAnimation();
	void SearchMike();
	
	//노래동그라미
	void InitSongRect();
	void UpdateSongRect();
	void RenderSongRect(HDC hdc);

	void UpdateNotes(Note* note);
};