#include "pch.h"
#include "Player.h"

#include "Image.h"
#include "Camera.h"
#include "Animation.h"
#include "MapObject.h"
#include "TrailManager.h"
#include "Ore.h"

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

	mIsAttackingTemp = false; // ��ȣ�ۿ� �� �׽�Ʈ ����

	mIsGettingItemThisFrame = false;

	mIsChangable = true;
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

	InputSpaceKey();
	InputZKey();
	InputXKey();
	InputCKey();

	//ChangeForm(); // ���� ������ �Լ�

	ChangeCurrentAnimation();

	// �÷��̾� ������Ʈ �� ��� �߰��Ϸ��� ���⿡ �߰��ϱ�

	mCurrentAnimation->Update();
}

void Player::Render(HDC hdc)
{
#ifdef DEBUG
	RenderRect(hdc, mColBox);
#endif
	CAMERAMANAGER->GetMainCamera()->ScaleFrameRender(hdc, mImage, mRect.left, mRect.top,
		mCurrentAnimation->GetNowFrameX(), mCurrentAnimation->GetNowFrameY(), (int)mSizeX, (int)mSizeY);

	// �׽�Ʈ �ؽ�Ʈ // ������ �� �����ּ���
	RenderTestText(hdc);

	// {{ ���� Ÿ��, ���� Ÿ�� Ȯ�ο� // ������ �� �����ּ��� // ����
	RECT currentRc = (*mTileListPtr)[mTileY][mTileX]->GetRect();
	RECT nextRc = (*mTileListPtr)[mNextTileY][mNextTileX]->GetRect();
	GIZMO->DrawRect(hdc, currentRc, Gizmo::Color::Yellow);
	GIZMO->DrawRect(hdc, nextRc, Gizmo::Color::Cyan);
	GIZMO->DrawRect(hdc, mColBox, Gizmo::Color::Violet);
	// }} ���� Ÿ��, ���� Ÿ�� Ȯ�ο� // ������ �� �����ּ���
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
		mInputType = 2; // ���� 2: up �Է� ó��
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
		mInputType = 1; // ���� 1: down �Է� ó��
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
		mInputType = 4; // ���� 4: right �Է� ó��
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
		mInputType = 3; // ���� 3: left �Է� ó��
		mIsDirectionKeyDown = true;
	}

	if (INPUT->GetKeyAKeyUpB(VK_DOWN, VK_UP))
	{
		mInputType = 1; // ���� 1: down �Է� ó��
		mIsDirectionKeyDown = true;
	}
	else if (INPUT->GetKeyAKeyUpB(VK_DOWN, VK_LEFT))
	{
		mInputType = 1; // ���� 1: down �Է� ó��
		mIsDirectionKeyDown = true;
	}
	else if (INPUT->GetKeyAKeyUpB(VK_DOWN, VK_RIGHT))
	{
		mInputType = 1; // ���� 1: down �Է� ó��
		mIsDirectionKeyDown = true;
	}
	else if (INPUT->GetKeyAKeyUpB(VK_UP, VK_DOWN))
	{
		mInputType = 2; // ���� 2: up �Է� ó��
		mIsDirectionKeyDown = true;
	}
	else if (INPUT->GetKeyAKeyUpB(VK_UP, VK_LEFT))
	{
		mInputType = 2; // ���� 2: up �Է� ó��
		mIsDirectionKeyDown = true;
	}
	else if (INPUT->GetKeyAKeyUpB(VK_UP, VK_RIGHT))
	{
		mInputType = 2; // ���� 2: up �Է� ó��
		mIsDirectionKeyDown = true;
	}
	else if (INPUT->GetKeyAKeyUpB(VK_LEFT, VK_DOWN))
	{
		mInputType = 3; // ���� 3: left �Է� ó��
		mIsDirectionKeyDown = true;
	}
	else if (INPUT->GetKeyAKeyUpB(VK_LEFT, VK_UP))
	{
		mInputType = 3; // ���� 3: left �Է� ó��
		mIsDirectionKeyDown = true;
	}
	else if (INPUT->GetKeyAKeyUpB(VK_LEFT, VK_RIGHT))
	{
		mInputType = 3; // ���� 3: left �Է� ó��
		mIsDirectionKeyDown = true;
	}
	else if (INPUT->GetKeyAKeyUpB(VK_RIGHT, VK_DOWN))
	{
		mInputType = 4; // ���� 4: right �Է� ó��
		mIsDirectionKeyDown = true;
	}
	else if (INPUT->GetKeyAKeyUpB(VK_RIGHT, VK_UP))
	{
		mInputType = 4; // ���� 4: right �Է� ó��
		mIsDirectionKeyDown = true;
	}
	else if (INPUT->GetKeyAKeyUpB(VK_RIGHT, VK_LEFT))
	{
		mInputType = 4; // ���� 4: right �Է� ó��
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

		// mapObject to item
		if ((mForm == Form::Chikorita) && ((*mMapObjectListPtr)[mNextTileY][mNextTileX]->GetMapObjectType() == ItemType::Green))
		{
			COLLISIONMANAGER->MapObjectCollision(&mRect, mMapObjectListPtr, mNextTileX, mNextTileY); // �Ϻη� �����Ÿ� �÷�����, ������ �� ��� mRect�� mColBox�� �ٲٰ� Ȯ���ϱ�
			mIsAttackingTemp = true;
		}
		else if ((mForm == Form::Totodile) && ((*mMapObjectListPtr)[mNextTileY][mNextTileX]->GetMapObjectType() == ItemType::Blue))
		{
			COLLISIONMANAGER->MapObjectCollision(&mRect, mMapObjectListPtr, mNextTileX, mNextTileY); // �Ϻη� �����Ÿ� �÷�����, ������ �� ��� mRect�� mColBox�� �ٲٰ� Ȯ���ϱ�
			mIsAttackingTemp = true;
		}
		else if ((mForm == Form::Charmander) && ((*mMapObjectListPtr)[mNextTileY][mNextTileX]->GetMapObjectType() == ItemType::Red))
		{
			COLLISIONMANAGER->MapObjectCollision(&mRect, mMapObjectListPtr, mNextTileX, mNextTileY); // �Ϻη� �����Ÿ� �÷�����, ������ �� ��� mRect�� mColBox�� �ٲٰ� Ȯ���ϱ�
			mIsAttackingTemp = true;
		}
		else
		{
			mIsAttackingTemp = false;
		}

		// ������(3) �������� �� �տ� 1. ������Ÿ�� 2. �ʿ�����Ʈ(��ĵ����) 3. ������(ĵ����)//���� ������ �ִ� ���̶� �ٸ� �� 4. ������ > �� �� ���� �� ����
		// ������(4) �������� �� �տ� 1. ������Ÿ�� 2. �ʿ�����Ʈ(��ĵ����) 3. ������(ĵ����) 4. ������ 5. ����ִ��� Ÿ�ϰ� �������� �Ӽ��� �� ���� �� > �� �� ���� �� ����

		

		// {{ ȭ��ǥ ������ // ��� Ű �и��ؾ� �� ��.. // �����̽��ٴ� ȭ��ǥ �ݱ�..

		//MapObjectType idontknowwhatitis = (*mMapObjectListPtr)[mNextTileY][mNextTileX]->GetMapObjectType(); // �̰� �׳� �ѰŰ�
//		mTrailManager->TurnTrail(); // �̰ͺ��� ���鼭 ���� �� �ָ鼭 ���ľ� ��

//		if (!((*mMapObjectListPtr)[mNextTileY][mNextTileX]->GetActive())) // �÷��̾ �ٶ󺸴� ���� Ÿ���� nonActive ������ ���� ������ ������ ����
//		{}
		// }} ȭ��ǥ ������

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

void Player::InputZKey()
{
	if (INPUT->GetKeyDown('Z'))
	{
		RECT currentRc = (*mTileListPtr)[mTileY][mTileX]->GetRect();
		vector<GameObject*>* itemListPtr = OBJECTMANAGER->GetObjectListPtr(ObjectLayer::ITEM);
		//float x;
		//float y;

		for (int i = 0; i < (*itemListPtr).size(); ++i)
		{
			Ore* item = (Ore*)((*itemListPtr)[i]);
			//x = (*itemListPtr)[i]->GetX();
			//y = (*itemListPtr)[i]->GetY();
			POINT pt = { item->GetX(), item->GetY() };

			if (PtInRect(&currentRc, pt))
			{
				InvenItem invenItem;
				invenItem.itemName = ItemName::ItemOre;

				if (item->GetOreType() == ItemType::Green)
					invenItem.itemType = ItemTypeP::Green;
				else if (item->GetOreType() == ItemType::Blue)
					invenItem.itemType = ItemTypeP::Blue;
				else if (item->GetOreType() == ItemType::Red)
					invenItem.itemType = ItemTypeP::Red;

				mInvenItemList.push_back(&invenItem);

				(*itemListPtr).erase((*itemListPtr).begin() + i);

				mIsGettingItemThisFrame = true;
			}
			else
				continue;
		}




/*

		// {{ ������(����) -> �κ� ���� ������(UI) // ������ �ݱ�
		GameObject* item = COLLISIONMANAGER->ItemCollision(&mColBox);
		vector<GameObject*>* itemList = OBJECTMANAGER->GetObjectListPtr(ObjectLayer::ITEM);
		//ItemType invenType;

		//if (mItemList.size())
		//	invenType = ((Ore*)mItemList[0])->GetOreType();

		if ((item != nullptr) && (item->GetIsActive()))
		{
			//if ((mForm == Form::Chikorita) && (((Ore*)item)->GetOreType() == ItemType::Green) && ((mItemList.size() == 0) || (invenType == ItemType::Green)))
			if (((Ore*)item)->GetOreType() == ItemType::Green)
			{
				Ore* greenOre = new Ore();
				greenOre = (Ore*)item;
				mItemList.push_back((GameObject*)greenOre);
				item->SetIsActive(false);
				mIsGettingItemThisFrame = true;
			}
			//else if ((mForm == Form::Totodile) && (((Ore*)item)->GetOreType() == ItemType::Blue) && ((mItemList.size() == 0) || (invenType == ItemType::Blue)))
			else if(((Ore*)item)->GetOreType() == ItemType::Blue)
			{
				Ore* blueOre = new Ore();
				blueOre = (Ore*)item;
				mItemList.push_back((GameObject*)blueOre);
				item->SetIsActive(false);
				mIsGettingItemThisFrame = true;
			}
			//else if ((mForm == Form::Charmander) && (((Ore*)item)->GetOreType() == ItemType::Red) && ((mItemList.size() == 0) || (invenType == ItemType::Red)))
			else if (((Ore*)item)->GetOreType() == ItemType::Red)
			{
				Ore* redOre = new Ore();
				redOre = (Ore*)item;
				mItemList.push_back((GameObject*)redOre);
				item->SetIsActive(false);
				mIsGettingItemThisFrame = true;
			}
		}
		// }} ������(����) -> �κ� ���� ������(UI) // ������ �ݱ�*/
	}
}

void Player::InputXKey()
{
	if (INPUT->GetKeyDown('X'))
	{
		if (!mInvenItemList.size())
			return;

		RECT currentRc = (*mTileListPtr)[mTileY][mTileX]->GetRect();
		vector<GameObject*>* itemListPtr = OBJECTMANAGER->GetObjectListPtr(ObjectLayer::ITEM);

		for (int i = 0; i < (*itemListPtr).size(); ++i)
		{
			GameObject* item = (*itemListPtr)[i];
			POINT pt = { item->GetX(), item->GetY() };

			if (PtInRect(&currentRc, pt))
				return;
		}////////////// �� ���� �� �� ���ʿ����� // ���� �ɷ��� ���� �����ϸ� ��




		/*



				InvenItem invenItem;
				invenItem.itemName = ItemName::ItemOre;

				if (item->GetOreType() == ItemType::Green)
					invenItem.itemType = ItemTypeP::Green;
				else if (item->GetOreType() == ItemType::Blue)
					invenItem.itemType = ItemTypeP::Blue;
				else if (item->GetOreType() == ItemType::Red)
					invenItem.itemType = ItemTypeP::Red;

				mInvenItemList.push_back(&invenItem);

				(*itemListPtr).erase((*itemListPtr).begin() + i);

				mIsGettingItemThisFrame = true;
			}
			else
				continue;
		}*/

		/*
		// {{ �κ� ���� ������(UI) -> ������(����) // ������ ������
		if (mItemList.size() && !mIsGettingItemThisFrame) // �������� ������ ��쿡�� ������ ������ ���� // �̹� �����ӿ��� ���� ��찡 �ƴ� ���� ������ ����(�̹� �����ӿ��� ������ �ٷ� ������)
		{
			if (!((*mMapObjectListPtr)[mNextTileY][mNextTileX]->GetActive())) // �ش� ��ġ�� �� ������Ʈ�� nonActive ������ ���� ������ �����ϰ�.
			{
				vector<GameObject*> itemList = OBJECTMANAGER->GetObjectList(ObjectLayer::ITEM);
				vector<GameObject*>::iterator iter = itemList.begin();

				for (; iter != itemList.end(); ++iter)
				{
					float x = (*iter)->GetX();
					float y = (*iter)->GetY();

					int indexX = (int)(x / TileSize);
					int indexY = (int)(y / TileSize);

					if (indexX == mNextTileX && indexY == mNextTileY) // �ش� ��ġ�� ���� �������� �ִ�
					{ // �ƹ��͵� ���Ұǵ�
					}
					else
					{
						Ore* greenOre = new Ore();
						greenOre->Drop(TileSize * mNextTileX, TileSize * mNextTileY, (int)invenType);
						OBJECTMANAGER->AddObject(ObjectLayer::ITEM, greenOre);

						mItemList.erase(mItemList.begin());
					}
				}
			}*/


			/*
			if (!((*mMapObjectListPtr)[mNextTileY][mNextTileX]->GetActive())) // �÷��̾ �ٶ󺸴� ���� Ÿ���� nonActive ������ ���� ������ ������ ����
			{  // �ε�� �Լ� ȣ���� �� ������Ʈ �ִ� ���� ��Ƽ�갡 ����� ������ �ȵ� �� ���� �����ε�!! ������Ʈ �ִ� �� ���� �� ���о��� true�� �� ����...
				if (invenType == ItemType::Green)
				{
					Ore* greenOre = new Ore();
					greenOre->Drop(TileSize * mNextTileX, TileSize * mNextTileY, (int)invenType);
					OBJECTMANAGER->AddObject(ObjectLayer::ITEM, greenOre);


					MapObject* greenObject = new MapObject(
						IMAGEMANAGER->FindImage(L"Ore"),
						TileSize * mNextTileX, TileSize * mNextTileY, TileSize, TileSize,
						0, 1, (int)(MapObjectType::Green)); // ����0 �׸�1 ���2

					(*mMapObjectListPtr)[mNextTileY][mNextTileX] = greenObject;

					//mItemList.push_back((GameObject*)greenOre);
					//item->SetIsAzzctive(false);

					mItemList.erase(mItemList.begin());
				}
			}
		}
		// }} �κ� ���� ������(UI) -> ������(����) // ������ ������*/
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

		// {{ �� ���� ���� �ʿ������ ����� // ����Ʈ �� ����� �Ÿ� ����α�
		mChangeT = 2.f; 

		//if (mChangeT)
		//{
		//	mChangeT -= TIME->DeltaTime();
		//
		//	if (mChangeT <= 0)
		//		mChangeT = 0.f;
		//}
		// }} �� ���� ���� �ʿ������ ����� // ����Ʈ �� ����� �Ÿ� ����α�
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
	RECT cam = CAMERAMANAGER->GetMainCamera()->GetRect();
	//wstring strInput = L"InputType : " + to_wstring(mInputType);
	//TextOut(hdc, 10, 100, strInput.c_str(), (int)strInput.length());

	wstring strChange = L"���ϸ� ��!��! ";
	if (!mIsChangable)
		strChange = L"���� �Ұ� �����Դϴ�";
	if (mChangeT)
		TextOut(hdc, (int)mX - 20 - cam.left, (int)mY - 25 - cam.top, strChange.c_str(), (int)strChange.length());

	//wstring strSpeed = L"SApeed: " + to_wstring(mSpeed);
	//wstring strKeyDown = L"IsKeyDownCheck: " + to_wstring(mIsDirectionKeyDown);
	//TextOut(hdc, (int)mX, (int)mY + 50, strSpeed.c_str(), (int)strSpeed.length());
	//TextOut(hdc, (int)mX, (int)mY + 75, strKeyDown.c_str(), (int)strKeyDown.length());

	wstring strTile = L"tile x: " + to_wstring(mTileX) + L", y: " + to_wstring(mTileY);
	TextOut(hdc, (int)mX + 25 - cam.left, (int)mY - cam.top, strTile.c_str(), (int)strTile.length());

	//wstring strAtk = L"���� ���̴�";
	//if (mIsAttackingTemp)
	//	TextOut(hdc, (int)mX + 55, (int)mY, strAtk.c_str(), (int)strAtk.length());

	wstring strInven = L"Inven size: " + to_wstring(mInvenItemList.size());
	TextOut(hdc, (int)mX + 25 - cam.left, (int)mY + 25 - cam.top, strInven.c_str(), (int)strInven.length());

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

		TextOut(hdc, (int)WINSIZEX * 3 / 4, (int)200 + 25 * i, strActive.c_str(), (int)strActive.length());
	}
}