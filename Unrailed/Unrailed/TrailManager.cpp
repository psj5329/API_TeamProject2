#include "pch.h"
//#include "Trail.h"
#include "TrailManager.h"


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
void TrailManager::InsertTrail(int indexY, int indexX, int type, int dir)
{
	//해당인덱스의 속성
	mTrailList[indexY][indexX]->SetTrailType(type);
	mTrailList[indexY][indexX]->SetDirection(dir);

	//바꿔야함
	mTrailList[indexY][indexX]->SetIsConnected(true);
}




//끝에 있는 기차길 돌리기	//트레일이 보는 방향에 타일이있나, 이미 커넥티드된 타일이 아니면 그타일을 커넥티드로
void TrailManager::TurnTrail(int indexY, int indexX)
{
	//mTrailList.back()->Turn();
	//돌릴려는게 꼬리면
	if (mTrailList[indexY][indexX]->GetIsTail())
	{
		//일단돌려
		mTrailList[indexY][indexX]->Turn();
		//아래보는데
		if (mTrailList[indexY][indexX]->GetDirection() == TrailDirection::Down)
		{
			if (indexY < TileCountY)
			{
				//아래에 연결된트레일이있다면
				if (mTrailList[indexY + 1][indexX]->GetIsConnected())
				{
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
			else
			{
				//맨아래줄이면 꼬리설정
				SetTrailTail(mStartY, mStartX);
				return;
			}
		}
		//위보는데
		if (mTrailList[indexY][indexX]->GetDirection() == TrailDirection::Down)
		{
			if (indexY > 0)
			{
				//위에 연결된트레일이있다면
				if (mTrailList[indexY - 1][indexX]->GetIsConnected())
				{
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
			else
			{
				//맨위줄이면 꼬리설정
				SetTrailTail(mStartY, mStartX);
				return;
			}
		}
		//오른쪽보는데
		if (mTrailList[indexY][indexX]->GetDirection() == TrailDirection::Right)
		{
			if (indexX < TileCountX)
			{
				//오른쪽에 연결된트레일이있다면
				if (mTrailList[indexY][indexX + 1]->GetIsConnected())
				{
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
			else
			{
				//맨오른쪽줄이면 꼬리설정
				SetTrailTail(mStartY, mStartX);
				return;
			}
		}
		//왼쪽보는데
		if (mTrailList[indexY][indexX]->GetDirection() == TrailDirection::Left)
		{
			if (indexX > 0)
			{
				//왼쪽에 연결된트레일이있다면
				if (mTrailList[indexY][indexX - 1]->GetIsConnected())
				{
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
			else
			{
				//맨왼쪽줄이면 꼬리설정
				SetTrailTail(mStartY, mStartX);
				return;
			}
		}
	}
}

//끄트머리의 기차길을 줍기
TrailType TrailManager::PickUpTrail(int indexY, int indexX)
{
	//mTrailList.pop_back();
	//건드린게 꼬리면
	if (mTrailList[indexY][indexX]->GetIsTail())
	{
		mTrailList[indexY][indexX]->SetTrailType(0);
		mTrailList[indexY][indexX]->SetIsActive(false);
		mTrailList[indexY][indexX]->SetIsTail(false);
		mTrailList[indexY][indexX]->SetIsConnected(false);

		SetTrailTail(mStartY,mStartX);

		return mTrailList[indexY][indexX]->GetTrailType();
	}
	else
	{
		return TrailType::None;
	}
}

//플레이어가 트레일 설치
bool TrailManager::PlaceTrail(int indexY, int indexX, int type, int dir)
{
	//벽처리해야해
	if (mTrailList[indexY - 1][indexX]->GetIsConnected() && mTrailList[indexY+1][indexX]->GetIsConnected() &&
		mTrailList[indexY][indexX -1]->GetIsConnected() && mTrailList[indexY][indexX + 1]->GetIsConnected())
	{
		return false;
	}

	mTrailList[indexY][indexX]->SetTrailType(type);
	mTrailList[indexY][indexX]->SetDirection(dir);
	mTrailList[indexY][indexX]->SetIsActive(true);
	SetTrailTail(mStartY,mStartX);
	return true;

}

//시작 트레일의 인덱스를 넣어 꼬리를설정해주자
void TrailManager::SetTrailTail(int indexY, int indexX)
{
	//빈트레일이 아니고
	if (mTrailList[indexY][indexX]->GetTrailType() != TrailType::None)
	{
		switch (mTrailList[indexY][indexX]->GetDirection())
		{
			////////////아래보고있고
		case TrailDirection::Down:
			if (indexY < TileCountY)
			{
				//아래칸에 트레일이 없거나 이미 연결된 트레일이면 
				if (mTrailList[indexY + 1][indexX]->GetTrailType() == TrailType::None || !mTrailList[indexY + 1][indexX]->GetIsConnected())
				{
					//내가 꼬리다
					mTrailList[indexY][indexX]->SetIsTail(true);
					mTrailList[indexY][indexX]->SetIsConnected(true);
					return;
				}
				else
				{
					//난꼬리가 아냐
					mTrailList[indexY][indexX]->SetIsTail(false);
					mTrailList[indexY][indexX]->SetIsConnected(true);
					//꼬리를 찾아가
					SetTrailTail(indexY + 1, indexX);
				}
			}
			//끝이면
			else
			{
				//내가 꼬리다
				mTrailList[indexY][indexX]->SetIsTail(true);
				mTrailList[indexY][indexX]->SetIsConnected(true);
				return;
			}
			break;

			/////////위를보고있고
		case TrailDirection::Up:
			if (indexY > 0)
			{
				//윗칸에 트레일이 없거나 이미 연결된 트레일이면 
				if (mTrailList[indexY - 1][indexX]->GetTrailType() == TrailType::None || !mTrailList[indexY - 1][indexX]->GetIsConnected())
				{
					//내가 꼬리다
					mTrailList[indexY][indexX]->SetIsTail(true);
					mTrailList[indexY][indexX]->SetIsConnected(true);
					return;
				}
				else
				{
					//난꼬리가 아냐
					mTrailList[indexY][indexX]->SetIsTail(false);
					mTrailList[indexY][indexX]->SetIsConnected(true);
					//꼬리를 찾아가
					SetTrailTail(indexY - 1, indexX);
				}
			}
			//끝이면
			else
			{
				//내가 꼬리다
				mTrailList[indexY][indexX]->SetIsTail(true);
				mTrailList[indexY][indexX]->SetIsConnected(true);
				return;
			}
			break;
			//////////왼쪽보고있고
		case TrailDirection::Left:
			if (indexX > 0)
			{
				//왼칸에 트레일이 없거나 이미 연결된 트레일이면 
				if (mTrailList[indexY][indexX - 1]->GetTrailType() == TrailType::None || !mTrailList[indexY][indexX - 1]->GetIsConnected())
				{
					//내가 꼬리다
					mTrailList[indexY][indexX]->SetIsTail(true);
					mTrailList[indexY][indexX]->SetIsConnected(true);
					return;
				}
				else
				{
					//난꼬리가 아냐
					mTrailList[indexY][indexX]->SetIsTail(false);
					mTrailList[indexY][indexX]->SetIsConnected(true);
					//꼬리를 찾아가
					SetTrailTail(indexY, indexX - 1);
				}
			}
			//끝이면
			else
			{
				//내가 꼬리다
				mTrailList[indexY][indexX]->SetIsTail(true);
				mTrailList[indexY][indexX]->SetIsConnected(true);
				return;
			}
			break;
			///////////오른쪽보고있고
		case TrailDirection::Right:
			if (indexX < TileCountX)
			{
				//오른칸에 트레일이 없거나 이미 연결된 트레일이면 
				if (mTrailList[indexY][indexX + 1]->GetTrailType() == TrailType::None || !mTrailList[indexY][indexX + 1]->GetIsConnected())
				{
					//내가 꼬리다
					mTrailList[indexY][indexX]->SetIsTail(true);
					mTrailList[indexY][indexX]->SetIsConnected(true);
					return;
				}
				else
				{
					//난꼬리가 아냐
					mTrailList[indexY][indexX]->SetIsTail(false);
					mTrailList[indexY][indexX]->SetIsConnected(true);
					//꼬리를 찾아가
					SetTrailTail(indexY, indexX + 1);
				}
			}
			//끝이면
			else
			{
				//내가 꼬리다
				mTrailList[indexY][indexX]->SetIsTail(true);
				mTrailList[indexY][indexX]->SetIsConnected(true);
				return;
			}
			break;

		default:
			break;
		}
	}
}