#include "pch.h"
#include "TrailManager.h"
#include "Trail.h"

void TrailManager::Init()
{
	//빈트레일 인잇
	for (int y = 0; y < TileCountY; ++y)
	{
		mTrailList.push_back(vector <Trail*>());
		for (int x = 0; x < TileCountX; ++x)
		{
			//벡터
			Trail* trail = new Trail();
			trail->Init(TileSize * x, TileSize * y, 0, 0);
			mTrailList[y].push_back(trail);

		}
	}
}

void TrailManager::Release()
{
	//타일 릴리즈
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
void TrailManager::InsertTrail(int x, int y, int type, int dir)
{
	//해당인덱스의 속성
	mTrailList[y][x]->SetTrailType(type);
	mTrailList[y][x]->SetDirection(dir);

	//바꿔야함
	mTrailList[y][x]->SetIsConnected(true);
}




//끝에 있는 기차길 돌리기	//트레일이 보는 방향에 타일이있나, 이미 커넥티드된 타일이 아니면 그타일을 커넥티드로
void TrailManager::TurnTrail(int indexY, int indexX)
{
	//mTrailList.back()->Turn();
	//돌릴려는게 꼬리면
	if (mTrailList[indexY][indexX]->GetIsTail())
	{
		//방향바꾸고
		mTrailList[indexY][indexX]->Turn();



		//타일찾기
		//아래를보고있고
		if (mTrailList[indexY][indexX]->GetDirection() == TrailDirection::Down)
		{
			//맨아랫출이 아니고
			if (indexY < TileCountY)
			{
				//아랫줄녀석이
				if (mTrailList[indexY + 1][indexX]->GetTrailType() != TrailType::None)
				{
					//이미 연결된게 아니면
					if (!mTrailList[indexY + 1][indexX]->GetIsConnected())
					{
						//연결시키고 꼬리체인지
						mTrailList[indexY][indexX]->SetIsTail(false);
						mTrailList[indexY + 1][indexX]->SetIsTail(true);
						mTrailList[indexY + 1][indexX]->SetIsConnected(true);
					}
				}

			}
		}
		//위를보고있는데
		if (mTrailList[indexY][indexX]->GetDirection() == TrailDirection::Up)
		{
			//맨윗줄이아니고
			if (indexY > 0)
			{
				//윗줄녀석이
				if (mTrailList[indexY - 1][indexX]->GetTrailType() != TrailType::None)
				{
					//이미 연결된게 아니면
					if (!mTrailList[indexY - 1][indexX]->GetIsConnected())
					{
						//연결시키고 꼬리체인지
						mTrailList[indexY][indexX]->SetIsTail(false);
						mTrailList[indexY - 1][indexX]->SetIsTail(true);
						mTrailList[indexY - 1][indexX]->SetIsConnected(true);
					}
				}
			}

		}
		//왼쪽을보고있는데
		if (mTrailList[indexY][indexX]->GetDirection() == TrailDirection::Left)
		{
			//맨왼쪽줄이아니고
			if (indexX > 0)
			{
				//왼쪽녀석이
				if (mTrailList[indexY][indexX - 1]->GetTrailType() != TrailType::None)
				{
					//이미 연결된게 아니면
					if (!mTrailList[indexY][indexX - 1]->GetIsConnected())
					{
						//연결시키고 꼬리체인지
						mTrailList[indexY][indexX]->SetIsTail(false);
						mTrailList[indexY][indexX - 1]->SetIsTail(true);
						mTrailList[indexY][indexX - 1]->SetIsConnected(true);
					}
				}
			}

		}
		//오른쪽을보고있는데
		if (mTrailList[indexY][indexX]->GetDirection() == TrailDirection::Right)
		{
			//맨오른쪽줄이아니고
			if (indexX > TileCountX)
			{
				//오른쪽녀석이
				if (mTrailList[indexY][indexX + 1]->GetTrailType() != TrailType::None)
				{
					//이미 연결된게 아니면
					if (!mTrailList[indexY][indexX + 1]->GetIsConnected())
					{
						//연결시키고 꼬리체인지
						mTrailList[indexY][indexX]->SetIsTail(false);
						mTrailList[indexY][indexX + 1]->SetIsTail(true);
						mTrailList[indexY][indexX + 1]->SetIsConnected(true);
					}
				}
			}

		}
	}
}

//끄트머리의 기차길을 줍기
void TrailManager::PickUpTrail(int indexY, int indexX)
{
	//mTrailList.pop_back();
	//건드린게 꼬리면
	if (mTrailList[indexY][indexX]->GetIsTail())
	{
		mTrailList[indexY][indexX]->SetTrailType(0);
		mTrailList[indexY][indexX]->SetIsActive(false);
		mTrailList[indexY][indexX]->SetIsTail(false);

		//뺀녀셕의주변 체크, 꼬리설정하기
		if (indexY > 0)
		{
			//윗놈이 연결된놈이고
			if (mTrailList[indexY - 1][indexX]->GetIsConnected())
			{
				//아래를 보고있으면
				if (mTrailList[indexY - 1][indexX]->GetDirection() == TrailDirection::Down)
				{
					//그놈이 꼬리
					mTrailList[indexY - 1][indexX]->SetIsTail(true);
				}
			}

		}
		if (indexY < TileCountY)
		{
			//아랫놈이 연결된놈이고
			if (mTrailList[indexY + 1][indexX]->GetIsConnected())
			{
				//위를 보고있으면
				if (mTrailList[indexY + 1][indexX]->GetDirection() == TrailDirection::Up)
				{
					//그놈이 꼬리
					mTrailList[indexY][indexX]->SetIsTail(true);
				}
			}
		}
		if (indexX > 0)
		{
			//왼쪽놈이 연결된놈이고
			if (mTrailList[indexY][indexX - 1]->GetIsConnected())
			{
				//오른쪽을 보고있으면
				if (mTrailList[indexY][indexX - 1]->GetDirection() == TrailDirection::Right)
				{
					//그놈이 꼬리
					mTrailList[indexY][indexX]->SetIsTail(true);
				}
			}
		}
		if (indexX < TileCountX)
		{
			//오른쪽놈이 연결된놈이고
			if (mTrailList[indexY][indexX + 1]->GetIsConnected())
			{
				//왼쪽을 보고있으면
				if (mTrailList[indexY][indexX + 1]->GetDirection() == TrailDirection::Left)
				{
					//그놈이 꼬리
					mTrailList[indexY][indexX]->SetIsTail(true);
				}
			}
		}
	}
}

//플레이어가 트레일 설치
void TrailManager::PlaceTrail(int x, int y, int type, int dir)
{
	mTrailList[y][x]->SetTrailType(type);
	mTrailList[y][x]->SetDirection(dir);
	mTrailList[y][x]->SetIsActive(true);

	//설치한녀석의 연결체크
	if (y > 0)
	{
		//윗놈이 연결된놈이고
		if (mTrailList[y - 1][x]->GetIsConnected())
		{
			//아래를 보고있으면
			if (mTrailList[y - 1][x]->GetDirection() == TrailDirection::Down)
			{
				//설치한놈은 연결된놈
				mTrailList[y][x]->SetIsConnected(true);

				//꼬리교체
				mTrailList[y - 1][x]->SetIsTail(false);
				mTrailList[y][x]->SetIsTail(true);
			}
		}

	}
	if (y < TileCountY)
	{
		//아랫놈이 연결된놈이고
		if (mTrailList[y + 1][x]->GetIsConnected())
		{
			//위를 보고있으면
			if (mTrailList[y + 1][x]->GetDirection() == TrailDirection::Up)
			{
				//설치한놈은 연결된놈
				mTrailList[y][x]->SetIsConnected(true);

				//꼬리교체
				mTrailList[y + 1][x]->SetIsTail(false);
				mTrailList[y][x]->SetIsTail(true);
			}
		}
	}
	if (x > 0)
	{
		//왼쪽놈이 연결된놈이고
		if (mTrailList[y][x - 1]->GetIsConnected())
		{
			//오른쪽을 보고있으면
			if (mTrailList[y][x - 1]->GetDirection() == TrailDirection::Right)
			{
				//설치한놈은 연결된놈
				mTrailList[y][x]->SetIsConnected(true);

				//꼬리교체
				mTrailList[y][x - 1]->SetIsTail(false);
				mTrailList[y][x]->SetIsTail(true);
			}
		}
	}
	if (x < TileCountX)
	{
		//오른쪽놈이 연결된놈이고
		if (mTrailList[y][x + 1]->GetIsConnected())
		{
			//왼쪽을 보고있으면
			if (mTrailList[y][x + 1]->GetDirection() == TrailDirection::Left)
			{
				//설치한놈은 연결된놈
				mTrailList[y][x]->SetIsConnected(true);


				//꼬리교체
				mTrailList[y][x + 1]->SetIsTail(false);
				mTrailList[y][x]->SetIsTail(true);
			}
		}
	}

}

//시작 트레일의 인덱스를 넣어 꼬리를찾자
//void TrailManager::SetTrailTail(int indexY, int indexX)
//{
//	//연결된 트레일이고
//	if (mTrailList[indexY][indexX]->GetIsConnected() && mTrailList[indexY][indexX]->GetTrailType() != TrailType::None)
//	{
//		switch (mTrailList[indexY][indexX]->GetDirection())
//		{
//		case TrailDirection::Down :
//			if(mTrailList[indexY+1][indexX].)
//			break;
//		case TrailDirection::Up:
//			break;
//
//		case TrailDirection::Left:
//			break;
//
//		case TrailDirection::Right:
//			break;
//
//		default:
//			break;
//		}
//	}
//}