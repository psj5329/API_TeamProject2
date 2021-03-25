#include "pch.h"
#include "Gizmo.h"
#include "Camera.h"

Singleton_NULL(Gizmo)

Gizmo::Gizmo()
{
	mNullBrush = (HBRUSH)GetStockObject(NULL_BRUSH);


	HPEN greenPen = CreatePen(PS_SOLID, 1, RGB(0, 128, 0));
	HPEN bluePen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	HPEN redPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	HPEN blackPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	HPEN grayPen = CreatePen(PS_SOLID, 1, RGB(128, 128, 128));

	HPEN limePen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	HPEN darkGreenPen = CreatePen(PS_SOLID, 1, RGB(0, 100, 0));
	HPEN navyPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 128));
	HPEN maroonPen = CreatePen(PS_SOLID, 1, RGB(128, 0, 0));
	HPEN darkRedPen = CreatePen(PS_SOLID, 1, RGB(139, 0, 0));
	HPEN crimsonPen = CreatePen(PS_SOLID, 1, RGB(220, 20, 60));
	HPEN brownPen = CreatePen(PS_SOLID, 1, RGB(165, 42, 42));
	HPEN pinkPen = CreatePen(PS_SOLID, 1, RGB(255, 192, 203));
	HPEN deepPinkPen = CreatePen(PS_SOLID, 1, RGB(255, 20, 147));

	HPEN yellowPen = CreatePen(PS_SOLID, 3, RGB(255, 255, 0)); // 플레이어 사용 중
	HPEN olivePen = CreatePen(PS_SOLID, 1, RGB(128, 128, 0));
	HPEN orangePen = CreatePen(PS_SOLID, 1, RGB(255, 165, 0));
	HPEN darkOrangePen = CreatePen(PS_SOLID, 1, RGB(255, 140, 0));
	HPEN orangeRedPen = CreatePen(PS_SOLID, 1, RGB(255, 69, 0));
	HPEN cyanPen = CreatePen(PS_SOLID, 3, RGB(0, 255, 255)); // 플레이어 사용 중
	HPEN deepSkyBluePen = CreatePen(PS_SOLID, 1, RGB(0, 191, 255));
	HPEN skyBluePen = CreatePen(PS_SOLID, 1, RGB(135, 206, 235));
	HPEN darkCyanPen = CreatePen(PS_SOLID, 1, RGB(0, 139, 139));
	HPEN purplePen = CreatePen(PS_SOLID, 3, RGB(128, 0, 128)); // 플레이어 사용 중
	HPEN violetPen = CreatePen(PS_SOLID, 3, RGB(238, 130, 238)); // 플레이어 사용 중

	mPenList.insert(make_pair(Color::Green, greenPen));
	mPenList.insert(make_pair(Color::Blue, bluePen));
	mPenList.insert(make_pair(Color::Red, redPen));
	mPenList.insert(make_pair(Color::Black, blackPen));
	mPenList.insert(make_pair(Color::Gray, grayPen));
	mPenList.insert(make_pair(Color::Lime, limePen));
	mPenList.insert(make_pair(Color::DarkGreen, darkGreenPen));
	mPenList.insert(make_pair(Color::Navy, navyPen));
	mPenList.insert(make_pair(Color::Maroon, maroonPen));
	mPenList.insert(make_pair(Color::DarkRed, darkRedPen));
	mPenList.insert(make_pair(Color::Crimson, crimsonPen));
	mPenList.insert(make_pair(Color::Brown, brownPen));
	mPenList.insert(make_pair(Color::Pink, pinkPen));
	mPenList.insert(make_pair(Color::DeepPink, deepPinkPen));
	mPenList.insert(make_pair(Color::Yellow, yellowPen));
	mPenList.insert(make_pair(Color::Olive, olivePen));
	mPenList.insert(make_pair(Color::Orange, orangePen));
	mPenList.insert(make_pair(Color::DarkOrange, darkOrangePen));
	mPenList.insert(make_pair(Color::OrangeRed, orangeRedPen));
	mPenList.insert(make_pair(Color::Cyan, cyanPen));
	mPenList.insert(make_pair(Color::DeepSkyBlue, deepSkyBluePen));
	mPenList.insert(make_pair(Color::SkyBlue, skyBluePen));
	mPenList.insert(make_pair(Color::DarkCyan, darkCyanPen));
	mPenList.insert(make_pair(Color::Purple, purplePen));
	mPenList.insert(make_pair(Color::Violet, violetPen));
}

Gizmo::~Gizmo()
{
	PenIter iter = mPenList.begin(); 
	for (; iter != mPenList.end(); ++iter)
	{
		DeleteObject(iter->second);
	}
	DeleteObject(mNullBrush);
}

void Gizmo::DrawRect(HDC hdc, RECT rc, Color color)
{
	HPEN pen = mPenList[color];

	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc,mNullBrush);
	HPEN oldPen = (HPEN)SelectObject(hdc, pen);
	RenderRect(hdc, rc);
	SelectObject(hdc, oldPen);
	SelectObject(hdc, oldBrush);
}

void Gizmo::DrawEllipse(HDC hdc, float x, float y, float radius, Color color)
{
	HPEN pen = mPenList[color];

	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, mNullBrush);
	HPEN oldPen = (HPEN)SelectObject(hdc, pen);
	RenderEllipse(hdc, x,y,radius);
	SelectObject(hdc, oldPen);
	SelectObject(hdc, oldBrush);
}

void Gizmo::DrawRectInCamera(HDC hdc, RECT rc, Color color)
{
	HPEN pen = mPenList[color];

	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, mNullBrush);
	HPEN oldPen = (HPEN)SelectObject(hdc, pen);
	CAMERAMANAGER->GetMainCamera()->RenderRectCam(hdc, rc);
	//RenderRect(hdc, rc);
	SelectObject(hdc, oldPen);
	SelectObject(hdc, oldBrush);
}
