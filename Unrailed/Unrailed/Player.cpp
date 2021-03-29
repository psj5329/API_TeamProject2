#include "pch.h"
#include "Player.h"

#include "Image.h"
#include "Camera.h"
#include "Animation.h"
#include "MapObject.h"
#include "TrailManager.h"
#include "Ore.h"
#include "Machop.h"
#include "Abra.h"
//#include "Bag.h"
#include "Inven.h"
#include "BagItem.h"
#include "Enemy.h"

Player::Player(const string& name, float x, float y)
	: GameObject(name, x, y)
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
	mState = PlayerState::Idle;

	mSpeed = 100.f;

	mInputType = 0;

	mForm = Form::Ditto;

	mChangeT = 0.f;

	mIsDirectionKeyDown = false;

	mTileX = 0;
	mTileY = 0;
	mNextTileX = 0;
	mNextTileY = 0;
	mRangeX = 0;
	mRangeY = 0;

	mIsAttackingTemp = false; // 상호작용 전 테스트 변수

	mIsGettingItemThisFrame = false;

	mIsChangable = true;

	mIsInfoOn = true;

	//mBag = new Bag();
	mInven = new Inven();
	mInven->Init();
	//mBagItemListPtr = mBag->GetBagItemListPtr();
	mBagItemListPtr = mInven->GetBagItemListPtr();

	// {{ 트레일 줍기 불가능해서 임시로 세팅
	//for (int i = 0; i < 3; ++i)
	//{
	//	BagItem* bagItem = new BagItem();
	//	bagItem->Init(ItemName::ItemTrail, ItemType::Green);
	//	mBagItemListPtr->push_back(bagItem);
	//}
	// 트레일 줍기 불가능해서 임시로 세팅 // 여기까지 }}
}

void Player::Release()
{
	SafeDeleteAnimation();
}

void Player::Update()
{
	mTileX = (int)(mX / TileSize);
	mTileY = (int)(mY / TileSize);

	InputDirectionKey();
	Move();
	CheckRange();

	InputSpaceKey();
	InputZKey();
	InputXKey();
	InputCKey();
	InputVKey();
	InputLKey(); // on/off gizmo
	InputCheatKey(); // for test

	ChangeCurrentAnimation();

	// 플레이어 업데이트 시 기능 추가하려면 여기에 추가하기

	mCurrentAnimation->Update();

	//mBag->Update((int)mX, (int)mY);
	mInven->Update();
}

void Player::Render(HDC hdc)
{
	//RenderRect(hdc, mColBox);

	// {{ 현재 타일, 다음 타일 확인용 // 유찬
	if (mIsInfoOn)
	{
		RECT currentRc = (*mTileListPtr)[mTileY][mTileX]->GetRect();
		RECT nextRc = (*mTileListPtr)[mNextTileY][mNextTileX]->GetRect();
		GIZMO->DrawRect(hdc, currentRc, Gizmo::Color::Yellow);
		GIZMO->DrawRect(hdc, nextRc, Gizmo::Color::Cyan);
		GIZMO->DrawRect(hdc, mRangeBox, Gizmo::Color::Purple);
		GIZMO->DrawRect(hdc, mColBox, Gizmo::Color::Violet);
	}
	// 현재 타일, 다음 타일 확인용 }}

	// 테스트 텍스트 // 릴리즈 전 지워주세요
	RenderTestText(hdc);

	CAMERAMANAGER->GetMainCamera()->ScaleFrameRender(hdc, mImage, mRect.left, mRect.top,
		mCurrentAnimation->GetNowFrameX(), mCurrentAnimation->GetNowFrameY(), (int)mSizeX, (int)mSizeY);

	//mBag->Render(hdc);
	mInven->Render(hdc);
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
		mState = PlayerState::Idle;
		mInputType = 0;
		mIsDirectionKeyDown = false;
	}
	else if (INPUT->GetKeyUp(VK_UP))
	{
		mState = PlayerState::Idle;
		mInputType = 0;
		mIsDirectionKeyDown = false;
	}
	else if (INPUT->GetKeyUp(VK_LEFT))
	{
		mState = PlayerState::Idle;
		mInputType = 0;
		mIsDirectionKeyDown = false;
	}
	else if (INPUT->GetKeyUp(VK_RIGHT))
	{
		mState = PlayerState::Idle;
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
			mSpeed = 300.f;

		if (mSpeed != 100.f)
		{
			mSpeed -= 1.f;

			if (mSpeed <= 275.f)
				mSpeed = 100.f;
		}
	}


	if (INPUT->GetKey(VK_DOWN))
	{
		mState = PlayerState::Move;

		mNextTileX = mTileX;
		if (mTileY == TileCountY - 1)
			mNextTileY = mTileY;
		else
			mNextTileY = mTileY + 1;

		if (mY >= TileSize * TileCountY - TileSize / 2)
			mY = TileSize * TileCountY - TileSize / 2;
		else
			mY += mSpeed * Time::GetInstance()->DeltaTime();
	}
	if (INPUT->GetKey(VK_UP))
	{
		mState = PlayerState::Move;

		mNextTileX = mTileX;
		if (mTileY == 0)
			mNextTileY = mTileY;
		else
			mNextTileY = mTileY - 1;

		if (mY <= TileSize / 2)
			mY = TileSize / 2;
		else
			mY -= mSpeed * Time::GetInstance()->DeltaTime();
	}
	if (INPUT->GetKey(VK_LEFT))
	{
		mState = PlayerState::Move;

		if (mTileX == 0)
			mNextTileX = mTileX;
		else
			mNextTileX = mTileX - 1;
		mNextTileY = mTileY;

		if (mX <= TileSize / 2)
			mX = TileSize / 2;
		else
			mX -= mSpeed * Time::GetInstance()->DeltaTime();
	}
	if (INPUT->GetKey(VK_RIGHT))
	{
		mState = PlayerState::Move;

		if (mTileX == TileCountX - 1)
			mNextTileX = mTileX;
		else
			mNextTileX = mTileX + 1;
		mNextTileY = mTileY;

		if (mX >= TileSize * TileCountX - TileSize / 2)
			mX = TileSize * TileCountX - TileSize / 2;
		else
			mX += mSpeed * Time::GetInstance()->DeltaTime();
	}
	mRect = RectMakeCenter((int)mX, (int)mY, (int)mSizeX, (int)mSizeY);
	mColBox = RectMakeCenter((int)mX, (int)mY, TileSize, TileSize);
}

void Player::Move()
{
	COLLISIONMANAGER->TileMapObjectCollision(this, &mColBox, mTileListPtr, mMapObjectListPtr, mTileX, mTileY);

	mRect = RectMakeCenter((int)mX, (int)mY, (int)mSizeX, (int)mSizeY);
	mColBox = RectMakeCenter((int)mX, (int)mY, TileSize, TileSize);
}

void Player::InputSpaceKey()
{
	mIsGettingItemThisFrame = false;

	if (INPUT->GetKeyDown(VK_SPACE))
	{
		CheckNextTile();

		vector<GameObject*>* enemyListPtr = OBJECTMANAGER->GetObjectListPtr(ObjectLayer::ENEMY);
		if((*enemyListPtr).size())
		{
			RECT temp;

			for (int i = 0; i < (*enemyListPtr).size(); ++i)
			{
				RECT enemyRc = ((*enemyListPtr)[i])->GetRect();
				if (IntersectRect(&temp, &mRangeBox, &enemyRc))
				{
					((Enemy*)(*enemyListPtr)[i])->DamagedHp();
					return;
				}
			}
		}

		if ((mForm == Form::Chikorita) && ((*mMapObjectListPtr)[mNextTileY][mNextTileX]->GetMapObjectType() == ItemType::Green))
		{
			COLLISIONMANAGER->MapObjectCollision(&mRect, mMapObjectListPtr, mNextTileX, mNextTileY); // 일부러 사정거리 늘려놓음, 마음에 안 들면 mRect를 mColBox로 바꾸고 확인하기
			mIsAttackingTemp = true;
		}
		else if ((mForm == Form::Totodile) && ((*mMapObjectListPtr)[mNextTileY][mNextTileX]->GetMapObjectType() == ItemType::Blue))
		{
			COLLISIONMANAGER->MapObjectCollision(&mRect, mMapObjectListPtr, mNextTileX, mNextTileY); // 일부러 사정거리 늘려놓음, 마음에 안 들면 mRect를 mColBox로 바꾸고 확인하기
			mIsAttackingTemp = true;
		}
		else if ((mForm == Form::Charmander) && ((*mMapObjectListPtr)[mNextTileY][mNextTileX]->GetMapObjectType() == ItemType::Red))
		{
			COLLISIONMANAGER->MapObjectCollision(&mRect, mMapObjectListPtr, mNextTileX, mNextTileY); // 일부러 사정거리 늘려놓음, 마음에 안 들면 mRect를 mColBox로 바꾸고 확인하기
			mIsAttackingTemp = true;
		}
		else
		{
			mIsAttackingTemp = false;
		}
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

void Player::InputZKey()
{
	if (INPUT->GetKeyDown('Z'))
	{
		//int bagSize = mBag->GetBagItemSize();
		int bagSize = mInven->GetBagItemSize();

		if (!bagSize || (mInven->GetBagItemName() == ItemName::ItemTrail))//mBag->GetBagItemName() == ItemName::ItemTrail))
		{
			RECT temp;
			vector<GameObject*> trainList = OBJECTMANAGER->GetObjectList(ObjectLayer::TRAIN);

			for (int i = 0; i < trainList.size(); ++i)
			{
				if (trainList[i]->GetName() != "Abra")
					continue; // abra 아니면 continue하게 해야 함

				RECT trainRc = trainList[i]->GetRect(); // 추후에 렉트 종류 새로 따면 바꾸기

				if (IntersectRect(&temp, &mRangeBox, &trainRc))
				{
					int trailCnt = ((Abra*)trainList[i])->GetTrailCount();
					if (trailCnt <= 0)
						continue;

					ItemType itemType = ((Abra*)trainList[i])->TrailErase();

					BagItem* bagItem = new BagItem();
					bagItem->Init(ItemName::ItemTrail, itemType);
					mBagItemListPtr->push_back(bagItem);
					return;
				}
			}

			vector<vector<Trail*>>* trailListPtr = mTrailManager->GetTrailListPtr();
			Trail* currentTrail = (*trailListPtr)[mTileY][mTileX];

			if ((currentTrail->GetIsTail()) || ((currentTrail->GetTrailType() != ItemType::None) && (!currentTrail->GetIsConnected())))
			{
				BagItem* bagItem = new BagItem();
				//bagItem->SetName(ItemName::ItemTrail);

				if (currentTrail->GetTrailType() == ItemType::Green)
					bagItem->Init(ItemName::ItemTrail, ItemType::Green);
					//bagItem->SetType(ItemType::Green);
				else if (currentTrail->GetTrailType() == ItemType::Blue)
					bagItem->Init(ItemName::ItemTrail, ItemType::Blue);
					//bagItem->SetType(ItemType::Blue);
				else if (currentTrail->GetTrailType() == ItemType::Red)
					bagItem->Init(ItemName::ItemTrail, ItemType::Red);
					//bagItem->SetType(ItemType::Red);

				mBagItemListPtr->push_back(bagItem);
				//mBagItemList.push_back(bagItem);

				mTrailManager->PickUpTrail(mTileY, mTileX);

				return;
			}
		}

		if (!bagSize || (mInven->GetBagItemName() == ItemName::ItemOre))//mBag->GetBagItemName() == ItemName::ItemOre))
		{
			RECT currentRc = (*mTileListPtr)[mTileY][mTileX]->GetRect();
			vector<GameObject*>* itemListPtr = OBJECTMANAGER->GetObjectListPtr(ObjectLayer::ITEM);

			for (int i = 0; i < (*itemListPtr).size(); ++i)
			{
				Ore* item = (Ore*)((*itemListPtr)[i]);
				POINT pt = { (LONG)(item->GetX()), (LONG)(item->GetY()) };

				if (PtInRect(&currentRc, pt))
				{
					int count = item->GetCount();

					for (int i = 0; i < count; ++i)
					{
						BagItem* bagItem = new BagItem();
						//bagItem->SetName(ItemName::ItemOre);

						if (item->GetOreType() == ItemType::Green)
							bagItem->Init(ItemName::ItemOre, ItemType::Green);
							//bagItem->SetType(ItemType::Green);
						else if (item->GetOreType() == ItemType::Blue)
							bagItem->Init(ItemName::ItemOre, ItemType::Blue);
							//bagItem->SetType(ItemType::Blue);
						else if (item->GetOreType() == ItemType::Red)
							bagItem->Init(ItemName::ItemOre, ItemType::Red);
							//bagItem->SetType(ItemType::Red);

						mBagItemListPtr->push_back(bagItem);
						//mBagItemList.push_back(bagItem);
					}

					(*itemListPtr).erase((*itemListPtr).begin() + i);

					mIsGettingItemThisFrame = true;
				}
				else
					continue;
			}
		}
	}
}

void Player::InputXKey()
{
	if (INPUT->GetKeyDown('X'))
	{
		//int bagSize = mBag->GetBagItemSize();
		if (!mInven->GetBagItemSize())//mBag->GetBagItemSize())
			return;

		ItemName itemName = mInven->GetBagItemName();//mBag->GetBagItemName();

		if (itemName == ItemName::ItemOre)
		{
			RECT temp;

			ItemType itemType = mInven->GetBagItemType(mInven->GetBagItemSize() - 1);//mBag->GetBagItemType(mBag->GetBagItemSize() - 1);
			//ItemType itemType = mBagItemList[mBagItemList.size() - 1]->GetType();
			vector<GameObject*> trainList = OBJECTMANAGER->GetObjectList(ObjectLayer::TRAIN);

			for (int i = 0; i < trainList.size(); ++i)
			{
				if (trainList[i]->GetName() != "Machop")
					continue; // machop 아니면 continue하게 해야 함

				RECT trainRc = trainList[i]->GetRect(); // 추후에 렉트 종류 새로 따면 바꾸기

				if (IntersectRect(&temp, &mRangeBox, &trainRc))
				{
					int oreCnt = ((Machop*)trainList[i])->GetOreCount();
					if (oreCnt > 6)
						continue;

					((Machop*)trainList[i])->InterceptOre(itemType);
					mBagItemListPtr->erase(mBagItemListPtr->begin() + mBagItemListPtr->size() - 1);
					//mBagItemList.erase(mBagItemList.begin() + mBagItemList.size() - 1);
					return;
				}
			}

			Tile* currentTile = (*mTileListPtr)[mTileY][mTileX];
			TileType currentTileType = currentTile->GetTileType();

			if (currentTileType == TileType::Water || currentTileType == TileType::Lava || currentTileType == TileType::ice)
				return;

			RECT currentRc = currentTile->GetRect();
			vector<GameObject*>* itemListPtr = OBJECTMANAGER->GetObjectListPtr(ObjectLayer::ITEM);

			for (int i = 0; i < (*itemListPtr).size(); ++i)
			{
				GameObject* item = (*itemListPtr)[i];
				POINT pt = { (LONG)(item->GetX()), (LONG)(item->GetY()) };

				if (PtInRect(&currentRc, pt))
				{
					if (((Ore*)item)->GetOreType() == itemType)
					{
						((Ore*)item)->PlusCount();
						mBagItemListPtr->erase(mBagItemListPtr->begin() + mBagItemListPtr->size() - 1);
						//mBagItemList.erase(mBagItemList.begin() + mBagItemList.size() - 1);
						return;
					}
					else
						return;
				}
			}

			vector<vector<Trail*>>* trailListPtr = mTrailManager->GetTrailListPtr();
			Trail* currentTrail = (Trail*)(*trailListPtr)[mTileY][mTileX];

			if (currentTrail->GetTrailType() != ItemType::None)
				return;

			Ore* ore = new Ore();
			ore->Drop(TileSize * mTileX, TileSize * mTileY, itemType);
			OBJECTMANAGER->AddObject(ObjectLayer::ITEM, ore);
			mBagItemListPtr->erase(mBagItemListPtr->begin() + mBagItemListPtr->size() - 1);
			//mBagItemList.erase(mBagItemList.begin() + mBagItemList.size() - 1);
		}
		else if (itemName == ItemName::ItemTrail)
		{
			Tile* currentTile = (*mTileListPtr)[mTileY][mTileX];
			TileType currentTileType = currentTile->GetTileType();
			ItemType itemType = mInven->GetBagItemType(mInven->GetBagItemSize() - 1);//mBag->GetBagItemType(mBag->GetBagItemSize() - 1);
			//ItemType itemType = mBagItemList[mBagItemList.size() - 1]->GetType();

			if (currentTileType == TileType::ice)
				return;
			else if ((currentTileType == TileType::Normal) && (itemType != ItemType::Green))
				return;
			else if ((currentTileType == TileType::Water) && (itemType != ItemType::Blue))
				return;
			else if ((currentTileType == TileType::Lava) && (itemType != ItemType::Red))
				return;

			RECT currentRc = currentTile->GetRect();
			vector<GameObject*>* itemListPtr = OBJECTMANAGER->GetObjectListPtr(ObjectLayer::ITEM);

			for (int i = 0; i < (*itemListPtr).size(); ++i)
			{
				GameObject* item = (*itemListPtr)[i];
				POINT pt = { (LONG)(item->GetX()), (LONG)(item->GetY()) };

				if (PtInRect(&currentRc, pt))
					return;
			}

			vector<vector<Trail*>>* trailListPtr = mTrailManager->GetTrailListPtr();
			Trail* currentTrail = (*trailListPtr)[mTileY][mTileX];

			if (currentTrail->GetTrailType() != ItemType::None)
				return;

			mTrailManager->PlaceTrail(mTileY, mTileX, itemType, 0); // 0: down 1: up 2: left 3: right
			mBagItemListPtr->erase(mBagItemListPtr->begin() + mBagItemListPtr->size() - 1);
			//mBagItemList.erase(mBagItemList.begin() + mBagItemList.size() - 1);
		}
	}
}

void Player::InputCKey()
{
	if (INPUT->GetKeyDown('C')) // Loop Order: normal -> leaf -> water -> fire -> normal -> ...
	{
		if (mForm == Form::Ditto)
		{
			mIsChangable = true;
			mForm = Form::Chikorita;
			mImage = IMAGEMANAGER->FindImage(L"chikorita");
		}
		else if (mForm == Form::Chikorita)
		{
			mIsChangable = true;
			mForm = Form::Totodile;
			mImage = IMAGEMANAGER->FindImage(L"totodile");
		}
		else if (mForm == Form::Totodile)
		{
			if (CheckTileType(TileType::Water))
			{
				mIsChangable = false;
			}
			else
			{
				mIsChangable = true;
				mForm = Form::Charmander;
				mImage = IMAGEMANAGER->FindImage(L"charmander");
			}
		}
		else if (mForm == Form::Charmander)
		{
			if (CheckTileType(TileType::Lava))
			{
				mIsChangable = false;
			}
			else
			{
				mIsChangable = true;
				mForm = Form::Ditto;
				mImage = IMAGEMANAGER->FindImage(L"ditto");
			}
		}

		// {{ 이 변수 굳이 필요없으면 지우기 // 이펙트 등 사용할 거면 살려두기
		mChangeT = 2.f; 

		//if (mChangeT)
		//{
		//	mChangeT -= TIME->DeltaTime();
		//
		//	if (mChangeT <= 0)
		//		mChangeT = 0.f;
		//}
		// }} 이 변수 굳이 필요없으면 지우기 // 이펙트 등 사용할 거면 살려두기
	}

	if (mChangeT)
	{
		mChangeT -= TIME->DeltaTime();

		if (mChangeT <= 0)
		{
			mChangeT = 0.f;
			mIsChangable = true;
		}
	}
}

void Player::InputVKey()
{
	if (INPUT->GetKeyDown('V'))
	{
		vector<vector<Trail*>>* trailListPtr = mTrailManager->GetTrailListPtr();
		Trail* currentTrail = (*trailListPtr)[mTileY][mTileX];

		if (currentTrail->GetTrailType() == ItemType::None)
			return;

		mTrailManager->TurnTrail(mTileY, mTileX);
	}
}

bool Player::CheckTileType(TileType tileType)
{
	for (int j = mTileY - 1; j <= mTileY + 1; ++j)
	{
		for (int i = mTileX - 1; i <= mTileX + 1; ++i)
		{
			if (i >= 0 && i < TileCountX && j >= 0 && j < TileCountY)
			{
				TileType currentTileType = (*mTileListPtr)[j][i]->GetTileType();

				if (currentTileType == tileType)
				{
					RECT temp;
					RECT tileRc = (*mTileListPtr)[j][i]->GetRect();

					if (IntersectRect(&temp, &mColBox, &tileRc))
						return true;
				}
			}
		}
	}

	return false;
}

void Player::CheckRange()
{
	int dir = 0;
	if (mDir == DirectionEight::LeftDown)
	{
		dir = 1;
	}
	else if (mDir == DirectionEight::RightDown)
	{
		dir = 1;
	}
	else if (mDir == DirectionEight::LeftUp)
	{
		dir = 2;
	}
	else if (mDir == DirectionEight::RightUp)
	{
		dir = 2;
	}


	if (mDir == DirectionEight::Down || dir == 1)
	{
		mRangeX = (int)mX;
		mRangeY = (int)mY + TileSize / 2;
	}
	else if (mDir == DirectionEight::Up || dir == 2)
	{
		mRangeX = (int)mX;
		mRangeY = (int)mY - TileSize / 2;
	}
	else if (mDir == DirectionEight::Left)
	{
		mRangeX = (int)mX - TileSize / 2;
		mRangeY = (int)mY;
	}
	else if (mDir == DirectionEight::Right)
	{
		mRangeX = (int)mX + TileSize / 2;
		mRangeY = (int)mY;
	}

	mRangeBox = RectMakeCenter(mRangeX, mRangeY, TileSize, TileSize);
}

void Player::ChangeCurrentAnimation()
{
	if (mState == PlayerState::Idle)
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
	else if (mState == PlayerState::Move)
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

void Player::InputLKey()
{
	if (INPUT->GetKeyDown('L'))
	{
		if (mIsInfoOn)
			mIsInfoOn = false;
		else
			mIsInfoOn = true;
	}
}

void Player::InputCheatKey()
{
	if (INPUT->GetKeyDown('J'))
	{
		int hidden = mInven->GetHiddenItem();

		if (hidden == 0)
			mInven->SetHiddenItem(1);
		else if (hidden == 1)
			mInven->SetHiddenItem(2);
		else if (hidden == 2)
			mInven->SetHiddenItem(0);
	}
}

void Player::RenderTestText(HDC hdc)
{
	RECT cam = CAMERAMANAGER->GetMainCamera()->GetRect();
	//wstring strInput = L"InputType : " + to_wstring(mInputType);
	//TextOut(hdc, 10, 100, strInput.c_str(), (int)strInput.length());

	wstring strChange = L"포켓몬 변!신! ";
	if (!mIsChangable)
		strChange = L"변신 불가 지역입니다";
	if (mChangeT)
		TextOut(hdc, (int)mX - 20 - cam.left, (int)mY - 25 - cam.top, strChange.c_str(), (int)strChange.length());

	if (mIsInfoOn)
	{
		wstring strTile = L"tile x: " + to_wstring(mTileX) + L", y: " + to_wstring(mTileY);
		TextOut(hdc, (int)mX + 25 - cam.left, (int)mY - cam.top, strTile.c_str(), (int)strTile.length());
	}

	//wstring strAtk = L"공격 중이다";
	//if (mIsAttackingTemp)
	//	TextOut(hdc, (int)mX + 55, (int)mY, strAtk.c_str(), (int)strAtk.length());

	//wstring strInven = L"Inven size: " + to_wstring(mBagItemList.size());
	//TextOut(hdc, (int)mX + 25 - cam.left, (int)mY + 25 - cam.top, strInven.c_str(), (int)strInven.length());

	wstring strObj = L"========== 바닥 오브젝트 ==========";
	TextOut(hdc, 520, 178, strObj.c_str(), (int)strObj.length());
	vector<GameObject*> itemList = OBJECTMANAGER->GetObjectList(ObjectLayer::ITEM);
	for (int i = 0; i < itemList.size(); ++i)
	{
		bool active = itemList[i]->GetIsActive();
		ItemType type = ((Ore*)itemList[i])->GetOreType();
		wstring strType = L"";
		if (type == ItemType::None)
			strType = L"Type: None";
		else if (type == ItemType::Green)
			strType = L"Type: Green";
		else if (type == ItemType::Blue)
			strType = L"Type: Blue";
		else if (type == ItemType::Red)
			strType = L"Type: Red";
		wstring strActive = L"";
		if (active)
			strActive = to_wstring(i) + L": active, " + strType;
		else
			strActive = to_wstring(i) + L": nonActive, " + strType;

		TextOut(hdc, 550, (int)200 + 22 * i, strActive.c_str(), (int)strActive.length());
	}

	wstring strInv = L"========== 인벤 ==========";
	TextOut(hdc, 520, 350, strInv.c_str(), (int)strInv.length());
	for (int i = 0; i < mBagItemListPtr->size(); ++i)
	{
		ItemName name = mInven->GetBagItemName();//mBag->GetBagItemName();// mBagItemList[i]->GetName();
		ItemType type = mInven->GetBagItemType(i);//mBag->GetBagItemType(i);// ItemList[i]->GetType();

		wstring strName = L"";
		if (name == ItemName::ItemOre)
			strName = L"Ore, ";
		else if (name == ItemName::ItemTrail)
			strName = L"Trail, ";

		wstring strType = L"";
		if (type == ItemType::None)
			strType = L"Type: None";
		else if (type == ItemType::Green)
			strType = L"Type: Green";
		else if (type == ItemType::Blue)
			strType = L"Type: Blue";
		else if (type == ItemType::Red)
			strType = L"Type: Red";
		wstring strActive = to_wstring(i) + L": " + strName + strType;

		TextOut(hdc, 550, (int)372 + 22 * i, strActive.c_str(), (int)strActive.length());
	}
}