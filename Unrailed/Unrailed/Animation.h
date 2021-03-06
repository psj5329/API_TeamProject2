#pragma once

class Animation
{
	bool mIsPlay;	//플레이중이냐
	bool mIsLoop;	//루프냐

	int mCurrentFrameIndex;	//현재 프레임 인덱스 

	float mCurrentFrameTime;	//현재 프레임 감는 시간
	float mFrameUpdateTime;		//프레임 갱신(증가) 해야하는 시간
	
	vector<pair<int, int>> mFrameList;		//프레임 정보
	function <void(void)> mCallbackFunc;		//애니메이션이 전부 플레이되고 끝날 때 실행할 함수
public:
	Animation();

	void Update();
	void UpdateMapCover();

	void Play();
	void Stop();
	void Pause();

	void InitFrameByVector(const vector<pair<int, int>>& frameList);
	void InitFrameByStartEnd(int startX, int startY, int endX, int endY, bool isReverse);

	void SetCallbackFunc(const function<void(void)>& func);
	inline void SetIsLoop(bool isLoop) { mIsLoop = isLoop; }
	inline void SetFrameUpdateTime(float updateTime) { mFrameUpdateTime = updateTime; }
	inline void SetCurrentFrameIndex(int index) { mCurrentFrameIndex = index; }

	inline float GetFrameUpdateTime()const { return mFrameUpdateTime; }
	inline int GetCurrentFrameIndex()const { return mCurrentFrameIndex; }
	inline float GetCurrentFrameTIme()const { return mCurrentFrameTime; }
	inline bool GetIsLoop()const { return mIsLoop; }
	inline bool GetIsPlay()const { return mIsPlay; }
	inline pair<int, int> GetNowFrame()const { return mFrameList[mCurrentFrameIndex]; }
	inline int GetNowFrameX()const { return mFrameList[mCurrentFrameIndex].first; }
	inline int GetNowFrameY()const { return mFrameList[mCurrentFrameIndex].second; }

};

