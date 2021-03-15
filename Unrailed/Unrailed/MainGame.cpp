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

