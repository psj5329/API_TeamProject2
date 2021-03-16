#pragma once
#include <thread>

class Scene;
class SceneManager
{
	Singleton(SceneManager)

private:
	typedef map<wstring, Scene*>::iterator SceneIter;
	map<wstring, Scene*> mSceneList;
	Scene* mCurrentScene;

	Scene* mTargetScene;
	Scene* mLoadingScene;
	thread* mLoadingThread;		// 이 세개가 로딩때 쓴다 고쳐보자
	bool mIsLoadingEnd;

private:
	void LoadingThread();

public:
	SceneManager();
	~SceneManager();

	void Update();
	void Render(HDC hdc);

	void AddScene(const wstring& sceneName, Scene* scene);
	void LoadScene(const wstring& sceneName);
	void LoadScene(const wstring& targetSceneName, const wstring& loadingSceneName);

	Scene* GetCurrentScene();
};

#define SCENEMANAGER SceneManager::GetInstance()