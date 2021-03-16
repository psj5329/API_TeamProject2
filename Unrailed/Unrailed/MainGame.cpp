#include "pch.h"
#include "MainGame.h"

#include "Image.h"
#include "Camera.h"

#include "CameraManager.h"
#include "ObjectManager.h"

#include "Scene1.h"
#include "Scene2.h"
#include "Scene3.h"
#include "Scene4.h"
#include "MapToolScene.h"

void MainGame::Init()
{
	mBackBuffer = new Image();
	mBackBuffer->CreateEmpty(WINSIZEX, WINSIZEY);

	// 나중에 정리하기
	SceneManager::GetInstance()->AddScene(L"Scene1", new Scene1);
	SceneManager::GetInstance()->AddScene(L"Scene2", new Scene2);
	SceneManager::GetInstance()->AddScene(L"Scene3", new Scene3);
	SceneManager::GetInstance()->AddScene(L"Scene4", new Scene4);
	SceneManager::GetInstance()->AddScene(L"MapToolScene", new MapToolScene);
	IMAGEMANAGER->LoadFromFile(L"Save", Resources(L"/Map/Tool/save"), 60, 16, 1, 1, true);
	IMAGEMANAGER->LoadFromFile(L"Load", Resources(L"/Map/Tool/load"), 60, 16, 1, 1, true);
	IMAGEMANAGER->LoadFromFile(L"Undo", Resources(L"/Map/Tool/undo"), 60, 16, 1, 1, true);
	IMAGEMANAGER->LoadFromFile(L"TinyWoods", Resources(L"/Map/TinyWoods"), 288, 144, 12, 6, true);
	IMAGEMANAGER->LoadFromFile(L"MagmaCavern", Resources(L"/Map/MagmaCavern"), 288, 144, 12, 6, true);
	IMAGEMANAGER->LoadFromFile(L"MtFarAway", Resources(L"/Map/MtFarAway"), 288, 144, 12, 6, true);
	IMAGEMANAGER->LoadFromFile(L"GreenMineral", Resources(L"/Map/Green"), 144, 72, 6, 3, true);
	IMAGEMANAGER->LoadFromFile(L"BlueMineral", Resources(L"/Map/Blue"), 144, 72, 6, 3, true);
	IMAGEMANAGER->LoadFromFile(L"RedMineral", Resources(L"/Map/Red"), 144, 72, 6, 3, true);
	IMAGEMANAGER->LoadFromFile(L"RightArrow", Resources(L"/Map/Tool/RightArrow"), 16, 14, 1, 1, true);
	IMAGEMANAGER->LoadFromFile(L"XTile", Resources(L"/Map/Tool/XTile"), 24, 24, 1, 1, true);

	// {{ 로딩씬 만들기 전까지 임시로 여기에 시작
	// 맵 관련 // 남훈
	IMAGEMANAGER->LoadFromFile(L"Save", Resources(L"/Map/Tool/save"), 60, 16, 1, 1, true);
	IMAGEMANAGER->LoadFromFile(L"Load", Resources(L"/Map/Tool/load"), 60, 16, 1, 1, true);
	IMAGEMANAGER->LoadFromFile(L"Undo", Resources(L"/Map/Tool/undo"), 60, 16, 1, 1, true);
	IMAGEMANAGER->LoadFromFile(L"TinyWoods", Resources(L"/Map/TinyWoods"), 288, 144, 12, 6, true);
	IMAGEMANAGER->LoadFromFile(L"GreenMineral", Resources(L"/Map/Green"), 144, 72, 6, 3, true);
	IMAGEMANAGER->LoadFromFile(L"BlueMineral", Resources(L"/Map/Blue"), 144, 72, 6, 3, true);
	IMAGEMANAGER->LoadFromFile(L"RedMineral", Resources(L"/Map/Red"), 144, 72, 6, 3, true);
	IMAGEMANAGER->LoadFromFile(L"RightArrow", Resources(L"/Map/Tool/RightArrow"), 16, 14, 1, 1, true);

	// 캐릭터 관련 // 유찬
	IMAGEMANAGER->LoadFromFile(L"charmander", Resources(L"charmander"), 192, 1968, 4, 41, true);
	IMAGEMANAGER->LoadFromFile(L"chikorita", Resources(L"chikorita"), 192, 1968, 4, 41, true);
	IMAGEMANAGER->LoadFromFile(L"ditto", Resources(L"ditto"), 144, 1200, 3, 25, true);
	IMAGEMANAGER->LoadFromFile(L"jigglypuff", Resources(L"jigglypuff"), 192, 816, 4, 17, true);
	IMAGEMANAGER->LoadFromFile(L"lapras", Resources(L"lapras"), 192, 624, 4, 13, true);
	IMAGEMANAGER->LoadFromFile(L"totodile", Resources(L"totodile"), 240, 1968, 5, 41, true);
	// 로딩씬 만들기 전까지 임시로 여기에 끝 }}

	Camera* mainCamera = new Camera;
	mainCamera->Init();
	mainCamera->SetX(WINSIZEX / 2);
	mainCamera->SetY(WINSIZEY / 2);
	
	CAMERAMANAGER->SetMainCamera(mainCamera);

	//SceneManager::GetInstance()->LoadScene(L"Scene1");
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
		SCENEMANAGER->LoadScene(L"MapToolScene");
	}
	
	SceneManager::GetInstance()->Update();
}

void MainGame::Render(HDC hdc)
{
	//백버퍼의 HDC 가져온다
	HDC backDC = mBackBuffer->GetHDC();
	//HDC 영역을 특정 색으로 밀어버리는 녀석
	PatBlt(backDC, 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
	// ==================================================
	{
		SceneManager::GetInstance()->Render(backDC);
		RenderTime(backDC);
	}
	//====================================================
	//후면버퍼 내용을 윈도우 창에 고속 복사
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

	TextOut(hdc, 10, 10, strWorldTime.c_str(), strWorldTime.length());
	TextOut(hdc, 10, 25, strDeltaTime.c_str(), strDeltaTime.length());
	TextOut(hdc, 10, 40, strFPS.c_str(), strFPS.length());


	/*wstring strScene = L"여긴 메인, 1~4 눌러서 씬 넘어가자";
	TextOut(hdc, WINSIZEX / 2 - 50, WINSIZEY / 2, strScene.c_str(), strScene.length());*/
}

