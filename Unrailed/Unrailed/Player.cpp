#include "pch.h"
#include "Player.h"

#include "Image.h"
#include "Animation.h"
#include "Camera.h"
#include "Tile.h"
#include "MapObject.h"

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

	mSizeX = mImage->GetFrameWidth() * 2;
	mSizeY = mImage->GetFrameHeight() * 2;
	mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);

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
}

void Player::Release()
{
	SafeDeleteAnimation();
}

void Player::Update()
{
	mTileX = (int)(mX / TileSize);
	mTileY = (int)(mY / TileSize);
	// ���� �÷��̾� ������ �÷��ָ鼭 mx, my�� ����� ������ �ʰ� ����, Ÿ���� �����ϸ� ��� �ϰ� �ִ��� �ľ��ϱ�
	// �÷��̾�� �׳� ���� �̹����� ������ ��� �÷��༭ �̻���
	// ���� ũ��δ� �� �߽�, 2�� ũ��δ� ���� ���κ� ����

	InputDirectionKey();

	InputDiggingKey();

	ChangeForm();




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

	mCurrentAnimation->Update();
}

void Player::Render(HDC hdc)
{
	// ī�޶� ���� ������(����)
	/*CameraManager::GetInstance()->GetMainCamera()
		->ScaleFrameRender(hdc, mImage, mRect.left, mRect.top,
			mCurrentAnimation->GetNowFrameX(), mCurrentAnimation->GetNowFrameY(),
			mImage->GetFrameWidth() * 2, mImage->GetFrameHeight() * 2);*/

	//RenderRect(hdc, mRect);

	// �ӽ÷� ī�޶� ������ ������
	mImage->ScaleFrameRender(hdc, mRect.left, mRect.top,
		mCurrentAnimation->GetNowFrameX(), mCurrentAnimation->GetNowFrameY(), mSizeX, mSizeY);

	//mImage->FrameRender(hdc,mRect.left,mRect.top, mCurrentAnimation->GetNowFrameX(),
	//	mCurrentAnimation->GetNowFrameY());

	wstring strInput = L"InputType : " + to_wstring(mInputType);
	TextOut(hdc, 10, 100, strInput.c_str(), (int)strInput.length());

	wstring strChange = L"���ϸ� ��!��! ";
	if (mChangeT)
		TextOut(hdc, mX - 20, mY - 25, strChange.c_str(), (int)strChange.length());

	wstring strSpeed = L"Speed: " + to_wstring(mSpeed);
	wstring strKeyDown = L"IsKeyDownCheck: " + to_wstring(mIsDirectionKeyDown);
	TextOut(hdc, mX, mY + 50, strSpeed.c_str(), (int)strSpeed.length());
	TextOut(hdc, mX, mY + 75, strKeyDown.c_str(), (int)strKeyDown.length());
	wstring strTile = L"Ÿ�� x: " + to_wstring(mTileX) + L", y: " + to_wstring(mTileY);
	TextOut(hdc, mX, mY + 100, strTile.c_str(), (int)strTile.length());




	wstring strAtk = L"���� ���̴�";
	if (mIsAttackingTemp)
		TextOut(hdc, mX + 55, mY, strAtk.c_str(), (int)strAtk.length());
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

void Player::InputDirectionKey()
{
	if (Input::GetInstance()->GetKeyDown(VK_DOWN))// || mInputType == 1)
	{
		mDir = DirectionEight::Down;
		mIsDirectionKeyDown = true;
	}
	else if (Input::GetInstance()->GetKeyDown(VK_UP))// || mInputType == 2)
	{
		mDir = DirectionEight::Up;
		mIsDirectionKeyDown = true;
	}
	else if (Input::GetInstance()->GetKeyDown(VK_LEFT))// || mInputType == 3)
	{
		mDir = DirectionEight::Left;
		mIsDirectionKeyDown = true;
	}
	else if (Input::GetInstance()->GetKeyDown(VK_RIGHT))// || mInputType == 4)
	{
		mDir = DirectionEight::Right;
		mIsDirectionKeyDown = true;
	}

	if (Input::GetInstance()->GetKeyAKeyDownB(VK_DOWN, VK_UP))
	{
		mInputType = 2; // ���� 2: w �Է� ó��
		mIsDirectionKeyDown = true;
	}
	else if (Input::GetInstance()->GetKeyAKeyDownB(VK_DOWN, VK_LEFT))
	{
		mDir = DirectionEight::LeftDown;
		mIsDirectionKeyDown = true;
	}
	else if (Input::GetInstance()->GetKeyAKeyDownB(VK_DOWN, VK_RIGHT))
	{
		mDir = DirectionEight::RightDown;
		mIsDirectionKeyDown = true;
	}
	else if (Input::GetInstance()->GetKeyAKeyDownB(VK_UP, VK_DOWN))
	{
		mInputType = 1; // ���� 1: s �Է� ó��
		mIsDirectionKeyDown = true;
	}
	else if (Input::GetInstance()->GetKeyAKeyDownB(VK_UP, VK_LEFT))
	{
		mDir = DirectionEight::LeftUp;
		mIsDirectionKeyDown = true;
	}
	else if (Input::GetInstance()->GetKeyAKeyDownB(VK_UP, VK_RIGHT))
	{
		mDir = DirectionEight::RightUp;
		mIsDirectionKeyDown = true;
	}
	else if (Input::GetInstance()->GetKeyAKeyDownB(VK_LEFT, VK_DOWN))
	{
		mDir = DirectionEight::LeftDown;
		mIsDirectionKeyDown = true;
	}
	else if (Input::GetInstance()->GetKeyAKeyDownB(VK_LEFT, VK_UP))
	{
		mDir = DirectionEight::LeftUp;
		mIsDirectionKeyDown = true;
	}
	else if (Input::GetInstance()->GetKeyAKeyDownB(VK_LEFT, VK_RIGHT))
	{
		mInputType = 4; // ���� 4: d �Է� ó��
		mIsDirectionKeyDown = true;
	}
	else if (Input::GetInstance()->GetKeyAKeyDownB(VK_RIGHT, VK_DOWN))
	{
		mDir = DirectionEight::RightDown;
		mIsDirectionKeyDown = true;
	}
	else if (Input::GetInstance()->GetKeyAKeyDownB(VK_RIGHT, VK_UP))
	{
		mDir = DirectionEight::RightUp;
		mIsDirectionKeyDown = true;

	}
	else if (Input::GetInstance()->GetKeyAKeyDownB(VK_RIGHT, VK_LEFT))
	{
		mInputType = 3; // ���� 3: a �Է� ó��
		mIsDirectionKeyDown = true;
	}

	if (Input::GetInstance()->GetKeyAKeyUpB(VK_DOWN, VK_UP))
	{
		mInputType = 1; // ���� 1: s �Է� ó��
		mIsDirectionKeyDown = true;
	}
	else if (Input::GetInstance()->GetKeyAKeyUpB(VK_DOWN, VK_LEFT))
	{
		mInputType = 1; // ���� 1: s �Է� ó��
		mIsDirectionKeyDown = true;
	}
	else if (Input::GetInstance()->GetKeyAKeyUpB(VK_DOWN, VK_RIGHT))
	{
		mInputType = 1; // ���� 1: s �Է� ó��
		mIsDirectionKeyDown = true;
	}
	else if (Input::GetInstance()->GetKeyAKeyUpB(VK_UP, VK_DOWN))
	{
		mInputType = 2; // ���� 2: w �Է� ó��
		mIsDirectionKeyDown = true;
	}
	else if (Input::GetInstance()->GetKeyAKeyUpB(VK_UP, VK_LEFT))
	{
		mInputType = 2; // ���� 2: w �Է� ó��
		mIsDirectionKeyDown = true;
	}
	else if (Input::GetInstance()->GetKeyAKeyUpB(VK_UP, VK_RIGHT))
	{
		mInputType = 2; // ���� 2: w �Է� ó��
		mIsDirectionKeyDown = true;
	}
	else if (Input::GetInstance()->GetKeyAKeyUpB(VK_LEFT, VK_DOWN))
	{
		mInputType = 3; // ���� 3: a �Է� ó��
		mIsDirectionKeyDown = true;
	}
	else if (Input::GetInstance()->GetKeyAKeyUpB(VK_LEFT, VK_UP))
	{
		mInputType = 3; // ���� 3: a �Է� ó��
		mIsDirectionKeyDown = true;
	}
	else if (Input::GetInstance()->GetKeyAKeyUpB(VK_LEFT, VK_RIGHT))
	{
		mInputType = 3; // ���� 3: a �Է� ó��
		mIsDirectionKeyDown = true;
	}
	else if (Input::GetInstance()->GetKeyAKeyUpB(VK_RIGHT, VK_DOWN))
	{
		mInputType = 4; // ���� 4: d �Է� ó��
		mIsDirectionKeyDown = true;
	}
	else if (Input::GetInstance()->GetKeyAKeyUpB(VK_RIGHT, VK_UP))
	{
		mInputType = 4; // ���� 4: d �Է� ó��
		mIsDirectionKeyDown = true;
	}
	else if (Input::GetInstance()->GetKeyAKeyUpB(VK_RIGHT, VK_LEFT))
	{
		mInputType = 4; // ���� 4: d �Է� ó��
		mIsDirectionKeyDown = true;
	}
	else if (Input::GetInstance()->GetKeyUp(VK_DOWN))
	{
		mState = State::Idle;
		mInputType = 0;
		mIsDirectionKeyDown = false;
	}
	else if (Input::GetInstance()->GetKeyUp(VK_UP))
	{
		mState = State::Idle;
		mInputType = 0;
		mIsDirectionKeyDown = false;
	}
	else if (Input::GetInstance()->GetKeyUp(VK_LEFT))
	{
		mState = State::Idle;
		mInputType = 0;
		mIsDirectionKeyDown = false;
	}
	else if (Input::GetInstance()->GetKeyUp(VK_RIGHT))
	{
		mState = State::Idle;
		mInputType = 0;
		mIsDirectionKeyDown = false;
	}

	//if (Input::GetInstance()->GetKeyDown(VK_DOWN) || 
	if (mInputType == 1)
	{
		mDir = DirectionEight::Down;
		mInputType = 0;
	}
	//else if (Input::GetInstance()->GetKeyDown(VK_UP) || 
	else if (mInputType == 2)
	{
		mDir = DirectionEight::Up;
		mInputType = 0;
	}
	//else if (Input::GetInstance()->GetKeyDown(VK_LEFT) || 
	else if (mInputType == 3)
	{
		mDir = DirectionEight::Left;
		mInputType = 0;
	}
	//else if (Input::GetInstance()->GetKeyDown(VK_RIGHT) || 
	else if (mInputType == 4)
	{
		mDir = DirectionEight::Right;
		mInputType = 0;
	}

	if (mIsDirectionKeyDown)
	{
		if (mSpeed == 100.f && Input::GetInstance()->GetKeyDown(VK_LSHIFT))
		{
			mSpeed = 300.f;
		}

		if (mSpeed != 100.f)
		{
			//mSpeed = Math::Lerp(100.f, mSpeed, 0.95f);
			mSpeed -= 1.f;

			if (mSpeed <= 275.f)
			{
				mSpeed = 100.f;
				//mIsDirectionKeyDown = false;
			}
		}
	}


	/*
	mTileX = (int)(mX / TileSize);
	mTileY = (int)(mY / TileSize);*/

	if (Input::GetInstance()->GetKey(VK_DOWN))
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
		else if ((mY >= TileSize * mTileY + TileSize / 2) && ((*mTileListPtr)[mNextTileY][mNextTileX]->GetTileType() == TileType::Wall))
			mY = TileSize * mTileY + TileSize / 2;
		else if ((mY >= TileSize * mTileY + TileSize / 2) && (mX < TileSize * mTileX + TileSize / 2) && ((*mTileListPtr)[mNextTileY][mNextTileX - 1]->GetTileType() == TileType::Wall))
			mY = TileSize * mTileY + TileSize / 2;
		else if ((mY >= TileSize * mTileY + TileSize / 2) && (mX > TileSize * mTileX + TileSize / 2) && ((*mTileListPtr)[mNextTileY][mNextTileX + 1]->GetTileType() == TileType::Wall))
			mY = TileSize * mTileY + TileSize / 2;
		else
			mY += mSpeed * Time::GetInstance()->DeltaTime();
	}
	if (Input::GetInstance()->GetKey(VK_UP))
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
		else if ((mY <= TileSize * mTileY + TileSize / 2) && ((*mTileListPtr)[mNextTileY][mNextTileX]->GetTileType() == TileType::Wall))
			mY = TileSize * mTileY + TileSize / 2;
		else if ((mY <= TileSize * mTileY + TileSize / 2) && (mX < TileSize * mTileX + TileSize / 2) && ((*mTileListPtr)[mNextTileY][mNextTileX - 1]->GetTileType() == TileType::Wall))
			mY = TileSize * mTileY + TileSize / 2;
		else if ((mY <= TileSize * mTileY + TileSize / 2) && (mX > TileSize * mTileX + TileSize / 2) && ((*mTileListPtr)[mNextTileY][mNextTileX + 1]->GetTileType() == TileType::Wall))
			mY = TileSize * mTileY + TileSize / 2;
		else
			mY -= mSpeed * Time::GetInstance()->DeltaTime();
	}
	if (Input::GetInstance()->GetKey(VK_LEFT))
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
		else if ((mX <= TileSize * mTileX + TileSize / 2) && ((*mTileListPtr)[mNextTileY][mNextTileX]->GetTileType() == TileType::Wall))
			mX = TileSize * mTileX + TileSize / 2;
		else if((mX <= TileSize * mTileX + TileSize / 2) && (mY < TileSize * mTileY + TileSize / 2) && ((*mTileListPtr)[mNextTileY - 1][mNextTileX]->GetTileType() == TileType::Wall))
			mX = TileSize * mTileX + TileSize / 2;
		else if ((mX <= TileSize * mTileX + TileSize / 2) && (mY > TileSize * mTileY + TileSize / 2) && ((*mTileListPtr)[mNextTileY + 1][mNextTileX]->GetTileType() == TileType::Wall))
			mX = TileSize * mTileX + TileSize / 2;
		else
			mX -= mSpeed * Time::GetInstance()->DeltaTime();
	}
	if (Input::GetInstance()->GetKey(VK_RIGHT))
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
		else if ((mX >= TileSize * mTileX + TileSize / 2) && ((*mTileListPtr)[mNextTileY][mNextTileX]->GetTileType() == TileType::Wall))
			mX = TileSize * mTileX + TileSize / 2;
		else if ((mX >= TileSize * mTileX + TileSize / 2) && (mY < TileSize * mTileY + TileSize / 2) && ((*mTileListPtr)[mNextTileY - 1][mNextTileX]->GetTileType() == TileType::Wall))
		mX = TileSize * mTileX + TileSize / 2;
		else if ((mX >= TileSize * mTileX + TileSize / 2) && (mY > TileSize * mTileY + TileSize / 2) && ((*mTileListPtr)[mNextTileY + 1][mNextTileX]->GetTileType() == TileType::Wall))
		mX = TileSize * mTileX + TileSize / 2;
		else
		mX += mSpeed * Time::GetInstance()->DeltaTime();
	}
	mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);
}

void Player::InputDiggingKey()
{
	if (Input::GetInstance()->GetKey(VK_SPACE))
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
			int width = mX - mTileX * TileSize;
			int height = (mTileY + 1) * TileSize - mY;

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
			int width = (mTileX + 1) * TileSize - mX;
			int height = (mTileY + 1) * TileSize - mY;

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
			int width = mX - mTileX * TileSize;
			int height = mY - mTileY * TileSize;

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
			int width = (mTileX + 1) * TileSize - mX;
			int height = mY - mTileY * TileSize;

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

		if ((mForm == Form::Charmander) && ((*mMapObjectListPtr)[mNextTileY][mNextTileX]->GetMapObjectType() == MapObjectType::Red))
		{
			mIsAttackingTemp = true;
			// ���� ���� > ���� ü�� ���� > ���� ü�� 0�̸� ���� Ÿ�� �ٲٰ� ��Ȱ��ȭ? > ������ UI�� �߰�
		}
		else if ((mForm == Form::Chikorita) && ((*mMapObjectListPtr)[mNextTileY][mNextTileX]->GetMapObjectType() == MapObjectType::Green))
		{
			mIsAttackingTemp = true;
			// ���� ���� > ���� ü�� ���� > ���� ü�� 0�̸� ���� Ÿ�� �ٲٰ� ��Ȱ��ȭ? > ������ UI�� �߰�
		}
		else if ((mForm == Form::Totodile) && ((*mMapObjectListPtr)[mNextTileY][mNextTileX]->GetMapObjectType() == MapObjectType::Blue))
		{
			mIsAttackingTemp = true;
			// ���� ���� > ���� ü�� ���� > ���� ü�� 0�̸� ���� Ÿ�� �ٲٰ� ��Ȱ��ȭ? > ������ UI�� �߰�
		}
		else
			mIsAttackingTemp = false;
	}
	else
		mIsAttackingTemp = false;
}

void Player::ChangeForm()
{
	if (Input::GetInstance()->GetKeyDown('7'))
	{
		mForm = Form::Ditto;
		mImage = IMAGEMANAGER->FindImage(L"ditto");

		mChangeT = 2.f;
	}
	else if (Input::GetInstance()->GetKeyDown('8'))
	{
		mForm = Form::Charmander;
		mImage = IMAGEMANAGER->FindImage(L"charmander");

		mChangeT = 2.f;
	}
	else if (Input::GetInstance()->GetKeyDown('9'))
	{
		mForm = Form::Chikorita;
		mImage = IMAGEMANAGER->FindImage(L"chikorita");

		mChangeT = 2.f;
	}
	else if (Input::GetInstance()->GetKeyDown('0'))
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