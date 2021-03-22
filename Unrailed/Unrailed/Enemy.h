#pragma once
#include "Unit.h"

enum EnemyState { IDLE, WALK, ATTACK, DEAD, ENEMYSTATEEND };

class Tile;
class Animation;
class Player;
class PathFinder;
class Enemy : public Unit
{
protected:
	vector<vector<Tile*>> mTileList;
	//vector<PathFinder::> mPath;

	RECT mHitBox;
	int mHp;		// 어차피 타격수에 따라 죽는다
	float mSpeed;
	EnemyState mState;
	Player* mPlayer;

	Animation* mIdleAnimation;
	Animation* mWalkAnimation;
	Animation* mAttackAnimation;		// 템 훔칠 때
	Animation* mDeadAnimation;
	Animation* mCurrentAnimation;

public:
	virtual void Init() override;
	virtual void Release()override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

	virtual void ReadyAnimation() = 0;
	virtual void SetAnimation() = 0;

public:
	// Get
	int GetHp() { return mHp; }
	float GetSpeed() { return mSpeed; }
	vector<vector<Tile*>> GetTileList() { return mTileList; }

	// Set
	void SetHp(int hp) { mHp = hp; }
	void SetSpeed(float s) { mSpeed = s; }
	void SetTileList(vector<vector<Tile*>> tile) { mTileList = tile; }

	void DamagedHp() { mHp -= 1; }
};

