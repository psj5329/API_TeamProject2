#include "pch.h"
#include "SceneManager.h"

#include "Scene.h"
#include "LoadingScene.h"

Singleton_NULL(SceneManager)

SceneManager::SceneManager()
	: mCurrentScene(nullptr)//, mLoadingThread(nullptr), mTargetScene(nullptr), mLoadingScene(nullptr), mIsLoadingEnd(false)
{
}

SceneManager::~SceneManager()
{
	SceneIter iter = mSceneList.begin();
	for (; iter != mSceneList.end(); ++iter)
	{
		//iter->second->Release();
		SafeDelete(iter->second);
	}
}

void SceneManager::Update()
{
	if (mCurrentScene != nullptr)
		mCurrentScene->Update();
}

void SceneManager::Render(HDC hdc)
{
	if (mCurrentScene != nullptr)
		mCurrentScene->Render(hdc);
}

void SceneManager::AddScene(const wstring& sceneName,Scene * scene)
{
	SceneIter iter = mSceneList.find(sceneName);

	if (iter != mSceneList.end())
		return;

	mSceneList.insert(make_pair(sceneName, scene));
}

void SceneManager::LoadScene(const wstring & sceneName)
{
	SceneIter iter = mSceneList.find(sceneName);

	if (iter == mSceneList.end())
		return;

	if (mCurrentScene == iter->second)
		return;
	
	Scene* targetScene = iter->second;

	if (mCurrentScene)
		mCurrentScene->Release();

	targetScene->Init();

	mCurrentScene = targetScene;
}

//void SceneManager::LoadScene(const wstring & targetSceneName, const wstring & loadingSceneName)
//{
//	SceneIter iter = mSceneList.find(targetSceneName);
//	if (iter != mSceneList.end())
//		mTargetScene = iter->second;
//
//	iter = mSceneList.find(loadingSceneName);
//	if (iter != mSceneList.end())
//		mLoadingScene = iter->second;
//
//	if (mTargetScene == nullptr || mLoadingScene == nullptr ||
//		mTargetScene == mCurrentScene || mLoadingScene == mCurrentScene)
//	{
//		mTargetScene = nullptr;
//		mLoadingScene = nullptr;
//		return;
//	}
//
//	function<void(void)> threadFunc = bind(&SceneManager::LoadingThread, this);
//	//스레드는 메모리 할당해주는 순간부터 바로 돌기 시작한다.
//	mLoadingThread = new thread(threadFunc);
//}
//
//void SceneManager::LoadingThread()
//{
//	Scene* prevScene = mCurrentScene;
//	
//	mCurrentScene = mLoadingScene;
//	mCurrentScene->Init();
//
//	prevScene->Release();
//
//	mTargetScene->Init();
//	
//	mCurrentScene = mTargetScene;
//
//
//	mLoadingScene->Release();
//
//	mTargetScene = nullptr;
//	mLoadingScene = nullptr;
//	mIsLoadingEnd = true;
//}

Scene* SceneManager::GetCurrentScene()
{
	return mCurrentScene;
}

wstring SceneManager::GetCurrentSceneName()
{
	SceneIter iter = mSceneList.begin();
	for (; iter != mSceneList.end(); ++iter)
	{
		if (mCurrentScene == iter->second)
			return iter->first;
	}
	
	return NULL;
}
