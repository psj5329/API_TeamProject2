#include "pch.h"
#include "MainGame.h"

#include "Image.h"
#include "Camera.h"

#include "CameraManager.h"
#include "ObjectManager.h"

#include "LoadingScene.h"
#include "Scene1.h"
#include "Scene2.h"
#include "Scene3.h"
#include "Scene4.h"
#include "MapToolScene.h"
#include "Scene6.h"
#include "MapToolReady.h"
#include "MainScene.h"
#include "SceneSelect.h"
#include "Stage1.h"
#include "Stage2.h"
#include "Stage3.h"
#include "Stage4.h"
#include "Stage5.h"

#include "FileSystemHelper.h"

void MainGame::Init()
{
	mBackBuffer = new Image();
	mBackBuffer->CreateEmpty(WINSIZEX, WINSIZEY);

	AddScene();

	Camera* mainCamera = new Camera;
	mainCamera->SetX(WINSIZEX / 2);
	mainCamera->SetY(WINSIZEY / 2);
	mainCamera->Init();
	//mainCamera->SetX(0);
	//mainCamera->SetY(0);

	CAMERAMANAGER->SetMainCamera(mainCamera);

	_BackgroundSound = 1.f;
	_EffectSound = 1.f;
}

void MainGame::Release()
{
	SafeDelete(mBackBuffer);

	Camera* camera = CAMERAMANAGER->GetMainCamera();
	SafeDelete(camera);
	CameraManager::ReleaseInstance();

	ObjectManager::ReleaseInstance();
	SceneManager::ReleaseInstance();
	ImageManager::ReleaseInstance();
	CollisionManager::ReleaseInstance();
	GameEventManager::ReleaseInstance();
	Gizmo::ReleaseInstance();
	PathFinder::ReleaseInstance();
	SoundManager::ReleaseInstance();
	Input::ReleaseInstance();
	Random::ReleaseInstance();
	Time::ReleaseInstance();
}

void MainGame::Update()
{
	if (Input::GetInstance()->GetKeyDown('1'))
	{
		SCENEMANAGER->LoadScene(L"Stage1");
	}
	else if (Input::GetInstance()->GetKeyDown('2'))
	{
		SCENEMANAGER->LoadScene(L"Stage2");
	}
	else if (Input::GetInstance()->GetKeyDown('3'))
	{
		//SCENEMANAGER->LoadScene(L"MapToolScene");
		SCENEMANAGER->LoadScene(L"Stage3");
	}
	else if (Input::GetInstance()->GetKeyDown('4'))
	{
		SCENEMANAGER->LoadScene(L"Stage4");
	}
	else if (Input::GetInstance()->GetKeyDown('5'))
	{
		SCENEMANAGER->LoadScene(L"Stage5");
	}
	else if (Input::GetInstance()->GetKeyDown('6'))
	{
		SCENEMANAGER->LoadScene(L"MapToolReady");
	}
	else if (Input::GetInstance()->GetKeyDown('7'))
	{
		SCENEMANAGER->LoadScene(L"MapToolReady");
	}

	//INPUT->Update();
	SCENEMANAGER->Update();
}

void MainGame::Render(HDC hdc)
{
	HDC backDC = mBackBuffer->GetHDC();

	if (SCENEMANAGER->GetCurrentSceneName() == L"LoadingScene")
		PatBlt(backDC, 0, 0, WINSIZEX, WINSIZEY, BLACKNESS);
	else
		PatBlt(backDC, 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
	// ==================================================
	{
		SceneManager::GetInstance()->Render(backDC);
		RenderTime(backDC);
	}
	//====================================================

	mBackBuffer->Render(hdc, 0, 0);
}

void MainGame::RenderTime(HDC hdc)
{
}

void MainGame::AddScene()
{
	mLoadingScene = new LoadingScene;

	LoadResources(mLoadingScene);

	SCENEMANAGER->AddScene(L"LoadingScene", mLoadingScene);
	SceneManager::GetInstance()->AddScene(L"MainScene", new MainScene);
	SceneManager::GetInstance()->AddScene(L"SceneSelect", new SceneSelect);
	SceneManager::GetInstance()->AddScene(L"Scene1", new Scene1);
	SceneManager::GetInstance()->AddScene(L"Scene2", new Scene2);
	SceneManager::GetInstance()->AddScene(L"Scene3", new Scene3);
	SceneManager::GetInstance()->AddScene(L"Scene4", new Scene4);
	SceneManager::GetInstance()->AddScene(L"MapToolScene", new MapToolScene);
	SceneManager::GetInstance()->AddScene(L"Scene6", new Scene6);
	SceneManager::GetInstance()->AddScene(L"MapToolReady", new MapToolReady);

	SCENEMANAGER->AddScene(L"Stage1", new Stage1);
	SCENEMANAGER->AddScene(L"Stage2", new Stage2);
	SCENEMANAGER->AddScene(L"Stage3", new Stage3);
	SCENEMANAGER->AddScene(L"Stage4", new Stage4);
	SCENEMANAGER->AddScene(L"Stage5", new Stage5);

	SCENEMANAGER->LoadScene(L"LoadingScene");
}

void MainGame::LoadResources(LoadingScene * scene)
{
	LoadImageResources(scene);
	LoadSoundResources(scene);
}

void MainGame::LoadImageResources(LoadingScene* scene)
{
	wchar_t path[100];
	if (GetCurrentDirectory(100, path) > 0)
	{
		wchar_t* ptr = nullptr;
		wstring strPath = path;
		//wstring str = wcstok_s(&strPath[0], L"2", &ptr);
		size_t strSize = strPath.find(L"\\Unrailed\\Unrailed");
		strPath.erase(strSize);
		strPath.append(L"\\Unrailed\\Resources\\*.*");
		FileSystemHelper::GetAllFile(strPath, FileType::IMAGE);
	}

	UINT width = 0, height = 0;
	int frameX = 0, frameY = 0;
	wchar_t* ptr = nullptr;
	CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&wicFactory));

	for (int i = 0; i < FileSystemHelper::mVecFileInfo.size(); ++i)
	{
		if (FileSystemHelper::mVecFileInfo[i].FileType == FileType::IMAGE)
		{
			width = 0;
			height = 0;
			frameX = 0;
			frameY = 0;

			wstring strKey = FileSystemHelper::mVecFileInfo[i].FileName.c_str();
			strKey = wcstok_s(&strKey[0], L".", &ptr);	// ???? .bmp ????

			if (wcsstr(strKey.c_str(), L"-") != NULL)
			{
				wstring strFrameX = wcstok_s(&strKey[0], L"-", &ptr);
				strFrameX = wcstok_s(nullptr, L"-", &ptr);
				frameX = _wtoi(strFrameX.c_str());
				wstring strFrameY = wcstok_s(nullptr, L"-", &ptr);			// x?????? ????
				frameY = _wtoi(strFrameY.c_str());
			}

			wstring strFull = FileSystemHelper::mVecFileInfo[i].FilePath + FileSystemHelper::mVecFileInfo[i].FileName;

			// ?????? ????
			IWICBitmapDecoder* ipDecoder = nullptr;
			wicFactory->CreateDecoderFromFilename(strFull.c_str(), NULL, GENERIC_READ,
				WICDecodeMetadataCacheOnDemand, &ipDecoder);

			// ?????????? ??????????
			IWICBitmapFrameDecode* ipFrame = nullptr;
			ipDecoder->GetFrame(0, &ipFrame);
			ipFrame->GetSize(&width, &height);

			if (frameX == 0 && frameY == 0)
				scene->AddLoadFunc([strKey, strFull, width, height, frameX, frameY]() {
				IMAGEMANAGER->LoadFromFile(strKey, strFull, width, height, true); });
			else
				scene->AddLoadFunc([strKey, strFull, width, height, frameX, frameY]() {
				IMAGEMANAGER->LoadFromFile(strKey, strFull, width, height, frameX, frameY, true); });
		}
	}
}

void MainGame::LoadSoundResources(LoadingScene* scene)
{
	wchar_t path[100];
	if (GetCurrentDirectory(100, path) > 0)
	{
		wchar_t* ptr = nullptr;
		wstring strPath = path;
		//wstring str = wcstok_s(&strPath[0], L"2", &ptr);
		size_t strSize = strPath.find(L"\\Unrailed\\Unrailed");
		strPath.erase(strSize);
		strPath.append(L"\\Unrailed\\Sound\\*.*");
		FileSystemHelper::GetAllFile(strPath, FileType::SOUND);
	}

	wchar_t* ptr = nullptr;

	for (int i = 0; i < FileSystemHelper::mVecFileInfo.size(); ++i)
	{
		if (FileSystemHelper::mVecFileInfo[i].FileType == FileType::SOUND)
		{
			wstring strFull = FileSystemHelper::mVecFileInfo[i].FilePath + FileSystemHelper::mVecFileInfo[i].FileName;
			wstring strKey = FileSystemHelper::mVecFileInfo[i].FileName.c_str();
			strKey = wcstok_s(&strKey[0], L".", &ptr);

			scene->AddLoadFunc([strKey, strFull]() { SOUNDMANAGER->LoadFromFile(strKey, strFull, false); });
		}
	}
}