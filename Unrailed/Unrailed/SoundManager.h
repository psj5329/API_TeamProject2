#pragma once

#define MaxChannelBuffer 50	//�ִ� ä�� ��

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
	vector<ChannelInfo> mActiveChannels;		//���� Ȱ��ȭ �Ǿ� �ִ� ä��
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

public:		// �޴��� �Ͻ����� ����� �ɼǿ��� ���� ���� �� �ʿ��� ��
	float GetVolume(const wstring& keyName);
	float GetPosition(const wstring& keyName);			// ���� ��ġ
	float GetWholePosition(const wstring& keyName);		// ��ü ����
	wstring GetNowPlaying() { return mActiveChannels.begin()->SoundName; }

	void SetVolume(const wstring& keyName, float volume);
	void SetPosition(const wstring& keyName, float time);
};

#define SOUNDMANAGER SoundManager::GetInstance()