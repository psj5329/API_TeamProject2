#include "pch.h"
#include "LoadingScene.h"

#include "Image.h"
#include "Animation.h"

void LoadingScene::AddLoadFunc(const function<void(void)>& func)
{
	mLoadList.push_back(func);
}

void LoadingScene::Init()
{
	IMAGEMANAGER->LoadFromFile(L"LoadingTrain", Resources(L"LoadingTrain"), 850, 70, 5, 1, true);
	IMAGEMANAGER->LoadFromFile(L"LoadingEndTrain", Resources(L"LoadingEndTrain"), 850, 70, 5, 1, true);
	mImage = IMAGEMANAGER->FindImage(L"LoadingTrain");

	IMAGEMANAGER->LoadFromFile(L"LoadingBar", Resources(L"LoadingBar"), 1280, 30, true);
	mLoadingBarImage = IMAGEMANAGER->FindImage(L"LoadingBar");

	mAnimation = new Animation;
	mAnimation->InitFrameByStartEnd(0, 0, 4, 0, false);
	mAnimation->SetIsLoop(true);
	mAnimation->SetFrameUpdateTime(0.2f);

	mLoadIndex = 0;
	mIsEndLoading = false;

	function<void(void)> threadFunc = bind(&LoadingScene::LoadingImageThread, this);
	mLoadingImageThread = new thread(threadFunc);

	mFontColor = 255;
	mIsColorChange = false;
}

void LoadingScene::Release()
{
}

void LoadingScene::Update()
{
	mAnimation->Update();

	if (!mIsColorChange)
		mFontColor -= 5;
	else
		mFontColor += 5;

	if (mIsEndLoading)
	{
		if (INPUT->GetKeyDown(VK_RETURN))
			SCENEMANAGER->LoadScene(L"Scene2");
	}

	if (mLoadIndex >= mLoadList.size())
	{
		mIsEndLoading = true;
		mImage = IMAGEMANAGER->FindImage(L"LoadingEndTrain");
		return;
	}

	function<void(void)> func = mLoadList[mLoadIndex];
	func();
	mLoadIndex++;

	mRatio = mLoadIndex / mLoadList.size();
}

void LoadingScene::Render(HDC hdc)
{
	mImage->ScaleFrameRender(hdc, WINSIZEX / 2 - mImage->GetWidth() / 10, WINSIZEY / 2, mAnimation->GetNowFrameX(), 0, mImage->GetWidth() / 5, mImage->GetHeight());
	mLoadingBarImage->ScaleRender(hdc, 0, WINSIZEY - mLoadingBarImage->GetHeight(), mLoadingBarImage->GetWidth() * mRatio, mLoadingBarImage->GetHeight());

	SetBkColor(hdc, RGB(0, 0, 0));
	SetTextColor(hdc, RGB(255, 255, 255));

	wstring strLoading = to_wstring(mLoadIndex);
	strLoading.append(L" / ").append(to_wstring(mLoadList.size()));
	
	TextOut(hdc, 0, WINSIZEY - 50, strLoading.c_str(), strLoading.length());
	
	if (mIsEndLoading)
	{
		HFONT font = CreateFont(20, 0, 0, 0, FW_NORMAL, false, false, false, DEFAULT_CHARSET, OUT_STROKE_PRECIS,
			CLIP_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH || FF_SWISS, TEXT("견고딕"));

		RECT fontArea = RectMakeCenter(WINSIZEX / 2, WINSIZEY / 2 + 200, 300, 50);

		HFONT oldFont = (HFONT)SelectObject(hdc, font);
		SetTextColor(hdc, RGB(mFontColor, mFontColor, mFontColor));

		wstring strScene = L"엔터 버튼을 눌러주세요!";
		DrawText(hdc, strScene.c_str(), strScene.length(), &fontArea, DT_CENTER | DT_TOP | DT_WORDBREAK | DT_EXTERNALLEADING);

		SelectObject(hdc, oldFont);
		DeleteObject(font);
	}

	SetBkColor(hdc, RGB(255, 255, 255));
	SetTextColor(hdc, RGB(0, 0, 0));
}

void LoadingScene::LoadingImageThread()
{
	mAnimation->Play();
}
