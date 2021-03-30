#pragma once

#define MaxChannelBuffer 50	//최대 채널 수

class SoundManager final
{
	Singleton(SoundManager)
private:
	struct ChannelInfo
	{
		wstring SoundName;
		FMOD::Channel* Channel;
		bool Loop;
	};
	typedef map<wstring, FMOD::Sound*>::iterator SoundIter;

private:
	map<wstring, FMOD::Sound*> mSoundList;
	vector<ChannelInfo> mActiveChannels;		//현재 활성화 되어 있는 채널
	FMOD::System* mSystem;

private:
	SoundManager();
	~SoundManager();

public:
	void Update();
	void LoadFromFile(const wstring& keyName, const wstring& fileName, bool isLoop = false);
	void Play(const wstring& keyName, float volume);
	void Pause(const wstring& keyName);
	void Stop(const wstring& keyName);

public:		// 메뉴랑 일시정지 만들면 옵션에서 사운드 조절 시 필요할 것
	float GetVolume(const wstring& keyName);
	float GetPosition(const wstring& keyName);			// 현재 위치
	float GetWholePosition(const wstring& keyName);		// 전체 길이
	wstring GetNowPlaying() { return mActiveChannels.begin()->SoundName; }

	void SetVolume(const wstring& keyName, float volume);
	void SetPosition(const wstring& keyName, float time);
};

#define SOUNDMANAGER SoundManager::GetInstance()