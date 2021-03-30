#include "pch.h"
//#include "Trail.h"
#include "TrailManager.h"
#include "Animation.h"


void TrailManager::Init()
{
	YTileCount = TileCountY;
	XTileCount = TileCountX;
	//빈트레일 인니셜라이즈
	AnimationInit();
	for (int y = 0; y < TileCountY; ++y)
	{
		mTrailList.push_back(vector <Trail*>());
		for (int x = 0; x < TileCountX; ++x)
		{
			//벡터
			Trail* trail = new Trail();
			trail->Init(TileSize * x, TileSize * y, 0, 0);
			trail->AnimationInit(mDown, mUp, mLeft, mRight);
			mTrailList[y].push_back(trail);

		}
	}

}

void TrailManager::Init(int y, int x)
{
	YTileCount = y;
	XTileCount = x;
	//빈트레일 이니셜라이즈
	AnimationInit();

	for (int y = 0; y < YTileCount; ++y)
	{
		mTrailList.push_back(vector <Trail*>());
		for (int x = 0; x < XTileCount; ++x)
		{
			//벡터
			Trail* trail = new Trail();
			trail->Init(TileSize * x, TileSize * y, 0, 0);
			trail->AnimationInit(mDown, mUp, mLeft, mRight);
			mTrailList[y].push_back(trail);
		}
	}
	mOrder = 0;
}

void TrailManager::Release()
{
	//트레일 릴리즈
	for (int y = 0; y < mTrailList.size(); ++y)
	{
		for (int x = 0; x < mTrailList[y].size(); ++x)
		{
			SafeDelete(mTrailList[y][x]);
			mTrailList[y].erase(mTrailList[y].begin() + x);
			x--;
		}
		if (mTrailList[y].size() <= 0)
		{
			mTrailList.erase(mTrailList.begin() + y);
			y--;
		}
	}
}

void TrailManager::Update()
{
	for (int y = 0; y < mTrailList.size(); ++y)
	{
		for (int x = 0; x < mTrailList[y].size(); ++x)
		{
			mTrailList[y][x]->Update();
		}
	}
	mUp->Update();
	mDown->Update();
	mLeft->Update();
	mRight->Update();
}

void TrailManager::Render(HDC hdc)
{
	for (int y = 0; y < mTrailList.size(); ++y)
	{
		for (int x = 0; x < mTrailList[y].size(); ++x)
		{
			mTrailList[y][x]->Render(hdc);
		}
	}
}

//씬에서 인잇할때
void TrailManager::InsertTrail(int indexY, int indexX, ItemType type, int dir)
{
	//해당인덱스의 속성
	mTrailList[indexY][indexX]->SetTrailType(type);
	mTrailList[indexY][indexX]->SetDirection(dir);

	//바꿔야함
	mTrailList[indexY][indexX]->SetIsConnected(true);
}

void TrailManager::TurnTrail(int indexY, int indexX)
{
	if (mTrailList[indexY][indexX]->GetTrailType() == ItemType::None)
		return;
	if (mTrailList[indexY][indexX]->GetIsTail() == false && mTrailList[indexY][indexX]->GetIsConnected() == true)
		return;

	POINT targetIndex;

	mTrailList[indexY][indexX]->Turn();

	if (GetNextTrailIndex(indexX, indexY, &targetIndex))
	{
	
		//아래에 연결된 트레일이있거나
		if (mTrailList[targetIndex.y][targetIndex.x]->GetIsConnected())
		{
			//무한루프를 만드는게 아니면
			if(!CheckIsLoop(indexX,indexY))
				//다시돌려
				TurnTrail(indexY, indexX);
		}
		else
		{
			//없으면 꼬리설정
			SetTrailTail(mStartY, mStartX);
			return;
		}
	}
}

//끄트머리의 기차길을 줍기
ItemType TrailManager::PickUpTrail(int indexY, int indexX)
{
	//건드린게 꼬리면
	if (mTrailList[indexY][indexX]->GetIsTail())
	{
		ItemType type = mTrailList[indexY][indexX]->GetTrailType();
		mTrailList[indexY][indexX]->SetTrailType(ItemType::None);
		mTrailList[indexY][indexX]->SetIsActive(false);
		mTrailList[indexY][indexX]->SetIsTail(false);
		mTrailList[indexY][indexX]->SetIsConnected(false);

		SetTrailTail(mStartY,mStartX);

		return type;
	}
	//꼬리가아니면
	else
	{
		// {{ 원래 남훈이가 했던 부분
			//트레일타입 None을 리턴
			//return ItemType::None;
		// 원래 남훈이가 했던 부분 }}
		
		// 남훈아 확인하고 위에 지워줘 // tail이 아니라서 아래에서 settrailtail은 호출하지 않음

		ItemType type = mTrailList[indexY][indexX]->GetTrailType();
		mTrailList[indexY][indexX]->SetTrailType(ItemType::None);
		mTrailList[indexY][indexX]->SetIsActive(false);
		mTrailList[indexY][indexX]->SetIsTail(false);
		mTrailList[indexY][indexX]->SetIsConnected(false);

		return type;
	}
}

//플레이어가 트레일 설치
bool TrailManager::PlaceTrail(int indexY, int indexX, ItemType type, int dir)
{
	//이미설치된자리면
	if (mTrailList[indexY][indexX]->GetTrailType() != ItemType::None)
		return false;

	mTrailList[indexY][indexX]->SetTrailType(type);
	mTrailList[indexY][indexX]->SetDirection(dir);
	mTrailList[indexY][indexX]->SetIsActive(true);
	SetTrailTail(mStartY,mStartX);
	return true;

}

//무한루프에 갇혔나?
bool TrailManager::CheckIsLoop(int indexX, int indexY)
{
	//벽에닿으면 루프아님
	if (indexX < 0 || indexX >= XTileCount)return false;
	if (indexY < 0 || indexY >= YTileCount)return false;
	if (mTrailList[indexY][indexX] == nullptr)return false;
	
	POINT nextIndex;
	//다음트레일이없으면 루프아님
	if (GetNextTrailIndex(indexX, indexY,&nextIndex) == false)return false;

	//다음트레일이 연결된트레일이면 루프야!
	if (mTrailList[nextIndex.y][nextIndex.x]->GetIsConnected())
		return true;

	//그다음녀석확인
	return CheckIsLoop(nextIndex.x, nextIndex.y);

}

bool TrailManager::GetNextTrailIndex(int indexX, int indexY, POINT* pOutput)
{
	//벽에닿으면 false;
	if (indexX < 0 || indexX >= XTileCount)return false;
	if (indexY < 0 || indexY >= YTileCount)return false;
	if (mTrailList[indexY][indexX]== nullptr)return false;

	Trail* trail = mTrailList[indexY][indexX];

	int targetIndexX = indexX;
	int targetIndexY = indexY;
	//방향에따라 목표인덱스설정
	switch (trail->GetDirection())
	{
	case TrailDirection::Down:
		targetIndexY++;
		break;
	case TrailDirection::Up:
		targetIndexY--;
		break;
	case TrailDirection::Right:
		targetIndexX++;
		break;
	case TrailDirection::Left:
		targetIndexX--;
		break;
	}

	if (targetIndexX < 0 || targetIndexX >= XTileCount)return false;
	if (targetIndexY < 0 || targetIndexY >= YTileCount)return false;
	if (pOutput != nullptr)
	{
		pOutput->x = targetIndexX;
		pOutput->y = targetIndexY;
	}

	return true;
}
void TrailManager::SetTrailTail(int indexY, int indexX)
{
	if (indexX < 0 || indexX >= XTileCount)return;
	if (indexY < 0 || indexY >= YTileCount)return;
	if (mTrailList[indexY][indexX]->GetTrailType()== ItemType::None)return;
	
	mTrailList[indexY][indexX]->SetOrder(mOrder);
	
	POINT nextIndex;
	//다음트레일찾기
	GetNextTrailIndex(indexX, indexY, &nextIndex);
	//다음트레일이 없으면
	if (mTrailList[nextIndex.y][nextIndex.x]->GetTrailType()== ItemType::None)
	{
		//내가꼬리야
		mTrailList[indexY][indexX]->SetIsConnected(true);
		mTrailList[indexY][indexX]->SetIsTail(true);
		mOrder = 0;
		return;
	}
	//다음트레일이 이미연결되어있고 서순이 어긋나려하면
	else if (mTrailList[nextIndex.y][nextIndex.x]->GetIsConnected() &&
		mTrailList[nextIndex.y][nextIndex.x]->GetOrder() < mTrailList[indexY][indexX]->GetOrder())
	{
		//내가꼬리야
		mTrailList[indexY][indexX]->SetIsConnected(true);
		mTrailList[indexY][indexX]->SetIsTail(true);
		mOrder = 0;
		return;

	}
	//그게아니면
	else
	{
		mTrailList[indexY][indexX]->SetIsConnected(true);
		mTrailList[indexY][indexX]->SetIsTail(false);
		mOrder++;
		SetTrailTail(nextIndex.y, nextIndex.x);
	}

}


void TrailManager::FindTail(int* indexY, int* indexX)
{
	for (int y = 0;y < mTrailList.size();y++)
	{
		for (int x = 0;x < mTrailList.size();x++)
		{
			if (mTrailList[y][x]->GetIsTail() == true)
			{
				*indexY = y;
				*indexX = x;
				break;
			}
		}
	}
}

void TrailManager::AnimationInit()
{
	mDown = new Animation();
	mDown->InitFrameByStartEnd(0, 1, 3, 1, false);
	mDown->SetIsLoop(true);
	mDown->SetFrameUpdateTime(0.2f);
	mDown->Play();

	mUp = new Animation();
	mUp->InitFrameByStartEnd(0, 0, 3, 0, false);
	mUp->SetIsLoop(true);
	mUp->SetFrameUpdateTime(0.2f);
	mUp->Play();

	mLeft = new Animation();
	mLeft->InitFrameByStartEnd(0, 3, 3, 3, false);
	mLeft->SetIsLoop(true);
	mLeft->SetFrameUpdateTime(0.2f);
	mLeft->Play();

	mRight = new Animation();
	mRight->InitFrameByStartEnd(0, 2, 3, 2, false);
	mRight->SetIsLoop(true);
	mRight->SetFrameUpdateTime(0.2f);
	mRight->Play();

}

void TrailManager::SetEndIndex(int y, int x)
{
	mTrailList[y][x]->SetisFinish(true);
}