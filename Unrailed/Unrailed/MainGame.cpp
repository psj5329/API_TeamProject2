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

	Input::ReleaseInstance();
	Random::ReleaseInstance();
	Time::ReleaseInstance();
}

void MainGame::Update()
{
	if (Input::GetInstance()->GetKeyDown('1'))
	{
		SCENEMANAGER->LoadScene(L"Scene1");
	}
	else if (Input::GetInstance()->GetKeyDown('2'))
	{
		SCENEMANAGER->LoadScene(L"Scene2");
	}
	else if (Input::GetInstance()->GetKeyDown('3'))
	{
		//SCENEMANAGER->LoadScene(L"MapToolScene");
		SCENEMANAGER->LoadScene(L"Scene3");
	}
	else if (Input::GetInstance()->GetKeyDown('4'))
	{
		SCENEMANAGER->LoadScene(L"Scene4");
	}
	else if (Input::GetInstance()->GetKeyDown('5'))
	{
		SCENEMANAGER->LoadScene(L"MapToolReady");
	}
	else if (Input::GetInstance()->GetKeyDown('6'))
	{
		SCENEMANAGER->LoadScene(L"Scene6");
	}
	else if (Input::GetInstance()->GetKeyDown('7'))
	{
		SCENEMANAGER->LoadScene(L"MapToolReady");
	}
	SceneManager::GetInstance()->Update();
}

void MainGame::Render(HDC hdc)
{
	HDC backDC = mBackBuffer->GetHDC();

	if(SCENEMANAGER->GetCurrentSceneName() == L"LoadingScene")
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
	float worldTime = Time::GetInstance()->GetWorldTime();
	float deltaTime = Time::GetInstance()->DeltaTime();
	ULONG fps = Time::GetInstance()->GetmFrameRate();
	wstring strWorldTime = L"WorldTime : " + to_wstring(worldTime);
	wstring strDeltaTime = L"DeltaTime : " + to_wstring(deltaTime);
	wstring strFPS = L"FPS : " + to_wstring(fps);

	TextOut(hdc, 10, 10, strWorldTime.c_str(), (int)strWorldTime.length());
	TextOut(hdc, 10, 25, strDeltaTime.c_str(), (int)strDeltaTime.length());
	TextOut(hdc, 10, 40, strFPS.c_str(), (int)strFPS.length());

	/*wstring strScene = L"여긴 메인, 1~4 눌러서 씬 넘어가자";
	TextOut(hdc, WINSIZEX / 2 - 50, WINSIZEY / 2, strScene.c_str(), strScene.length());*/
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
		size_t strSize = strPath.find(L"2\\Unrailed\\Unrailed");
		strPath.erase(strSize);
		strPath.append(L"2\\Unrailed\\Resources\\*.*");		// ㅏ 진짜 넘 맘에 안든다
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
			strKey = wcstok_s(&strKey[0], L".", &ptr);	// 우선 .bmp 떼고

			if (wcsstr(strKey.c_str(), L"-") != NULL)
			{
				wstring strFrameX = wcstok_s(&strKey[0], L"-", &ptr);
				strFrameX = wcstok_s(nullptr, L"-", &ptr);
				frameX = _wtoi(strFrameX.c_str());
				wstring strFrameY = wcstok_s(nullptr, L"-", &ptr);			// x프레임 떼고
				frameY = _wtoi(strFrameY.c_str());
			}

			wstring strFull = FileSystemHelper::mVecFileInfo[i].FilePath + FileSystemHelper::mVecFileInfo[i].FileName;

			// 디코더 생성
			IWICBitmapDecoder* ipDecoder = nullptr;
			wicFactory->CreateDecoderFromFilename(strFull.c_str(), NULL, GENERIC_READ,
				WICDecodeMetadataCacheOnDemand, &ipDecoder);
			// 디코더에서 프레임얻음
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
		size_t strSize = strPath.find(L"2\\Unrailed\\Unrailed");
		strPath.erase(strSize);
		strPath.append(L"2\\Unrailed\\Sound\\*.*");
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