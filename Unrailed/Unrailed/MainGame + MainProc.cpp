#include "pch.h"
#include "MainGame.h"
#include "SceneManager.h"
#include "Scene.h"

LRESULT MainGame::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_MOUSEMOVE:
		_mousePosition.x = LOWORD(lParam);
		_mousePosition.y = HIWORD(lParam);
		break;
	case WM_KEYDOWN:
		//cout << wParam << endl;
		if(SCENEMANAGER->GetCurrentScene())
			SCENEMANAGER->GetCurrentScene()->MainProc(hWnd, iMessage, wParam, lParam);
		break;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));

}
