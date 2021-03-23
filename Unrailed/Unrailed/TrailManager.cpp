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
	if (mTrailList[indexY][indexX]->GetTrailType() == TrailType::None)
		return;

	//돌릴려는게 꼬리거나 연결안된친구면
	if (mTrailList[indexY][indexX]->GetIsTail() || !mTrailList[indexY][indexX]->GetIsConnected())
	{
		//일단돌려
		mTrailList[indexY][indexX]->Turn();
		//아래보는데
		if (mTrailList[indexY][indexX]->GetDirection() == TrailDirection::Down)
		{
			if (indexY < YTileCount-1)
			{
				//아래에 연결된 트레일이있거나
				if (mTrailList[indexY + 1][indexX]->GetIsConnected())
				{
					//다시돌려
					TurnTrail(indexY, indexX);
				}
				//마주보고있으면
				else if (mTrailList[indexY + 1][indexX]->GetDirection() == TrailDirection::Up && mTrailList[indexY + 1][indexX]->GetTrailType() != TrailType::None)
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
		if (mTrailList[indexY][indexX]->GetDirection() == TrailDirection::Up)
		{
			if (indexY > 0)
			{
				//위에 연결된트레일이있다면
				if (mTrailList[indexY - 1][indexX]->GetIsConnected())
				{
					//다시돌려
					TurnTrail(indexY, indexX);
				}
				//마주보고있으면
				else if (mTrailList[indexY - 1][indexX]->GetDirection() == TrailDirection::Down && mTrailList[indexY - 1][indexX]->GetTrailType() != TrailType::None)
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
			if (indexX < XTileCount-1)
			{
				//오른쪽에 연결된트레일이있다면
				if (mTrailList[indexY][indexX + 1]->GetIsConnected())
				{
					//다시돌려
					TurnTrail(indexY, indexX);
				}
				//마주보고있으면
				else if (mTrailList[indexY][indexX+1]->GetDirection() == TrailDirection::Left && mTrailList[indexY][indexX+1]->GetTrailType() != TrailType::None)
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
				//마주보고있으면
				else if (mTrailList[indexY][indexX-1]->GetDirection() == TrailDirection::Right && mTrailList[indexY][indexX-1]->GetTrailType() != TrailType::None)
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

void TrailManager::TurnTrail2(int indexY, int indexX)
{
	//mTrailList.back()->Turn();
	if (mTrailList[indexY][indexX]->GetTrailType() == TrailType::None)
		return;

	if (mTrailList[indexY][indexX]->GetIsTail() == false && mTrailList[indexY][indexX]->GetIsConnected() == true)
		return;

	int targetIndexX;
	int targetIndexY;
	TrailDirection reverseDirection;
	bool isEndLine = false;
	switch (mTrailList[indexY][indexX]->GetDirection())
	{
	case TrailDirection::Down:
		targetIndexX = indexX;
		targetIndexY = indexY + 1;
		reverseDirection = TrailDirection::Up;
		isEndLine = indexY < YTileCount - 1 ? true : false;
		break;
	case TrailDirection::Up:
		targetIndexX = indexX;
		targetIndexY = indexY - 1;
		reverseDirection = TrailDirection::Down;
		break;
	case TrailDirection::Right:
		targetIndexX = indexX + 1;
		targetIndexY = indexY;
		reverseDirection = TrailDirection::Left;
		break;
	case TrailDirection::Left:
		targetIndexX = indexX - 1;
		targetIndexY = indexY;
		reverseDirection = TrailDirection::Right;
		break;
	}

	if (isEndLine)
	{
		//아래에 연결된 트레일이있거나
		if (mTrailList[targetIndexY][targetIndexX]->GetIsConnected())
		{
			//다시돌려
			TurnTrail(indexY, indexX);
		}
		//마주보고있으면
		else if (mTrailList[targetIndexY][targetIndexX]->GetDirection() == reverseDirection && 
			mTrailList[targetIndexY][targetIndexX]->GetTrailType() != TrailType::None)
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

//끄트머리의 기차길을 줍기
TrailType TrailManager::PickUpTrail(int indexY, int indexX)
{
	//건드린게 꼬리면
	if (mTrailList[indexY][indexX]->GetIsTail())
	{
		TrailType type = mTrailList[indexY][indexX]->GetTrailType();
		mTrailList[indexY][indexX]->SetTrailType(0);
		mTrailList[indexY][indexX]->SetIsActive(false);
		mTrailList[indexY][indexX]->SetIsTail(false);
		mTrailList[indexY][indexX]->SetIsConnected(false);

		SetTrailTail(mStartY,mStartX);

		return type;
	}
	else
	{
		return TrailType::None;
	}
}

//플레이어가 트레일 설치
bool TrailManager::PlaceTrail(int indexY, int indexX, int type, int dir)
{
	int counter = 4;
	bool up = true;
	bool down = true;
	bool left = true;
	bool right = true;


	//어디가 막혀있나 체크
	//아래 확인하자
	if (indexY < YTileCount-1)
	{
		if (mTrailList[indexY + 1][indexX]->GetIsConnected())
		{
			counter--;
			down = false;
		}
		else if (mTrailList[indexY + 1][indexX]->GetTrailType() != TrailType::None && mTrailList[indexY + 1][indexX]->GetDirection() == TrailDirection::Up)
		{
			counter--;
			down = false;
		}
	}
	//위확인
	if (indexY > 0) 
	{
		if (mTrailList[indexY - 1][indexX]->GetIsConnected())
		{
			counter--;
			up = false;
		}
		else if (mTrailList[indexY - 1][indexX]->GetTrailType() != TrailType::None && mTrailList[indexY - 1][indexX]->GetDirection() == TrailDirection::Down)
		{
			counter--;
			up = false;
		}
	}
	//오른쪽확인
	if (indexX  < XTileCount-1)
	{
		if (mTrailList[indexY][indexX + 1]->GetIsConnected())
		{
			counter--;
			right = false;
		}
		else if (mTrailList[indexY][indexX + 1]->GetTrailType() != TrailType::None && mTrailList[indexY][indexX + 1]->GetDirection() == TrailDirection::Left)
		{
			counter--;
			right = false;
		}
	}
	//왼쪽확인
	if (indexX > 0)
	{
		if (mTrailList[indexY][indexX - 1]->GetIsConnected())
		{
			counter--;
			left = false;
		}
		else if (mTrailList[indexY][indexX - 1]->GetTrailType() != TrailType::None && mTrailList[indexY][indexX - 1]->GetDirection() == TrailDirection::Left)
		{
			counter--;
			left = false;
		}
	}
	if (!counter)
	{
		return false;
	}

	//방향정하기
	//아래로넣고싶어
	if (dir == 0)
	{
		//아래막혔으면
		if (!down)
		{
			//위
			dir++;
			//위막혔으면
			if (!up)
			{
				//왼쪽
				dir++;
				//왼막혔으면
				if (!left)
				{
					//오
					dir++;
				}
			}
		}
	}
	//위로넣고싶어
	else if (dir == 1)
	{
		//위막혔으면
		if (!up)
		{
			//왼
			dir++;
			//왼막혔으면
			if (!left)
			{
				//오
				dir++;
				//오막혔으면
				if (!right)
				{
					//아래
					dir = 0;
				}
			}
		}
	}
	//왼쪽으로넣고싶어
	else if (dir == 2)
	{
		//왼막혔으면
		if (!left)
		{
			//오
			dir++;
			//오막혔으면
			if (!right)
			{
				//아래
				dir = 0;
				//아래막혔으면
				if (!down)
				{
					//위
					dir++;
				}
			}
		}
	}
	//오른쪽으로 넣고싶어
	else if (dir == 3)
	{
		//오막혔으면
		if (!right)
		{
			//아래
			dir = 0;
			//아래막혔으면
			if (!down)
			{
				//위
				dir++;
				//위막혔으면
				if (!up)
				{
					//왼쪽
					dir++;
				}
			}
		}
	}

	mTrailList[indexY][indexX]->SetTrailType(type);
	mTrailList[indexY][indexX]->SetDirection(dir);
	mTrailList[indexY][indexX]->SetIsActive(true);
	SetTrailTail(mStartY,mStartX);
	return true;

}

bool TrailManager::CheckIsLoop(int indexX, int indexY)
{
	if (indexX < 0 || indexX >= XTileCount)return false;
	if (indexY < 0 || indexY >= YTileCount)return false;
	if (mTrailList[indexY][indexX] == nullptr)return false;
	
	POINT nextIndex;
	if (GetNextTrailIndex(indexX, indexY,&nextIndex) == false)return false;

	if (mTrailList[nextIndex.y][nextIndex.x]->GetIsConnected())
		return true;

	return CheckIsLoop(nextIndex.x, nextIndex.y);

}

bool TrailManager::GetNextTrailIndex(int indexX, int indexY, POINT* pOutput)
{
	if (indexX < 0 || indexX >= XTileCount)return false;
	if (indexY < 0 || indexY >= YTileCount)return false;
	if (mTrailList[indexY][indexX]->GetTrailType() == TrailType::None)return false;

	Trail* trail = mTrailList[indexY][indexX];

	int targetIndexX = indexX;
	int targetIndexY = indexY;
	switch (trail->GetDirection())
	{
	case TrailDirection::Down:
		targetIndexY--;
		break;
	case TrailDirection::Up:
		targetIndexY++;
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
void TrailManager::SetTrailTail2(int indexY, int indexX)
{
	//if (indexX < 0 || indexX >= XTileCount)return;
	//if (indexY < 0 || indexY >= YTileCount)return;
	//if (mTrailList[indexY][indexX]->GetTrailType == TrailType::None)return;
	//
	//POINT nextIndex;
	//if (GetNextTrailIndex(indexX, indexY, &nextIndex) == false)
	//{
	//	mTrailList[indexY][indexX]->SetIsConnected(true);
	//	mTrailList[indexY][indexX]->SetIsTail(true);
	//	return;
	//}
	//else if ()
	//{
	//	mTrailList[indexY][indexX]->SetIsConnected(true);
	//	mTrailList[indexY][indexX]->SetIsTail(true);
	//	return;
	//}
	//else
	//{
	//	mTrailList[indexY][indexX]->SetIsConnected(true);
	//	SetTrailTail2(nextIndex.y, nextIndex.x);
	//}

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
			if (indexY < YTileCount-1)
			{
				//아래칸에 트레일이 없으면 
				if (mTrailList[indexY + 1][indexX]->GetTrailType() == TrailType::None)
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
			//끝줄이면
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
				//윗칸에 트레일이 없으면 
				if (mTrailList[indexY - 1][indexX]->GetTrailType() == TrailType::None)
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
				//왼칸에 트레일이 없으면 
				if (mTrailList[indexY][indexX - 1]->GetTrailType() == TrailType::None)
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
			if (indexX < XTileCount-1)
			{
				//오른칸에 트레일이 없으면 
				if (mTrailList[indexY][indexX + 1]->GetTrailType() == TrailType::None)
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