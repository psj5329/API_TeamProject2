#include "pch.h"
#include "MapToolScene.h"
#include "Tile.h"
#include "Image.h"
#include "Button.h"
#include "Path.h"
#include "Camera.h"

void MapToolScene::Init()
{
	ImageLoad();

	Image* tileImage2 = ImageManager::GetInstance()->FindImage(L"Tile");
	Image* tileImage = ImageManager::GetInstance()->FindImage(L"TinyWoods");
	Image* mineralImage = ImageManager::GetInstance()->FindImage(L"GreenMineral");
	Image* save = ImageManager::GetInstance()->FindImage(L"Save");
	Image* load = ImageManager::GetInstance()->FindImage(L"Load");
	Image* undo = ImageManager::GetInstance()->FindImage(L"Undo");
	Image* eraser = ImageManager::GetInstance()->FindImage(L"XTile");
	Image* rightArrow = ImageManager::GetInstance()->FindImage(L"RightArrow");
	Image* empty = nullptr;

	InitEmptyMap();

	//팔레트
	int palleteStartX = WINSIZEX / 2;
	int palleteStartY = 50;
	for (int y = 0; y < PalleteCountY; ++y)
	{
		mPallete.push_back(vector <TilePallete>());
		for (int x = 0; x < PalleteCountX; ++x)
		{
			TilePallete tempTilePallete;
			tempTilePallete.image = tileImage;
			tempTilePallete.positionX = palleteStartX + TileSize * x;
			tempTilePallete.positionY = palleteStartY + TileSize * y;
			tempTilePallete.width = TileSize;
			tempTilePallete.height = TileSize;
			tempTilePallete.rect = RectMake(tempTilePallete.positionX, tempTilePallete.positionY, tempTilePallete.width, tempTilePallete.height);
			tempTilePallete.frameX = x;
			tempTilePallete.frameY = y;
			mPallete[y].push_back(tempTilePallete);
		}
	}

	//속성팔레트 넣어주기
	//시작점
	palleteStartX = WINSIZEX / 2 + 350;
	palleteStartY = 400;
	for (int i = 0;i < TypePalleteCount;i++)
	{
		TypePallete tempTypePallete;
		tempTypePallete.positionX = palleteStartX + TileSize * i;
		tempTypePallete.positionY = palleteStartY;
		tempTypePallete.width = TileSize;
		tempTypePallete.height = TileSize;
		tempTypePallete.rect = RectMake(tempTypePallete.positionX, tempTypePallete.positionY, tempTypePallete.width, tempTypePallete.height);
		tempTypePallete.type = i;
		tempTypePallete.tiletype = (TileType)i;
		mTypePallete.push_back(tempTypePallete);
	}

	//맵옵젝팔레트
	palleteStartX = WINSIZEX / 2;
	palleteStartY = 400;
	for (int y = 0; y < ObjectPalletCountY; ++y)
	{
		mObjectPallete.push_back(vector <MapObjectPallete>());
		for (int x = 0; x < ObjectPalletCountX; ++x)
		{
			MapObjectPallete tempPallete;
			tempPallete.image = mineralImage;
			tempPallete.positionX = palleteStartX + TileSize * x;
			tempPallete.positionY = palleteStartY + TileSize * y;
			tempPallete.width = TileSize;
			tempPallete.height = TileSize;
			tempPallete.rect = RectMake(tempPallete.positionX, tempPallete.positionY, tempPallete.width, tempPallete.height);
			tempPallete.frameX = x;
			tempPallete.frameY = y;
			tempPallete.type = 1;
			mObjectPallete[y].push_back(tempPallete);
		}
	}

	//mSaveButton = RectMake(WINSIZEX / 2, WINSIZEY / 2, 200, 50);
	//mLoadButton = RectMake(mSaveButton.right + 10, WINSIZEY / 2, 200, 50);

	//버튼
	mRightArrowButton = new Button(rightArrow, WINSIZEX - 40, 210, rightArrow->GetFrameWidth() * 2, rightArrow->GetFrameHeight() * 2, bind(&MapToolScene::SwitchTilePallete, this));
	mRightArrowButton2 = new Button(rightArrow, WINSIZEX / 2+325, 480, rightArrow->GetFrameWidth()*2, rightArrow->GetFrameHeight()*2, bind(&MapToolScene::SwitchObjectPallete, this));
	mSaveButton = new Button(save, WINSIZEX / 2, WINSIZEY / 2, save->GetFrameWidth(), save->GetFrameHeight(), bind(&MapToolScene::Save, this,1));
	mLoadButton = new Button(load, WINSIZEX / 2 + 100, WINSIZEY / 2, load->GetFrameWidth(), load->GetFrameHeight(), bind(&MapToolScene::OpenLoadWindow, this));
	mUndoButton = new Button(undo, WINSIZEX / 2 + 200, WINSIZEY / 2, undo->GetFrameWidth(), undo->GetFrameHeight(), bind(&MapToolScene::Undo, this));
	mEraseButton = new Button(eraser, WINSIZEX / 2 + 325, 425, eraser->GetFrameWidth()*2, eraser->GetFrameHeight()*2, bind(&MapToolScene::EraseButton, this));
	
	for (int i = 0;i < 5;i++)
	{
		wstring str = L"Save" + to_wstring(i+1);
		Image* save = ImageManager::GetInstance()->FindImage(str);
		Button* tempButton = new Button(save, WINSIZEX / 2 + 80*i, WINSIZEY - 130, save->GetFrameWidth(), save->GetFrameHeight(), bind(&MapToolScene::Save, this,i));
		mSaveButtons.push_back(tempButton);
	}

	mCurrentTile = mPallete[0][0];
	mCurrentType = TileType::Normal;
	mCurrentPallete = CurrentPallete::Tile;

}

void MapToolScene::Release()
{
	
	ReleaseMap();

	SafeDelete(mSaveButton);
	SafeDelete(mLoadButton);
	SafeDelete(mUndoButton);
	SafeDelete(mEraseButton);
	SafeDelete(mRightArrowButton);
	SafeDelete(mRightArrowButton2);
}

void MapToolScene::Update()
{
	if (Input::GetInstance()->GetKeyDown(VK_RETURN))
	{
		Path::OpenFileDialog(L"", nullptr, L"../Resources/", nullptr, _hWnd);
	}


	if (Input::GetInstance()->GetKeyDown(VK_LBUTTON))
	{
		//팔레트 픽~
		for (int y = 0; y < mPallete.size(); ++y)
		{
			for (int x = 0; x < mPallete[y].size(); ++x)
			{
				if (PtInRect(&mPallete[y][x].rect, _mousePosition))
				{
					//팔레트교체
					mCurrentTile = mPallete[y][x];
					//들고있는종류바꾸고
					mCurrentPallete = CurrentPallete::Tile;
				}
			}
		}

		//타입픽
		for (int i = 0;i < TypePalleteCount; i++)
		{
			if (PtInRect(&mTypePallete[i].rect, _mousePosition))
			{
				mCurrentType = (TileType)mTypePallete[i].type;

				mCurrentPallete = CurrentPallete::Type;
			}
		}

		//옵젝픽
		for (int y = 0; y < mObjectPallete.size(); ++y)
		{
			for (int x = 0; x < mObjectPallete[y].size(); ++x)
			{
				if (PtInRect(&mObjectPallete[y][x].rect, _mousePosition))
				{
					//팔레트교체
					mCurrentObject = mObjectPallete[y][x];
					//들고있는종류바꾸고
					mCurrentPallete = CurrentPallete::Object;
				}
			}
		}
	}


	float x = CAMERAMANAGER->GetMainCamera()->GetX();
	float y = CAMERAMANAGER->GetMainCamera()->GetY();

	// {{ 타일 그리기~
	if (Input::GetInstance()->GetKey(VK_LBUTTON))
	{
		int indexX = (_mousePosition.x + (x - WINSIZEX / 2)) / TileSize;
		int indexY = (_mousePosition.y + (y - WINSIZEY / 2)) / TileSize;

		if (indexX >= 0 && indexX < mXTileCount &&
			indexY >= 0 && indexY < mYTileCount)
		{

			//타일바꾸기
			if (mCurrentPallete == CurrentPallete::Tile)
			{

				if (mTileList[indexY][indexX]->GetImage() != mCurrentTile.image ||
					mTileList[indexY][indexX]->GetFrameIndexX() != mCurrentTile.frameX ||
					mTileList[indexY][indexX]->GetFrameIndexY() != mCurrentTile.frameY)
				{
					IBrushTile* command = new IBrushTile(mTileList[indexY][indexX], mCurrentTile);
					PushCommand(command);
					//cout << "OnPushCommand" << endl;
				}

			}
			//속성바꾸기
			if (mCurrentPallete == CurrentPallete::Type)
			{
				if (mTileList[indexY][indexX]->GetTileType() != mCurrentType)
				{
					ISetTileType* command = new ISetTileType(mTileList[indexY][indexX], mCurrentType);
					PushCommand(command);
				}
			}
			//옵젝바꾸기
			if (mCurrentPallete == CurrentPallete::Object)
			{
				if (mMapObjectList[indexY][indexX]->GetImage() != mCurrentObject.image ||
					mMapObjectList[indexY][indexX]->GetFrameIndexX() != mCurrentObject.frameX ||
					mMapObjectList[indexY][indexX]->GetFrameIndexY() != mCurrentObject.frameY)
				{
					IPlaceObject* command = new IPlaceObject(mMapObjectList[indexY][indexX], mCurrentObject);
					PushCommand(command);
				}
			}

			//옵젝지우기
			if (mCurrentPallete == CurrentPallete::Erase)
			{
				if (mMapObjectList[indexY][indexX]->GetMapObjectType() != MapObjectType::None)
				{
					mMapObjectList[indexY][indexX]->SetObjectType(0);
					mMapObjectList[indexY][indexX]->SetImage(nullptr);
				}
			}


		}


	}
	// }}


	mSaveButton->Update();
	mLoadButton->Update();
	mUndoButton->Update();
	mEraseButton->Update();
	mRightArrowButton->Update();
	mRightArrowButton2->Update();
	for (int i = 0; i < mSaveButtons.size();i++)
	{
		mSaveButtons[i]->Update();
	}
}

void MapToolScene::Render(HDC hdc)
{
	//맵
	for (int y = 0; y < mTileList.size(); ++y)
	{
		for (int x = 0; x < mTileList[y].size(); ++x)
		{
			mTileList[y][x]->Render(hdc);
		}
	}

	//옵젝
	for (int y = 0; y < mMapObjectList.size(); ++y)
	{
		for (int x = 0; x < mMapObjectList[y].size(); ++x)
		{
			mMapObjectList[y][x]->Render(hdc);
		}
	}

	//팔렛
	for (int y = 0; y < mPallete.size(); ++y)
	{
		for (int x = 0; x < mPallete[y].size(); ++x)
		{
			mPallete[y][x].image->ScaleFrameRender
			(
				hdc,
				mPallete[y][x].rect.left,
				mPallete[y][x].rect.top,
				mPallete[y][x].frameX,
				mPallete[y][x].frameY,
				mPallete[y][x].width,
				mPallete[y][x].height
			);

			Gizmo::GetInstance()->DrawRect(hdc, mPallete[y][x].rect, Gizmo::Color::Red);
		}
	}

	//타입팔렛
	for (int i = 0;i < TypePalleteCount;i++)
	{
		switch (mTypePallete[i].tiletype)
		{
		case TileType::Normal:
			Gizmo::GetInstance()->DrawRect(hdc, mTypePallete[i].rect, Gizmo::Color::Green);
			break;

		case TileType::Wall:
			Gizmo::GetInstance()->DrawRect(hdc, mTypePallete[i].rect, Gizmo::Color::Black);
			break;

		case TileType::Water:
			Gizmo::GetInstance()->DrawRect(hdc, mTypePallete[i].rect, Gizmo::Color::Blue);
			break;

		case TileType::Lava:
			Gizmo::GetInstance()->DrawRect(hdc, mTypePallete[i].rect, Gizmo::Color::Red);
			break;

		case TileType::ice:
			Gizmo::GetInstance()->DrawRect(hdc, mTypePallete[i].rect, Gizmo::Color::Gray);
			break;

		default:
			break;
		}
	}

	//옵젝팔렛
	for (int y = 0; y < mObjectPallete.size(); ++y)
	{
		for (int x = 0; x < mObjectPallete[y].size(); ++x)
		{
			mObjectPallete[y][x].image->ScaleFrameRender
			(
				hdc,
				mObjectPallete[y][x].rect.left,
				mObjectPallete[y][x].rect.top,
				mObjectPallete[y][x].frameX,
				mObjectPallete[y][x].frameY,
				mObjectPallete[y][x].width,
				mObjectPallete[y][x].height
			);

		}
	}


	wstring str3 = L"P 씬전환";
	TextOut(hdc, 500, 50, str3.c_str(), str3.length());
	mSaveButton->Render(hdc);
	mLoadButton->Render(hdc);
	mUndoButton->Render(hdc);
	mEraseButton->Render(hdc);
	mRightArrowButton->Render(hdc);
	mRightArrowButton2->Render(hdc);

	for (int i = 0; i < mSaveButtons.size();i++)
	{
		mSaveButtons[i]->Render(hdc);
	}


	wstring y = L"TileY: " + to_wstring(mYTileCount);
	TextOut(hdc, _mousePosition.x, _mousePosition.y, y.c_str(), (int)y.length());
	wstring x = L"TileX: " + to_wstring(mXTileCount);
	TextOut(hdc, _mousePosition.x, _mousePosition.y + 20, x.c_str(), (int)x.length());
}

void MapToolScene::Save(int i)
{
	wstring str = L"../Data/Save" + to_wstring(i+1) + L".txt";
	//ofstream saveStream(L"../Data/Test.txt");
	ofstream saveStream(str);
	if (saveStream.is_open())
	{
		string tempImageKey;
		int frameX;
		int frameY;

		//맵크기저장
		saveStream << mYTileCount;
		saveStream << ",";
		saveStream << mXTileCount;
		saveStream << endl;

		for (int y = 0; y < mYTileCount; ++y)
		{
			for (int x = 0; x < mXTileCount; ++x)
			{
				string str;
				wstring keyName;
				if (mTileList[y][x]->GetImage() != nullptr)
					keyName = mTileList[y][x]->GetImage()->GetKeyName();
				str.assign(keyName.begin(), keyName.end());

				saveStream << str.c_str();
				saveStream << ",";
				saveStream << mTileList[y][x]->GetFrameIndexX();
				saveStream << ",";
				saveStream << mTileList[y][x]->GetFrameIndexY();
				saveStream << ",";
				saveStream << (int)mTileList[y][x]->GetTileType();
				saveStream << ",";


				if (mMapObjectList[y][x]->GetImage() != nullptr)
					keyName = mMapObjectList[y][x]->GetImage()->GetKeyName();
				else
					keyName = L"none";
				str.assign(keyName.begin(), keyName.end());

				saveStream << str.c_str();
				saveStream << ",";
				saveStream << mMapObjectList[y][x]->GetFrameIndexX();
				saveStream << ",";
				saveStream << mMapObjectList[y][x]->GetFrameIndexY();
				saveStream << ",";
				saveStream << (int)mMapObjectList[y][x]->GetMapObjectType();

				saveStream << endl;
			}
		}
	}
}

void MapToolScene::Load(wstring fileName)
{
	//ifstream loadStream(L"../Data/Test.txt");
	ReleaseMap();
	ifstream loadStream(fileName);
	if (loadStream.is_open())
	{
		string buffer;
		getline(loadStream, buffer, ',');
		mYTileCount = stoi(buffer);
		getline(loadStream, buffer);
		mXTileCount = stoi(buffer);
		InitEmptyMap();

		for (int y = 0; y < mYTileCount; ++y)
		{
			for (int x = 0; x < mXTileCount; ++x)
			{
				string key;
				int frameX;
				int frameY;
				int type;
				string objectKey;
				int objectFrameX;
				int objectFrameY;
				int objectType;


				getline(loadStream, buffer, ',');
				key = buffer;
				getline(loadStream, buffer, ',');
				frameX = stoi(buffer);
				getline(loadStream, buffer, ',');
				frameY = stoi(buffer);
				getline(loadStream, buffer, ',');
				type = stoi(buffer);

				//옵젝
				getline(loadStream, buffer, ',');
				objectKey = buffer;
				getline(loadStream, buffer, ',');
				objectFrameX = stoi(buffer);
				getline(loadStream, buffer, ',');
				objectFrameY = stoi(buffer);
				getline(loadStream, buffer);
				objectType = stoi(buffer);

				wstring wstr;
				wstr.assign(key.begin(), key.end());
				mTileList[y][x]->SetImage(ImageManager::GetInstance()->FindImage(wstr));
				mTileList[y][x]->SetFrameIndexX(frameX);
				mTileList[y][x]->SetFrameIndexY(frameY);
				mTileList[y][x]->SetTileType((TileType)type);

				if (objectKey == "none")
					mMapObjectList[y][x]->SetImage(nullptr);
				else
				{
					wstr.assign(objectKey.begin(), objectKey.end());
					mMapObjectList[y][x]->SetImage(ImageManager::GetInstance()->FindImage(wstr));
				}
				mMapObjectList[y][x]->SetFrameIndexX(objectFrameX);
				mMapObjectList[y][x]->SetFrameIndexY(objectFrameY);
				mMapObjectList[y][x]->SetObjectType(objectType);
			}
		}
	}
}

void MapToolScene::OpenLoadWindow()
{
	Path::OpenFileDialog(L"", nullptr, L"../Resources/", bind(&MapToolScene::Load,this, placeholders::_1), _hWnd);
}


void MapToolScene::PushCommand(ICommand* command)
{
	command->Execute();
	mCommandList.emplace(command);
}

void MapToolScene::Undo()
{
	if (mCommandList.size() == 0)
		return;

	mCommandList.top()->Reset();
	mCommandList.pop();
}

void MapToolScene::Redo()
{

}

void MapToolScene::EraseButton()
{
	mCurrentPallete = CurrentPallete::Erase;
}

void MapToolScene::SwitchObjectPallete()
{
	Image* mineralImage;
	MapObjectType type;
	//바꿀 이미지랑 속성 정하고
	if (mObjectPallete[0][0].type == (int)MapObjectType::Green)
	{
		mineralImage = IMAGEMANAGER->FindImage(L"BlueMineral");
		type = MapObjectType::Blue;
	}
	else if (mObjectPallete[0][0].type == (int)MapObjectType::Blue)
	{
		mineralImage = IMAGEMANAGER->FindImage(L"RedMineral");
		type = MapObjectType::Red;
	}
	else
	{
		mineralImage = IMAGEMANAGER->FindImage(L"GreenMineral");
		type = MapObjectType::Green;
	}
	
	//이미지랑 속성바꾸기
	for (int y = 0; y < mObjectPallete.size(); ++y)
	{
		for (int x = 0; x < mObjectPallete[y].size(); ++x)
		{
			mObjectPallete[y][x].image = mineralImage;
			mObjectPallete[y][x].type = (int)type;
		}
	}
}

void MapToolScene::SwitchTilePallete()
{
	Image* mapImage;
	//바꿀이미지 정하기
	if (mPallete[0][0].image->GetKeyName() == L"TinyWoods")
	{
		mapImage = IMAGEMANAGER->FindImage(L"MagmaCavern");
	}
	else if (mPallete[0][0].image->GetKeyName() == L"MagmaCavern")
	{
		mapImage = IMAGEMANAGER->FindImage(L"MtFarAway");
	}
	else
	{
		mapImage = IMAGEMANAGER->FindImage(L"TinyWoods");
	}

	//이미지바꾸기
	for (int y = 0; y < mPallete.size(); ++y)
	{
		for (int x = 0; x < mPallete[y].size(); ++x)
		{
			mPallete[y][x].image = mapImage;
		}
	}


}

void MapToolScene::InitEmptyMap()
{
	Image* tileImage = ImageManager::GetInstance()->FindImage(L"TinyWoods");

	//빈맵 인잇
	for (int y = 0; y < mYTileCount; ++y)
	{
		mTileList.push_back(vector <Tile*>());
		for (int x = 0; x < mXTileCount; ++x)
		{
			//벡터
			Tile* tempTile = new Tile
			(
				tileImage,
				TileSize * x,
				TileSize * y,
				TileSize,
				TileSize,
				Random::GetInstance()->RandomInt(2),
				0,
				0
			);

			mTileList[y].push_back(tempTile);

		}
	}
	//빈맵옵젝 인잇
	for (int y = 0; y < mYTileCount; ++y)
	{
		mMapObjectList.push_back(vector <MapObject*>());
		for (int x = 0; x < mXTileCount; ++x)
		{

			//벡터
			MapObject* tempObject = new MapObject
			(
				nullptr,
				TileSize * x,
				TileSize * y,
				TileSize,
				TileSize,
				0,
				0,
				0
			);
			mMapObjectList[y].push_back(tempObject);

		}
	}
}

void MapToolScene::ReleaseMap()
{
	//타일 릴리즈
	for (int y = 0; y < mTileList.size(); ++y)
	{
		for (int x = 0; x < mTileList[y].size(); ++x)
		{
			SafeDelete(mTileList[y][x]);
			mTileList[y].erase(mTileList[y].begin() + x);
			x--;
		}
		if (mTileList[y].size() <= 0)
		{
			mTileList.erase(mTileList.begin() + y);
			y--;
		}
	}
	//옵젝 릴리즈
	for (int y = 0; y < mMapObjectList.size(); ++y)
	{
		for (int x = 0; x < mMapObjectList[y].size(); ++x)
		{
			SafeDelete(mMapObjectList[y][x]);
			mMapObjectList[y].erase(mMapObjectList[y].begin() + x);
			x--;
		}
		if (mMapObjectList[y].size() <= 0)
		{
			mMapObjectList.erase(mMapObjectList.begin() + y);
			y--;
		}
	}

}

void MapToolScene::ImageLoad()
{
	//IMAGEMANAGER->LoadFromFile(L"Save", Resources(L"/Map/Tool/save"), 60, 16, 1, 1, true);
	//IMAGEMANAGER->LoadFromFile(L"Load", Resources(L"/Map/Tool/load"), 60, 16, 1, 1, true);
	//IMAGEMANAGER->LoadFromFile(L"Undo", Resources(L"/Map/Tool/undo"), 60, 16, 1, 1, true);
	//IMAGEMANAGER->LoadFromFile(L"TinyWoods", Resources(L"/Map/TinyWoods"), 288, 144, 12, 6, true);
	//IMAGEMANAGER->LoadFromFile(L"GreenMineral", Resources(L"/Map/Green"), 144, 72, 6, 3, true);
	//IMAGEMANAGER->LoadFromFile(L"BlueMineral", Resources(L"/Map/Blue"), 144, 72, 6, 3, true);
	//IMAGEMANAGER->LoadFromFile(L"RedMineral", Resources(L"/Map/Red"), 144, 72, 6, 3, true);
	//IMAGEMANAGER->LoadFromFile(L"RightArrow", Resources(L"/Map/Tool/RightArrow"), 16, 14, 1, 1, true);
	//IMAGEMANAGER->LoadFromFile(L"XTile", Resources(L"/Map/Tool/XTile"), 24, 24, 1, 1, true);
}

 LRESULT MapToolScene::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) 
 {
	//wParam
	 return 0;
 }



 //////마우스표시관련//////
 void MapToolScene::InitMouseRect()
 {
	 mMouse.image = mCurrentTile.image;
	 mMouse.frameX = mCurrentTile.frameX;
	 mMouse.frameY = mCurrentTile.frameY;
	 mMouse.positionX = _mousePosition.x;
	 mMouse.positionY = _mousePosition.y;
	 mMouse.sizeX = TileSize;
	 mMouse.sizeY = TileSize;
	 mMouse.rect = RectMakeCenter(mMouse.positionX, mMouse.positionY, mMouse.sizeX, mMouse.sizeY);
 }

 void MapToolScene::UpdateMouseRect()
 {
	 mMouse.positionX = _mousePosition.x;
	 mMouse.positionY = _mousePosition.y;
	 mMouse.rect = RectMakeCenter(mMouse.positionX, mMouse.positionY, mMouse.sizeX, mMouse.sizeY);
 }

 void MapToolScene::RenderMouseRect(HDC hdc)
 {
	 switch (mCurrentPallete)
	 {
	 case CurrentPallete::Tile:
		 mMouse.image->AlphaScaleFrameRender(hdc, mMouse.rect.left, mMouse.rect.top, mMouse.frameX, mMouse.frameY, mMouse.sizeX, mMouse.sizeY, 0.2);
		 break;
	 case CurrentPallete::Object:
		 mMouse.image->AlphaScaleFrameRender(hdc, mMouse.rect.left, mMouse.rect.top, mMouse.frameX, mMouse.frameY, mMouse.sizeX, mMouse.sizeY, 0.2);
		 break;
	 case CurrentPallete::Type:
		 switch (mCurrentType)
		 {
		 case TileType::Normal:
			 Gizmo::GetInstance()->DrawRect(hdc, mMouse.rect, Gizmo::Color::Green);
			 break;

		 case TileType::Wall:
			 Gizmo::GetInstance()->DrawRect(hdc, mMouse.rect, Gizmo::Color::Black);
			 break;

		 case TileType::Water:
			 Gizmo::GetInstance()->DrawRect(hdc, mMouse.rect, Gizmo::Color::Blue);
			 break;

		 case TileType::Lava:
			 Gizmo::GetInstance()->DrawRect(hdc, mMouse.rect, Gizmo::Color::Red);
			 break;

		 case TileType::ice:
			 Gizmo::GetInstance()->DrawRect(hdc, mMouse.rect, Gizmo::Color::Gray);
			 break;

		 default:
			 break;
		 }
		 break;
	 case CurrentPallete::Erase:
		 mMouse.image->AlphaScaleFrameRender(hdc, mMouse.rect.left, mMouse.rect.top, mMouse.frameX, mMouse.frameY, mMouse.sizeX, mMouse.sizeY, 0.3);
		 break;
	 default:
		 break;
	 }
 }

 void MapToolScene::SetMouseRect()
 {
	 switch (mCurrentPallete)
	 {
	 case CurrentPallete::Tile:
		 mMouse.image = mCurrentTile.image;
		 mMouse.frameX = mCurrentTile.frameX;
		 mMouse.frameY = mCurrentTile.frameY;
		 break;
	 case CurrentPallete::Object:
		 mMouse.image = mCurrentObject.image;
		 mMouse.frameX = mCurrentObject.frameX;
		 mMouse.frameY = mCurrentObject.frameY;
		 break;
	 case CurrentPallete::Type:
		 mMouse.image = nullptr;
		 break;
	 case CurrentPallete::Erase:
		 mMouse.image = IMAGEMANAGER->FindImage(L"XTile");
		 break;
	 default:
		 break;
	 }
 }