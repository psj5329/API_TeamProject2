#include "pch.h"
#include "Unrailed.h"
#include "MainGame.h"

HINSTANCE	_hInstance;		//APP 고유번호
HWND		_hWnd;			//나중에 (그럴일은 없지만) WinAPI창을 여러개 생성했을 때 각 창을 컨트롤할 수 있는 Hanlder가 필요하다. 그게 바료 요 녀석

wstring _className = L"Unrailed";	//유니코드는 아스키코드랑 다르게 2byte씩 처리한다(한글등등 그런 언어들을 처리하기 위해서) 

POINT _mousePosition;	//마우스 좌표

MainGame* _mainGame = NULL;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int cmdShow)
{
	_hInstance = hInstance;

	WNDCLASS wndClass;			//윈도우 구조체(윈도우 창의 정보를 여기에 초기화 한다)

	wndClass.cbClsExtra = 0;										//클래스 여분 메모리 설정
	wndClass.cbWndExtra = 0;										//윈도우 여분 메모리 설정
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	//윈도우 백그라운드
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);					//윈도우 커서 모양
	wndClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));	//윈도우 아이콘 모양
	wndClass.hInstance = hInstance;									//윈도우 인스턴스 번호
	wndClass.lpfnWndProc = (WNDPROC)WndProc;						//윈도우 프로시져
	wndClass.lpszClassName = _className.c_str();					//윈도우 클래스 이름
	wndClass.lpszMenuName = NULL;									//윈도우 메뉴 이름
	wndClass.style = CS_HREDRAW | CS_VREDRAW;						//윈도우 창 스타일

	RegisterClass(&wndClass); //윈도우 정보를 등록해준다

	_hWnd = CreateWindow(
		_className.c_str(),
		_className.c_str(),
		WS_OVERLAPPEDWINDOW,		//윈도우 창 스타일
		WINSTARTX,							//윈도우 시작 X좌표
		WINSTARTY,							//윈도우 시작 Y좌표
		WINSIZEX,							//윈도우 창 가로크기
		WINSIZEY,							//윈도우 창 세로크기
		NULL,
		(HMENU)NULL,
		hInstance,
		NULL);

	RECT windowRect;
	windowRect.left = 0;
	windowRect.top = 0;
	windowRect.right = WINSIZEX;
	windowRect.bottom = WINSIZEY;

	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, false);

	SetWindowPos(_hWnd, NULL, WINSTARTX, WINSTARTY,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top, SWP_NOZORDER | SWP_NOMOVE);
	// }}

	ShowWindow(_hWnd, cmdShow);

	MSG message;
	HDC hdc = GetDC(_hWnd);

	_mainGame = new MainGame;
	_mainGame->Init();
	TIME->Start();

	while (true)
	{
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			if (message.message == WM_QUIT)
			{
				break;
			}

			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else
		{
			TIME->Update();
			SOUNDMANAGER->Update();
			INPUT->Update();

			_mainGame->Update();
			_mainGame->Render(hdc);
		}
	}

	//여기까지 왔다면 프로그램이 끝났다는 얘기
	_mainGame->Release();
	delete _mainGame;
	_mainGame = NULL;

	return (int)message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	if (_mainGame != NULL)
	{
		return _mainGame->MainProc(hWnd, iMessage, wParam, lParam);
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}