#include "pch.h"
//#include "Trail.h"
#include "TrailManager.h"
#include "Animation.h"


void TrailManager::Init()
{
	YTileCount = TileCountY;
	XTileCount = TileCountX;
	//��Ʈ���� �δϼȶ�����
	AnimationInit();
	for (int y = 0; y < TileCountY; ++y)
	{
		mTrailList.push_back(vector <Trail*>());
		for (int x = 0; x < TileCountX; ++x)
		{
			//����
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
	//��Ʈ���� �̴ϼȶ�����
	AnimationInit();

	for (int y = 0; y < YTileCount; ++y)
	{
		mTrailList.push_back(vector <Trail*>());
		for (int x = 0; x < XTileCount; ++x)
		{
			//����
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
	//Ʈ���� ������
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

//������ �����Ҷ�
void TrailManager::InsertTrail(int indexY, int indexX, int type, int dir)
{
	//�ش��ε����� �Ӽ�
	mTrailList[indexY][indexX]->SetTrailType(type);
	mTrailList[indexY][indexX]->SetDirection(dir);

	//�ٲ����
	mTrailList[indexY][indexX]->SetIsConnected(true);
}

void TrailManager::TurnTrail(int indexY, int indexX)
{
	if (mTrailList[indexY][indexX]->GetTrailType() == TrailType::None)
		return;
	if (mTrailList[indexY][indexX]->GetIsTail() == false && mTrailList[indexY][indexX]->GetIsConnected() == true)
		return;

	POINT targetIndex;

	mTrailList[indexY][indexX]->Turn();

	if (GetNextTrailIndex(indexX, indexY, &targetIndex))
	{
	
		//�Ʒ��� ����� Ʈ�������ְų�
		if (mTrailList[targetIndex.y][targetIndex.x]->GetIsConnected())
		{
			//���ѷ����� ����°� �ƴϸ�
			if(!CheckIsLoop(indexX,indexY))
				//�ٽõ���
				TurnTrail(indexY, indexX);
		}
		else
		{
			//������ ��������
			SetTrailTail(mStartY, mStartX);
			return;
		}
	}
}

//��Ʈ�Ӹ��� �������� �ݱ�
TrailType TrailManager::PickUpTrail(int indexY, int indexX)
{
	//�ǵ帰�� ������
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
	//�������ƴϸ�
	else
	{
		//Ʈ����Ÿ�� None�� ����
		return TrailType::None;
	}
}

//�÷��̾ Ʈ���� ��ġ
bool TrailManager::PlaceTrail(int indexY, int indexX, int type, int dir)
{
	//�̹̼�ġ���ڸ���
	if (mTrailList[indexY][indexX]->GetTrailType() != TrailType::None)
		return false;

	mTrailList[indexY][indexX]->SetTrailType(type);
	mTrailList[indexY][indexX]->SetDirection(dir);
	mTrailList[indexY][indexX]->SetIsActive(true);
	SetTrailTail(mStartY,mStartX);
	return true;

}

//���ѷ����� ������?
bool TrailManager::CheckIsLoop(int indexX, int indexY)
{
	//���������� �����ƴ�
	if (indexX < 0 || indexX >= XTileCount)return false;
	if (indexY < 0 || indexY >= YTileCount)return false;
	if (mTrailList[indexY][indexX] == nullptr)return false;
	
	POINT nextIndex;
	//����Ʈ�����̾����� �����ƴ�
	if (GetNextTrailIndex(indexX, indexY,&nextIndex) == false)return false;

	//����Ʈ������ �����Ʈ�����̸� ������!
	if (mTrailList[nextIndex.y][nextIndex.x]->GetIsConnected())
		return true;

	//�״����༮Ȯ��
	return CheckIsLoop(nextIndex.x, nextIndex.y);

}

bool TrailManager::GetNextTrailIndex(int indexX, int indexY, POINT* pOutput)
{
	//���������� false;
	if (indexX < 0 || indexX >= XTileCount)return false;
	if (indexY < 0 || indexY >= YTileCount)return false;
	if (mTrailList[indexY][indexX]== nullptr)return false;

	Trail* trail = mTrailList[indexY][indexX];

	int targetIndexX = indexX;
	int targetIndexY = indexY;
	//���⿡���� ��ǥ�ε�������
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
	if (mTrailList[indexY][indexX]->GetTrailType()== TrailType::None)return;
	
	mTrailList[indexY][indexX]->SetOrder(mOrder);
	
	POINT nextIndex;
	//����Ʈ����ã��
	GetNextTrailIndex(indexX, indexY, &nextIndex);
	//����Ʈ������ ������
	if (mTrailList[nextIndex.y][nextIndex.x]->GetTrailType()==TrailType::None)
	{
		//����������
		mTrailList[indexY][indexX]->SetIsConnected(true);
		mTrailList[indexY][indexX]->SetIsTail(true);
		mOrder = 0;
		return;
	}
	//����Ʈ������ �̹̿���Ǿ��ְ� ������ ��߳����ϸ�
	else if (mTrailList[nextIndex.y][nextIndex.x]->GetIsConnected() &&
		mTrailList[nextIndex.y][nextIndex.x]->GetOrder() < mTrailList[indexY][indexX]->GetOrder())
	{
		//����������
		mTrailList[indexY][indexX]->SetIsConnected(true);
		mTrailList[indexY][indexX]->SetIsTail(true);
		mOrder = 0;
		return;

	}
	//�װԾƴϸ�
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



/////�ڵ��� ����

////���� �ִ� ������ ������	//Ʈ������ ���� ���⿡ Ÿ�����ֳ�, �̹� Ŀ��Ƽ��� Ÿ���� �ƴϸ� ��Ÿ���� Ŀ��Ƽ���
//void TrailManager::TurnTrail(int indexY, int indexX)
//{
//	//mTrailList.back()->Turn();
//	if (mTrailList[indexY][indexX]->GetTrailType() == TrailType::None)
//		return;
//
//	//�������°� �����ų� ����ȵ�ģ����
//	if (mTrailList[indexY][indexX]->GetIsTail() || !mTrailList[indexY][indexX]->GetIsConnected())
//	{
//		//�ϴܵ���
//		mTrailList[indexY][indexX]->Turn();
//		//�Ʒ����µ�
//		if (mTrailList[indexY][indexX]->GetDirection() == TrailDirection::Down)
//		{
//			if (indexY < YTileCount-1)
//			{
//				//�Ʒ��� ����� Ʈ�������ְų�
//				if (mTrailList[indexY + 1][indexX]->GetIsConnected())
//				{
//					//�ٽõ���
//					TurnTrail(indexY, indexX);
//				}
//				//���ֺ���������
//				else if (mTrailList[indexY + 1][indexX]->GetDirection() == TrailDirection::Up && mTrailList[indexY + 1][indexX]->GetTrailType() != TrailType::None)
//				{
//					//�ٽõ���
//					TurnTrail(indexY, indexX);
//				}
//				else
//				{
//					//������ ��������
//					SetTrailTail2(mStartY, mStartX);
//					return;
//				}
//			}
//			else
//			{
//				//�ǾƷ����̸� ��������
//				SetTrailTail2(mStartY, mStartX);
//				return;
//			}
//		}
//		//�����µ�
//		if (mTrailList[indexY][indexX]->GetDirection() == TrailDirection::Up)
//		{
//			if (indexY > 0)
//			{
//				//���� �����Ʈ�������ִٸ�
//				if (mTrailList[indexY - 1][indexX]->GetIsConnected())
//				{
//					//�ٽõ���
//					TurnTrail(indexY, indexX);
//				}
//				//���ֺ���������
//				else if (mTrailList[indexY - 1][indexX]->GetDirection() == TrailDirection::Down && mTrailList[indexY - 1][indexX]->GetTrailType() != TrailType::None)
//				{
//					//�ٽõ���
//					TurnTrail(indexY, indexX);
//				}
//				else
//				{
//					//������ ��������
//					SetTrailTail(mStartY, mStartX);
//					return;
//				}
//			}
//			else
//			{
//				//�������̸� ��������
//				SetTrailTail(mStartY, mStartX);
//				return;
//			}
//		}
//		//�����ʺ��µ�
//		if (mTrailList[indexY][indexX]->GetDirection() == TrailDirection::Right)
//		{
//			if (indexX < XTileCount-1)
//			{
//				//�����ʿ� �����Ʈ�������ִٸ�
//				if (mTrailList[indexY][indexX + 1]->GetIsConnected())
//				{
//					//�ٽõ���
//					TurnTrail(indexY, indexX);
//				}
//				//���ֺ���������
//				else if (mTrailList[indexY][indexX+1]->GetDirection() == TrailDirection::Left && mTrailList[indexY][indexX+1]->GetTrailType() != TrailType::None)
//				{
//					//�ٽõ���
//					TurnTrail(indexY, indexX);
//				}
//				else
//				{
//					//������ ��������
//					SetTrailTail2(mStartY, mStartX);
//					return;
//				}
//			}
//			else
//			{
//				//�ǿ��������̸� ��������
//				SetTrailTail2(mStartY, mStartX);
//				return;
//			}
//		}
//		//���ʺ��µ�
//		if (mTrailList[indexY][indexX]->GetDirection() == TrailDirection::Left)
//		{
//			if (indexX > 0)
//			{
//				//���ʿ� �����Ʈ�������ִٸ�
//				if (mTrailList[indexY][indexX - 1]->GetIsConnected())
//				{
//					//�ٽõ���
//					TurnTrail(indexY, indexX);
//				}
//				//���ֺ���������
//				else if (mTrailList[indexY][indexX-1]->GetDirection() == TrailDirection::Right && mTrailList[indexY][indexX-1]->GetTrailType() != TrailType::None)
//				{
//					//�ٽõ���
//					TurnTrail(indexY, indexX);
//				}
//				else
//				{
//					//������ ��������
//					SetTrailTail2(mStartY, mStartX);
//					return;
//				}
//			}
//			else
//			{
//				//�ǿ������̸� ��������
//				SetTrailTail2(mStartY, mStartX);
//				return;
//			}
//		}
//	}
//}
//���� Ʈ������ �ε����� �־� ����������������
//void TrailManager::SetTrailTail(int indexY, int indexX)
//{
//	//��Ʈ������ �ƴϰ�
//	if (mTrailList[indexY][indexX]->GetTrailType() != TrailType::None)
//	{
//		switch (mTrailList[indexY][indexX]->GetDirection())
//		{
//			////////////�Ʒ������ְ�
//		case TrailDirection::Down:
//			if (indexY < YTileCount-1)
//			{
//				//�Ʒ�ĭ�� Ʈ������ ������ 
//				if (mTrailList[indexY + 1][indexX]->GetTrailType() == TrailType::None)
//				{
//					//���� ������
//					mTrailList[indexY][indexX]->SetIsTail(true);
//					mTrailList[indexY][indexX]->SetIsConnected(true);
//					return;
//				}
//				else
//				{
//					//�������� �Ƴ�
//					mTrailList[indexY][indexX]->SetIsTail(false);
//					mTrailList[indexY][indexX]->SetIsConnected(true);
//					//������ ã�ư�
//					SetTrailTail(indexY + 1, indexX);
//				}
//			}
//			//�����̸�
//			else
//			{
//				//���� ������
//				mTrailList[indexY][indexX]->SetIsTail(true);
//				mTrailList[indexY][indexX]->SetIsConnected(true);
//				return;
//			}
//			break;
//
//			/////////���������ְ�
//		case TrailDirection::Up:
//			if (indexY > 0)
//			{
//				//��ĭ�� Ʈ������ ������ 
//				if (mTrailList[indexY - 1][indexX]->GetTrailType() == TrailType::None)
//				{
//					//���� ������
//					mTrailList[indexY][indexX]->SetIsTail(true);
//					mTrailList[indexY][indexX]->SetIsConnected(true);
//					return;
//				}
//				else
//				{
//					//�������� �Ƴ�
//					mTrailList[indexY][indexX]->SetIsTail(false);
//					mTrailList[indexY][indexX]->SetIsConnected(true);
//					//������ ã�ư�
//					SetTrailTail(indexY - 1, indexX);
//				}
//			}
//			//���̸�
//			else
//			{
//				//���� ������
//				mTrailList[indexY][indexX]->SetIsTail(true);
//				mTrailList[indexY][indexX]->SetIsConnected(true);
//				return;
//			}
//			break;
//			//////////���ʺ����ְ�
//		case TrailDirection::Left:
//			if (indexX > 0)
//			{
//				//��ĭ�� Ʈ������ ������ 
//				if (mTrailList[indexY][indexX - 1]->GetTrailType() == TrailType::None)
//				{
//					//���� ������
//					mTrailList[indexY][indexX]->SetIsTail(true);
//					mTrailList[indexY][indexX]->SetIsConnected(true);
//					return;
//				}
//				else
//				{
//					//�������� �Ƴ�
//					mTrailList[indexY][indexX]->SetIsTail(false);
//					mTrailList[indexY][indexX]->SetIsConnected(true);
//					//������ ã�ư�
//					SetTrailTail(indexY, indexX - 1);
//				}
//			}
//			//���̸�
//			else
//			{
//				//���� ������
//				mTrailList[indexY][indexX]->SetIsTail(true);
//				mTrailList[indexY][indexX]->SetIsConnected(true);
//				return;
//			}
//			break;
//			///////////�����ʺ����ְ�
//		case TrailDirection::Right:
//			if (indexX < XTileCount-1)
//			{
//				//����ĭ�� Ʈ������ ������ 
//				if (mTrailList[indexY][indexX + 1]->GetTrailType() == TrailType::None)
//				{
//					//���� ������
//					mTrailList[indexY][indexX]->SetIsTail(true);
//					mTrailList[indexY][indexX]->SetIsConnected(true);
//					return;
//				}
//				else
//				{
//					//�������� �Ƴ�
//					mTrailList[indexY][indexX]->SetIsTail(false);
//					mTrailList[indexY][indexX]->SetIsConnected(true);
//					//������ ã�ư�
//					SetTrailTail(indexY, indexX + 1);
//				}
//			}
//			//���̸�
//			else
//			{
//				//���� ������
//				mTrailList[indexY][indexX]->SetIsTail(true);
//				mTrailList[indexY][indexX]->SetIsConnected(true);
//				return;
//			}
//			break;
//
//		default:
//			break;
//		}
//	}
//}
//