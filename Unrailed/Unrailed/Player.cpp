#include "pch.h"
#include "Player.h"

#include "Image.h"
#include "Animation.h"
#include "Camera.h"
#include "Tile.h"
#include "MapObject.h"
#include "Ore.h"
#include "TrailManager.h"

Player::Player(const string& name, float x, float y)
	:GameObject(name, x, y)
{
}

void Player::Init()
{
	mImage = IMAGEMANAGER->FindImage(L"ditto");

	InitAnimation();

	mCurrentAnimation = mDownIdleAnimation;
	mCurrentAnimation->Play();

	mSizeX = mImage->GetFrameWidth() * 2.f;
	mSizeY = mImage->GetFrameHeight() * 2.f;
	mRect = RectMakeCenter((int)mX, (int)mY, (int)mSizeX, (int)mSizeY);

	mColBox = RectMakeCenter((int)mX, (int)mY, TileSize, TileSize);

	mDir = DirectionEight::Down;
	mState = State::Idle;

	mSpeed = 100.f;

	mInputType = 0;

	mForm = Form::Ditto;

	mChangeT = 0.f;

	mIsDirectionKeyDown = false;

	mTileX = 0;
	mTileY = 0;

	mNextTileX = 0;
	mNextTileY = 0;

	mIsAttackingTemp = false; // 상호작용 전 테스트 변수

	mIsGettingItemThisFrame = false;
}

void Player::Release()
{
	SafeDeleteAnimation();
}

void Player::Update()
{
	mTileX = (int)(mX / TileSize);
	mTileY = (int)(mY / TileSize);
	// 현재 플레이어 사이즈 늘려주면서 mx, my가 제대로 잡히지 않고 있음, 타일은 스케일링 어떻게 하고 있는지 파악하기
	// 플레이어는 그냥 현재 이미지에 렌더만 배수 늘려줘서 이상함
	// 원래 크기로는 딱 중심, 2배 크기로는 왼쪽 윗부분 잡힘

	InputDirectionKey();

	InputSpaceKey();

	InputCKey();

	//ChangeForm(); // 지울 예정인 함수

	ChangeCurrentAnimation();

	// 플레이어 업데이트 시 기능 추가하려면 여기에 추가하기

	mCurrentAnimation->Update();
}

void Player::Render(HDC hdc)
{
#ifdef DEBUG
	RenderRect(hdc, mColBox);
#endif
	// 카메라 기준 렌더링(유찬)
	/*CameraManager::GetInstance()->GetMainCamera()
		->ScaleFrameRender(hdc, mImage, mRect.left, mRect.top,
			mCurrentAnimation->GetNowFrameX(), mCurrentAnimation->GetNowFrameY(),
			mImage->GetFrameWidth() * 2, mImage->GetFrameHeight() * 2);*/

	// 임시로 카메라 고정형 렌더링
	mImage->ScaleFrameRender(hdc, mRect.left, mRect.top,
		mCurrentAnimation->GetNowFrameX(), mCurrentAnimation->GetNowFrameY(), (int)mSizeX, (int)mSizeY);

	//mImage->FrameRender(hdc,mRect.left,mRect.top, mCurrentAnimation->GetNowFrameX(),
	//	mCurrentAnimation->GetNowFrameY());

	RenderTestText(hdc);
}

void Player::InitAnimation()
{
	mDownIdleAnimation = new Animation();
	mDownIdleAnimation->InitFrameByStartEnd(0, 0, 1, 0, false);
	mDownIdleAnimation->SetIsLoop(true);
	mDownIdleAnimation->SetFrameUpdateTime(0.3f);

	mUpIdleAnimation = new Animation();
	mUpIdleAnimation->InitFrameByStartEnd(0, 1, 1, 1, false);
	mUpIdleAnimation->SetIsLoop(true);
	mUpIdleAnimation->SetFrameUpdateTime(0.3f);

	mLeftIdleAnimation = new Animation();
	mLeftIdleAnimation->InitFrameByStartEnd(0, 2, 1, 2, false);
	mLeftIdleAnimation->SetIsLoop(true);
	mLeftIdleAnimation->SetFrameUpdateTime(0.3f);

	mRightIdleAnimation = new Animation();
	mRightIdleAnimation->InitFrameByStartEnd(0, 3, 1, 3, false);
	mRightIdleAnimation->SetIsLoop(true);
	mRightIdleAnimation->SetFrameUpdateTime(0.3f);

	mLDIdleAnimation = new Animation();
	mLDIdleAnimation->InitFrameByStartEnd(0, 4, 1, 4, false);
	mLDIdleAnimation->SetIsLoop(true);
	mLDIdleAnimation->SetFrameUpdateTime(0.3f);

	mRDIdleAnimation = new Animation();
	mRDIdleAnimation->InitFrameByStartEnd(0, 5, 1, 5, false);
	mRDIdleAnimation->SetIsLoop(true);
	mRDIdleAnimation->SetFrameUpdateTime(0.3f);

	mLUIdleAnimation = new Animation();
	mLUIdleAnimation->InitFrameByStartEnd(0, 6, 1, 6, false);
	mLUIdleAnimation->SetIsLoop(true);
	mLUIdleAnimation->SetFrameUpdateTime(0.3f);

	mRUIdleAnimation = new Animation();
	mRUIdleAnimation->InitFrameByStartEnd(0, 7, 1, 7, false);
	mRUIdleAnimation->SetIsLoop(true);
	mRUIdleAnimation->SetFrameUpdateTime(0.3f);


	mDownMoveAnimation = new Animation();
	mDownMoveAnimation->InitFrameByStartEnd(0, 8, 2, 8, false);
	mDownMoveAnimation->SetIsLoop(true);
	mDownMoveAnimation->SetFrameUpdateTime(0.3f);

	mUpMoveAnimation = new Animation();
	mUpMoveAnimation->InitFrameByStartEnd(0, 9, 2, 9, false);
	mUpMoveAnimation->SetIsLoop(true);
	mUpMoveAnimation->SetFrameUpdateTime(0.3f);

	mLeftMoveAnimation = new Animation();
	mLeftMoveAnimation->InitFrameByStartEnd(0, 10, 2, 10, false);
	mLeftMoveAnimation->SetIsLoop(true);
	mLeftMoveAnimation->SetFrameUpdateTime(0.3f);

	mRightMoveAnimation = new Animation();
	mRightMoveAnimation->InitFrameByStartEnd(0, 11, 2, 11, false);
	mRightMoveAnimation->SetIsLoop(true);
	mRightMoveAnimation->SetFrameUpdateTime(0.3f);

	mLDMoveAnimation = new Animation();
	mLDMoveAnimation->InitFrameByStartEnd(0, 12, 2, 12, false);
	mLDMoveAnimation->SetIsLoop(true);
	mLDMoveAnimation->SetFrameUpdateTime(0.3f);

	mRDMoveAnimation = new Animation();
	mRDMoveAnimation->InitFrameByStartEnd(0, 13, 2, 13, false);
	mRDMoveAnimation->SetIsLoop(true);
	mRDMoveAnimation->SetFrameUpdateTime(0.3f);

	mLUMoveAnimation = new Animation();
	mLUMoveAnimation->InitFrameByStartEnd(0, 14, 2, 14, false);
	mLUMoveAnimation->SetIsLoop(true);
	mLUMoveAnimation->SetFrameUpdateTime(0.3f);

	mRUMoveAnimation = new Animation();
	mRUMoveAnimation->InitFrameByStartEnd(0, 15, 2, 15, false);
	mRUMoveAnimation->SetIsLoop(true);
	mRUMoveAnimation->SetFrameUpdateTime(0.3f);
}

void Player::SafeDeleteAnimation()
{
	SafeDelete(mDownIdleAnimation);
	SafeDelete(mUpIdleAnimation);
	SafeDelete(mLeftIdleAnimation);
	SafeDelete(mRightIdleAnimation);
	SafeDelete(mLDIdleAnimation);
	SafeDelete(mRDIdleAnimation);
	SafeDelete(mLUIdleAnimation);
	SafeDelete(mRUIdleAnimation);
	SafeDelete(mDownMoveAnimation);
	SafeDelete(mUpMoveAnimation);
	SafeDelete(mLeftMoveAnimation);
	SafeDelete(mRightMoveAnimation);
	SafeDelete(mLDMoveAnimation);
	SafeDelete(mRDMoveAnimation);
	SafeDelete(mLUMoveAnimation);
	SafeDelete(mRUMoveAnimation);
}

void Player::InputDirectionKey()
{
	if (INPUT->GetKeyDown(VK_DOWN))
	{
		mDir = DirectionEight::Down;
		mIsDirectionKeyDown = true;
	}
	else if (INPUT->GetKeyDown(VK_UP))
	{
		mDir = DirectionEight::Up;
		mIsDirectionKeyDown = true;
	}
	else if (INPUT->GetKeyDown(VK_LEFT))
	{
		mDir = DirectionEight::Left;
		mIsDirectionKeyDown = true;
	}
	else if (INPUT->GetKeyDown(VK_RIGHT))
	{
		mDir = DirectionEight::Right;
		mIsDirectionKeyDown = true;
	}

	if (INPUT->GetKeyAKeyDownB(VK_DOWN, VK_UP))
	{
		mInputType = 2; // 예외 2: up 입력 처리
		mIsDirectionKeyDown = true;
	}
	else if (INPUT->GetKeyAKeyDownB(VK_DOWN, VK_LEFT))
	{
		mDir = DirectionEight::LeftDown;
		mIsDirectionKeyDown = true;
	}
	else if (INPUT->GetKeyAKeyDownB(VK_DOWN, VK_RIGHT))
	{
		mDir = DirectionEight::RightDown;
		mIsDirectionKeyDown = true;
	}
	else if (INPUT->GetKeyAKeyDownB(VK_UP, VK_DOWN))
	{
		mInputType = 1; // 예외 1: down 입력 처리
		mIsDirectionKeyDown = true;
	}
	else if (INPUT->GetKeyAKeyDownB(VK_UP, VK_LEFT))
	{
		mDir = DirectionEight::LeftUp;
		mIsDirectionKeyDown = true;
	}
	else if (INPUT->GetKeyAKeyDownB(VK_UP, VK_RIGHT))
	{
		mDir = DirectionEight::RightUp;
		mIsDirectionKeyDown = true;
	}
	else if (INPUT->GetKeyAKeyDownB(VK_LEFT, VK_DOWN))
	{
		mDir = DirectionEight::LeftDown;
		mIsDirectionKeyDown = true;
	}
	else if (INPUT->GetKeyAKeyDownB(VK_LEFT, VK_UP))
	{
		mDir = DirectionEight::LeftUp;
		mIsDirectionKeyDown = true;
	}
	else if (INPUT->GetKeyAKeyDownB(VK_LEFT, VK_RIGHT))
	{
		mInputType = 4; // 예외 4: right 입력 처리
		mIsDirectionKeyDown = true;
	}
	else if (INPUT->GetKeyAKeyDownB(VK_RIGHT, VK_DOWN))
	{
		mDir = DirectionEight::RightDown;
		mIsDirectionKeyDown = true;
	}
	else if (INPUT->GetKeyAKeyDownB(VK_RIGHT, VK_UP))
	{
		mDir = DirectionEight::RightUp;
		mIsDirectionKeyDown = true;

	}
	else if (INPUT->GetKeyAKeyDownB(VK_RIGHT, VK_LEFT))
	{
		mInputType = 3; // 예외 3: left 입력 처리
		mIsDirectionKeyDown = true;
	}

	if (INPUT->GetKeyAKeyUpB(VK_DOWN, VK_UP))
	{
		mInputType = 1; // 예외 1: down 입력 처리
		mIsDirectionKeyDown = true;
	}
	else if (INPUT->GetKeyAKeyUpB(VK_DOWN, VK_LEFT))
	{
		mInputType = 1; // 예외 1: down 입력 처리
		mIsDirectionKeyDown = true;
	}
	else if (INPUT->GetKeyAKeyUpB(VK_DOWN, VK_RIGHT))
	{
		mInputType = 1; // 예외 1: down 입력 처리
		mIsDirectionKeyDown = true;
	}
	else if (INPUT->GetKeyAKeyUpB(VK_UP, VK_DOWN))
	{
		mInputType = 2; // 예외 2: up 입력 처리
		mIsDirectionKeyDown = true;
	}
	else if (INPUT->GetKeyAKeyUpB(VK_UP, VK_LEFT))
	{
		mInputType = 2; // 예외 2: up 입력 처리
		mIsDirectionKeyDown = true;
	}
	else if (INPUT->GetKeyAKeyUpB(VK_UP, VK_RIGHT))
	{
		mInputType = 2; // 예외 2: up 입력 처리
		mIsDirectionKeyDown = true;
	}
	else if (INPUT->GetKeyAKeyUpB(VK_LEFT, VK_DOWN))
	{
		mInputType = 3; // 예외 3: left 입력 처리
		mIsDirectionKeyDown = true;
	}
	else if (INPUT->GetKeyAKeyUpB(VK_LEFT, VK_UP))
	{
		mInputType = 3; // 예외 3: left 입력 처리
		mIsDirectionKeyDown = true;
	}
	else if (INPUT->GetKeyAKeyUpB(VK_LEFT, VK_RIGHT))
	{
		mInputType = 3; // 예외 3: left 입력 처리
		mIsDirectionKeyDown = true;
	}
	else if (INPUT->GetKeyAKeyUpB(VK_RIGHT, VK_DOWN))
	{
		mInputType = 4; // 예외 4: right 입력 처리
		mIsDirectionKeyDown = true;
	}
	else if (INPUT->GetKeyAKeyUpB(VK_RIGHT, VK_UP))
	{
		mInputType = 4; // 예외 4: right 입력 처리
		mIsDirectionKeyDown = true;
	}
	else if (INPUT->GetKeyAKeyUpB(VK_RIGHT, VK_LEFT))
	{
		mInputType = 4; // 예외 4: right 입력 처리
		mIsDirectionKeyDown = true;
	}
	else if (INPUT->GetKeyUp(VK_DOWN))
	{
		mState = State::Idle;
		mInputType = 0;
		mIsDirectionKeyDown = false;
	}
	else if (INPUT->GetKeyUp(VK_UP))
	{
		mState = State::Idle;
		mInputType = 0;
		mIsDirectionKeyDown = false;
	}
	else if (INPUT->GetKeyUp(VK_LEFT))
	{
		mState = State::Idle;
		mInputType = 0;
		mIsDirectionKeyDown = false;
	}
	else if (INPUT->GetKeyUp(VK_RIGHT))
	{
		mState = State::Idle;
		mInputType = 0;
		mIsDirectionKeyDown = false;
	}

	if (mInputType == 1)
	{
		mDir = DirectionEight::Down;
		mInputType = 0;
	}
	else if (mInputType == 2)
	{
		mDir = DirectionEight::Up;
		mInputType = 0;
	}
	else if (mInputType == 3)
	{
		mDir = DirectionEight::Left;
		mInputType = 0;
	}
	else if (mInputType == 4)
	{
		mDir = DirectionEight::Right;
		mInputType = 0;
	}

	if (mIsDirectionKeyDown)
	{
		if (mSpeed == 100.f && INPUT->GetKeyDown(VK_LSHIFT))
		{
			mSpeed = 300.f;
		}

		if (mSpeed != 100.f)
		{
			mSpeed -= 1.f;

			if (mSpeed <= 275.f)
			{
				mSpeed = 100.f;
			}
		}
	}


	if (INPUT->GetKey(VK_DOWN))
	{
		mState = State::Move;

		mNextTileX = mTileX;
		if (mTileY == TileCountY - 1)
			mNextTileY = mTileY;
		else
			mNextTileY = mTileY + 1;

		if (mY >= TileSize * TileCountY - TileSize / 2)
		{
			mY = TileSize * TileCountY - TileSize / 2;
		}
		else
			mY += mSpeed * Time::GetInstance()->DeltaTime();
	}
	if (INPUT->GetKey(VK_UP))
	{
		mState = State::Move;

		mNextTileX = mTileX;
		if (mTileY == 0)
			mNextTileY = mTileY;
		else
			mNextTileY = mTileY - 1;

		if (mY <= TileSize / 2)
		{
			mY = TileSize / 2;
		}
		else
			mY -= mSpeed * Time::GetInstance()->DeltaTime();
	}
	if (INPUT->GetKey(VK_LEFT))
	{
		mState = State::Move;

		if (mTileX == 0)
			mNextTileX = mTileX;
		else
			mNextTileX = mTileX - 1;
		mNextTileY = mTileY;

		if (mX <= TileSize / 2)
		{
			mX = TileSize / 2;
		}
		else
			mX -= mSpeed * Time::GetInstance()->DeltaTime();
	}
	if (INPUT->GetKey(VK_RIGHT))
	{
		mState = State::Move;

		if (mTileX == TileCountX - 1)
			mNextTileX = mTileX;
		else
			mNextTileX = mTileX + 1;
		mNextTileY = mTileY;

		if (mX >= TileSize * TileCountX - TileSize / 2)
		{
			mX = TileSize * TileCountX - TileSize / 2;
		}
		else
			mX += mSpeed * Time::GetInstance()->DeltaTime();
	}
	mRect = RectMakeCenter((int)mX, (int)mY, (int)mSizeX, (int)mSizeY);
	mColBox = RectMakeCenter((int)mX, (int)mY, TileSize, TileSize);
}

void Player::InputSpaceKey()
{
	mIsGettingItemThisFrame = false;

	if (INPUT->GetKeyDown(VK_SPACE))
	{
		CheckNextTile();

		// {{ 맵오브젝트(벽) -> 아이템(광물) // 광물 캐기
		if ((mForm == Form::Chikorita) && ((*mMapObjectListPtr)[mNextTileY][mNextTileX]->GetMapObjectType() == MapObjectType::Green))
		{
			COLLISIONMANAGER->MapObjectCollision(this, &mColBox, mTileListPtr, mMapObjectListPtr);
			mIsAttackingTemp = true;
			// 어택 시작 > 광물 체력 감소 > 광물 체력 0이면 광물 타입 바꾸고 비활성화? > 아이템 UI에 추가
		}
		else if ((mForm == Form::Totodile) && ((*mMapObjectListPtr)[mNextTileY][mNextTileX]->GetMapObjectType() == MapObjectType::Blue))
		{
			COLLISIONMANAGER->MapObjectCollision(this, &mColBox, mTileListPtr, mMapObjectListPtr);
			mIsAttackingTemp = true;
			// 어택 시작 > 광물 체력 감소 > 광물 체력 0이면 광물 타입 바꾸고 비활성화? > 아이템 UI에 추가
		}
		else if ((mForm == Form::Charmander) && ((*mMapObjectListPtr)[mNextTileY][mNextTileX]->GetMapObjectType() == MapObjectType::Red))
		{
			COLLISIONMANAGER->MapObjectCollision(this, &mColBox, mTileListPtr, mMapObjectListPtr);
			mIsAttackingTemp = true;
			// 어택 시작 > 광물 체력 감소 > 광물 체력 0이면 광물 타입 바꾸고 비활성화? > 아이템 UI에 추가
		}
		else
			mIsAttackingTemp = false;
		// }} 맵오브젝트(벽) -> 아이템(광물) // 광물 캐기

		// {{ 아이템(광물) -> 인벤 소지 아이템(UI) // 아이템 줍기
		GameObject* item = COLLISIONMANAGER->ItemCollision(&mColBox);
		OreType invenType;

		if (mItemList.size())
			invenType = ((Ore*)mItemList[0])->GetOreType();
		
		if ((item != nullptr) && (item->GetIsActive()))
		{
			if ((mForm == Form::Chikorita) && (((Ore*)item)->GetOreType() == OreType::Green) && ((mItemList.size() == 0) || (invenType == OreType::Green)))
			{
				Ore* greenOre = new Ore();
				greenOre = (Ore*)item;
				mItemList.push_back((GameObject*)greenOre);
				item->SetIsActive(false);
				mIsGettingItemThisFrame = true;
			}
			else if ((mForm == Form::Totodile) && (((Ore*)item)->GetOreType() == OreType::Blue) && ((mItemList.size() == 0) || (invenType == OreType::Blue)))
			{
				Ore* blueOre = new Ore();
				blueOre = (Ore*)item;
				mItemList.push_back((GameObject*)blueOre);
				item->SetIsActive(false);
				mIsGettingItemThisFrame = true;
			}
			else if ((mForm == Form::Charmander) && (((Ore*)item)->GetOreType() == OreType::Red) && ((mItemList.size() == 0) || (invenType == OreType::Red)))
			{
				Ore* redOre = new Ore();
				redOre = (Ore*)item;
				mItemList.push_back((GameObject*)redOre);
				item->SetIsActive(false);
				mIsGettingItemThisFrame = true;
			}
		}
		// }} 아이템(광물) -> 인벤 소지 아이템(UI) // 아이템 줍기

		// {{ 인벤 소지 아이템(UI) -> 아이템(광물) // 아이템 버리기
		if (mItemList.size() && !mIsGettingItemThisFrame) // 아이템을 소지한 경우에만 아이템 버리기 가능 // 이번 프레임에서 얻은 경우가 아닐 때만 버리기 가능(이번 프레임에서 얻으면 바로 버려짐)
		{
			if (!((*mMapObjectListPtr)[mNextTileY][mNextTileX]->GetActive())) // 해당 위치에 맵 오브젝트도 nonActive 상태일 때만 버리기 가능하게.
			{
				vector<GameObject*> itemList = OBJECTMANAGER->GetObjectList(ObjectLayer::ITEM);
				vector<GameObject*>::iterator iter = itemList.begin();

				for (; iter != itemList.end(); ++iter)
				{
					float x = (*iter)->GetX();
					float y = (*iter)->GetY();

					int indexX = (int)(x / TileSize);
					int indexY = (int)(y / TileSize);

					if (indexX == mNextTileX && indexY == mNextTileY) // 해당 위치에 무슨 아이템이 있다
					{ // 아무것도 안할건데
					}
					else
					{
						Ore* greenOre = new Ore();
						greenOre->Drop(TileSize * mNextTileX, TileSize * mNextTileY, (int)invenType);
						OBJECTMANAGER->AddObject(ObjectLayer::ITEM, greenOre);

						mItemList.erase(mItemList.begin());
					}
				}
			}
			


			/*if (!((*mMapObjectListPtr)[mNextTileY][mNextTileX]->GetActive())) // 플레이어가 바라보는 다음 타일이 nonActive 상태일 때만 아이템 버리기 가능
			{  // 로드맵 함수 호출할 때 오브젝트 있는 곳에 액티브가 제대로 세팅이 안된 것 같은 느낌인데!! 오브젝트 있는 곳 없는 곳 구분없이 true인 것 같다...
				if (invenType == OreType::Green)
				{
					Ore* greenOre = new Ore();
					greenOre->Drop(TileSize * mNextTileX, TileSize * mNextTileY, (int)invenType);
					OBJECTMANAGER->AddObject(ObjectLayer::ITEM, greenOre);
					
					
					MapObject* greenObject = new MapObject(
						IMAGEMANAGER->FindImage(L"Ore"),
						TileSize * mNextTileX, TileSize * mNextTileY, TileSize, TileSize,
						0, 1, (int)(MapObjectType::Green)); // 레드0 그린1 블루2

 					(*mMapObjectListPtr)[mNextTileY][mNextTileX] = greenObject;

					//mItemList.push_back((GameObject*)greenOre);
					//item->SetIsAzzctive(false);

					mItemList.erase(mItemList.begin());
				}
			}*/
		}
		// }} 인벤 소지 아이템(UI) -> 아이템(광물) // 아이템 버리기

		// {{ 화살표 돌리기 // 얘는 키 분리해야 할 듯.. // 스페이스바는 화살표 줍기..

		//MapObjectType idontknowwhatitis = (*mMapObjectListPtr)[mNextTileY][mNextTileX]->GetMapObjectType(); // 이건 그냥 한거고
//		mTrailManager->TurnTrail(); // 이것부터 보면서 조건 등 주면서 고쳐야 함

//		if (!((*mMapObjectListPtr)[mNextTileY][mNextTileX]->GetActive())) // 플레이어가 바라보는 다음 타일이 nonActive 상태일 때만 아이템 버리기 가능
//		{}
		// }} 화살표 돌리기

		// (*mMapObjectListPtr)[mNextTileY][mNextTileX]->GetMapObjectType() == MapObjectType::Red))
	}
	else
		mIsAttackingTemp = false;
}

void Player::CheckNextTile()
{
	if (mDir == DirectionEight::Down)
	{
		mNextTileX = mTileX;
		mNextTileY = mTileY + 1;
	}
	else if (mDir == DirectionEight::Up)
	{
		mNextTileX = mTileX;
		mNextTileY = mTileY - 1;
	}
	else if (mDir == DirectionEight::Left)
	{
		mNextTileX = mTileX - 1;
		mNextTileY = mTileY;
	}
	else if (mDir == DirectionEight::Right)
	{
		mNextTileX = mTileX + 1;
		mNextTileY = mTileY;
	}
	else if (mDir == DirectionEight::LeftDown)
	{
		int width = (int)mX - mTileX * TileSize;
		int height = (mTileY + 1) * TileSize - (int)mY;

		if (width <= height) // left
		{
			mNextTileX = mTileX - 1;
			mNextTileY = mTileY;
		}
		else // down
		{
			mNextTileX = mTileX;
			mNextTileY = mTileY + 1;
		}
	}
	else if (mDir == DirectionEight::RightDown)
	{
		int width = (mTileX + 1) * TileSize - (int)mX;
		int height = (mTileY + 1) * TileSize - (int)mY;

		if (width <= height) // right
		{
			mNextTileX = mTileX + 1;
			mNextTileY = mTileY;
		}
		else // down
		{
			mNextTileX = mTileX;
			mNextTileY = mTileY + 1;
		}
	}
	else if (mDir == DirectionEight::LeftUp)
	{
		int width = (int)mX - mTileX * TileSize;
		int height = (int)mY - mTileY * TileSize;

		if (width <= height) // left
		{
			mNextTileX = mTileX - 1;
			mNextTileY = mTileY;
		}
		else // up
		{
			mNextTileX = mTileX;
			mNextTileY = mTileY - 1;
		}
	}
	else if (mDir == DirectionEight::RightUp)
	{
		int width = (mTileX + 1) * TileSize - (int)mX;
		int height = (int)mY - mTileY * TileSize;

		if (width <= height) // right
		{
			mNextTileX = mTileX + 1;
			mNextTileY = mTileY;
		}
		else // up
		{
			mNextTileX = mTileX;
			mNextTileY = mTileY - 1;
		}
	}
}

void Player::InputCKey()
{
	if (INPUT->GetKeyDown('C')) // Loop Order: normal -> leaf -> water -> fire -> normal -> ...
	{
		if (mForm == Form::Ditto)
		{
			mForm = Form::Chikorita;
			mImage = IMAGEMANAGER->FindImage(L"chikorita");
		}
		else if (mForm == Form::Chikorita)
		{
			mForm = Form::Totodile;
			mImage = IMAGEMANAGER->FindImage(L"totodile");
		}
		else if (mForm == Form::Totodile)
		{
			mForm = Form::Charmander;
			mImage = IMAGEMANAGER->FindImage(L"charmander");
		}
		else if (mForm == Form::Charmander)
		{
			mForm = Form::Ditto;
			mImage = IMAGEMANAGER->FindImage(L"ditto");
		}

		// {{ 이 변수 굳이 필요없으면 지우기 // 이펙트 등 사용할 거면 살려두기
		mChangeT = 2.f; 

		if (mChangeT)
		{
			mChangeT -= TIME->DeltaTime();

			if (mChangeT <= 0)
				mChangeT = 0.f;
		}
		// }} 이 변수 굳이 필요없으면 지우기 // 이펙트 등 사용할 거면 살려두기
	}
}

void Player::ChangeForm()
{
	if (INPUT->GetKeyDown('7'))
	{
		mForm = Form::Ditto;
		mImage = IMAGEMANAGER->FindImage(L"ditto");

		mChangeT = 2.f;
	}
	else if (INPUT->GetKeyDown('8'))
	{
		mForm = Form::Charmander;
		mImage = IMAGEMANAGER->FindImage(L"charmander");

		mChangeT = 2.f;
	}
	else if (INPUT->GetKeyDown('9'))
	{
		mForm = Form::Chikorita;
		mImage = IMAGEMANAGER->FindImage(L"chikorita");

		mChangeT = 2.f;
	}
	else if (INPUT->GetKeyDown('0'))
	{
		mForm = Form::Totodile;
		mImage = IMAGEMANAGER->FindImage(L"totodile");

		mChangeT = 2.f;
	}

	if (mChangeT)
	{
		mChangeT -= Time::GetInstance()->DeltaTime();

		if (mChangeT <= 0)
			mChangeT = 0.f;
	}
}

void Player::ChangeCurrentAnimation()
{
	if (mState == State::Idle)
	{
		if (mDir == DirectionEight::Down && mCurrentAnimation != mDownIdleAnimation)
		{
			mCurrentAnimation->Stop();
			mCurrentAnimation = mDownIdleAnimation;
			mCurrentAnimation->Play();
		}
		else if (mDir == DirectionEight::Up && mCurrentAnimation != mUpIdleAnimation)
		{
			mCurrentAnimation->Stop();
			mCurrentAnimation = mUpIdleAnimation;
			mCurrentAnimation->Play();
		}
		else if (mDir == DirectionEight::Left && mCurrentAnimation != mLeftIdleAnimation)
		{
			mCurrentAnimation->Stop();
			mCurrentAnimation = mLeftIdleAnimation;
			mCurrentAnimation->Play();
		}
		else if (mDir == DirectionEight::Right && mCurrentAnimation != mRightIdleAnimation)
		{
			mCurrentAnimation->Stop();
			mCurrentAnimation = mRightIdleAnimation;
			mCurrentAnimation->Play();
		}
		else if (mDir == DirectionEight::LeftDown && mCurrentAnimation != mLDIdleAnimation)
		{
			mCurrentAnimation->Stop();
			mCurrentAnimation = mLDIdleAnimation;
			mCurrentAnimation->Play();
		}
		else if (mDir == DirectionEight::RightDown && mCurrentAnimation != mRDIdleAnimation)
		{
			mCurrentAnimation->Stop();
			mCurrentAnimation = mRDIdleAnimation;
			mCurrentAnimation->Play();
		}
		else if (mDir == DirectionEight::LeftUp && mCurrentAnimation != mLUIdleAnimation)
		{
			mCurrentAnimation->Stop();
			mCurrentAnimation = mLUIdleAnimation;
			mCurrentAnimation->Play();
		}
		else if (mDir == DirectionEight::RightUp && mCurrentAnimation != mRUIdleAnimation)
		{
			mCurrentAnimation->Stop();
			mCurrentAnimation = mRUIdleAnimation;
			mCurrentAnimation->Play();
		}
	}
	else if (mState == State::Move)
	{
		if (mDir == DirectionEight::Down && mCurrentAnimation != mDownMoveAnimation)
		{
			mCurrentAnimation->Stop();
			mCurrentAnimation = mDownMoveAnimation;
			mCurrentAnimation->Play();
		}
		else if (mDir == DirectionEight::Up && mCurrentAnimation != mUpMoveAnimation)
		{
			mCurrentAnimation->Stop();
			mCurrentAnimation = mUpMoveAnimation;
			mCurrentAnimation->Play();
		}
		else if (mDir == DirectionEight::Left && mCurrentAnimation != mLeftMoveAnimation)
		{
			mCurrentAnimation->Stop();
			mCurrentAnimation = mLeftMoveAnimation;
			mCurrentAnimation->Play();
		}
		else if (mDir == DirectionEight::Right && mCurrentAnimation != mRightMoveAnimation)
		{
			mCurrentAnimation->Stop();
			mCurrentAnimation = mRightMoveAnimation;
			mCurrentAnimation->Play();
		}
		else if (mDir == DirectionEight::LeftDown && mCurrentAnimation != mLDMoveAnimation)
		{
			mCurrentAnimation->Stop();
			mCurrentAnimation = mLDMoveAnimation;
			mCurrentAnimation->Play();
		}
		else if (mDir == DirectionEight::RightDown && mCurrentAnimation != mRDMoveAnimation)
		{
			mCurrentAnimation->Stop();
			mCurrentAnimation = mRDMoveAnimation;
			mCurrentAnimation->Play();
		}
		else if (mDir == DirectionEight::LeftUp && mCurrentAnimation != mLUMoveAnimation)
		{
			mCurrentAnimation->Stop();
			mCurrentAnimation = mLUMoveAnimation;
			mCurrentAnimation->Play();
		}
		else if (mDir == DirectionEight::RightUp && mCurrentAnimation != mRUMoveAnimation)
		{
			mCurrentAnimation->Stop();
			mCurrentAnimation = mRUMoveAnimation;
			mCurrentAnimation->Play();
		}
	}
}

void Player::RenderTestText(HDC hdc)
{
	//wstring strInput = L"InputType : " + to_wstring(mInputType);
	//TextOut(hdc, 10, 100, strInput.c_str(), (int)strInput.length());

	wstring strChange = L"포켓몬 변!신! ";
	if (mChangeT)
		TextOut(hdc, (int)mX - 20, (int)mY - 25, strChange.c_str(), (int)strChange.length());

	//wstring strSpeed = L"Speed: " + to_wstring(mSpeed);
	//wstring strKeyDown = L"IsKeyDownCheck: " + to_wstring(mIsDirectionKeyDown);
	//TextOut(hdc, (int)mX, (int)mY + 50, strSpeed.c_str(), (int)strSpeed.length());
	//TextOut(hdc, (int)mX, (int)mY + 75, strKeyDown.c_str(), (int)strKeyDown.length());

	wstring strTile = L"tile x: " + to_wstring(mTileX) + L", y: " + to_wstring(mTileY);
	TextOut(hdc, (int)mX + 25, (int)mY, strTile.c_str(), (int)strTile.length());

	//wstring strAtk = L"공격 중이다";
	//if (mIsAttackingTemp)
	//	TextOut(hdc, (int)mX + 55, (int)mY, strAtk.c_str(), (int)strAtk.length());

	wstring strInven = L"Inven size: " + to_wstring(mItemList.size());
	TextOut(hdc, (int)mX + 25, (int)mY + 25, strInven.c_str(), (int)strInven.length());
}