#include "pch.h"
//#include "Trail.h"
#include "TrailManager.h"


void TrailManager::Init()
{
	//��Ʈ���� ����
	for (int y = 0; y < TileCountY; ++y)
	{
		mTrailList.push_back(vector <Trail*>());
		for (int x = 0; x < TileCountX; ++x)
		{
			//����
			Trail* trail = new Trail();
			trail->Init(TileSize * x, TileSize * y, 0, 0);
			mTrailList[y].push_back(trail);

		}
	}
}

void TrailManager::Release()
{
	//Ÿ�� ������
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

//������ �����Ҷ�
void TrailManager::InsertTrail(int indexY, int indexX, int type, int dir)
{
	//�ش��ε����� �Ӽ�
	mTrailList[indexY][indexX]->SetTrailType(type);
	mTrailList[indexY][indexX]->SetDirection(dir);

	//�ٲ����
	mTrailList[indexY][indexX]->SetIsConnected(true);
}




//���� �ִ� ������ ������	//Ʈ������ ���� ���⿡ Ÿ�����ֳ�, �̹� Ŀ��Ƽ��� Ÿ���� �ƴϸ� ��Ÿ���� Ŀ��Ƽ���
void TrailManager::TurnTrail(int indexY, int indexX)
{
	//mTrailList.back()->Turn();
	//�������°� ������
	if (mTrailList[indexY][indexX]->GetIsTail())
	{
		//�ϴܵ���
		mTrailList[indexY][indexX]->Turn();
		//�Ʒ����µ�
		if (mTrailList[indexY][indexX]->GetDirection() == TrailDirection::Down)
		{
			if (indexY < TileCountY)
			{
				//�Ʒ��� �����Ʈ�������ִٸ�
				if (mTrailList[indexY + 1][indexX]->GetIsConnected())
				{
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
			else
			{
				//�ǾƷ����̸� ��������
				SetTrailTail(mStartY, mStartX);
				return;
			}
		}
		//�����µ�
		if (mTrailList[indexY][indexX]->GetDirection() == TrailDirection::Down)
		{
			if (indexY > 0)
			{
				//���� �����Ʈ�������ִٸ�
				if (mTrailList[indexY - 1][indexX]->GetIsConnected())
				{
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
			else
			{
				//�������̸� ��������
				SetTrailTail(mStartY, mStartX);
				return;
			}
		}
		//�����ʺ��µ�
		if (mTrailList[indexY][indexX]->GetDirection() == TrailDirection::Right)
		{
			if (indexX < TileCountX)
			{
				//�����ʿ� �����Ʈ�������ִٸ�
				if (mTrailList[indexY][indexX + 1]->GetIsConnected())
				{
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
			else
			{
				//�ǿ��������̸� ��������
				SetTrailTail(mStartY, mStartX);
				return;
			}
		}
		//���ʺ��µ�
		if (mTrailList[indexY][indexX]->GetDirection() == TrailDirection::Left)
		{
			if (indexX > 0)
			{
				//���ʿ� �����Ʈ�������ִٸ�
				if (mTrailList[indexY][indexX - 1]->GetIsConnected())
				{
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
			else
			{
				//�ǿ������̸� ��������
				SetTrailTail(mStartY, mStartX);
				return;
			}
		}
	}
}

//��Ʈ�Ӹ��� �������� �ݱ�
TrailType TrailManager::PickUpTrail(int indexY, int indexX)
{
	//mTrailList.pop_back();
	//�ǵ帰�� ������
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

//�÷��̾ Ʈ���� ��ġ
bool TrailManager::PlaceTrail(int indexY, int indexX, int type, int dir)
{
	//��ó���ؾ���
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

//���� Ʈ������ �ε����� �־� ����������������
void TrailManager::SetTrailTail(int indexY, int indexX)
{
	//��Ʈ������ �ƴϰ�
	if (mTrailList[indexY][indexX]->GetTrailType() != TrailType::None)
	{
		switch (mTrailList[indexY][indexX]->GetDirection())
		{
			////////////�Ʒ������ְ�
		case TrailDirection::Down:
			if (indexY < TileCountY)
			{
				//�Ʒ�ĭ�� Ʈ������ ���ų� �̹� ����� Ʈ�����̸� 
				if (mTrailList[indexY + 1][indexX]->GetTrailType() == TrailType::None || !mTrailList[indexY + 1][indexX]->GetIsConnected())
				{
					//���� ������
					mTrailList[indexY][indexX]->SetIsTail(true);
					mTrailList[indexY][indexX]->SetIsConnected(true);
					return;
				}
				else
				{
					//�������� �Ƴ�
					mTrailList[indexY][indexX]->SetIsTail(false);
					mTrailList[indexY][indexX]->SetIsConnected(true);
					//������ ã�ư�
					SetTrailTail(indexY + 1, indexX);
				}
			}
			//���̸�
			else
			{
				//���� ������
				mTrailList[indexY][indexX]->SetIsTail(true);
				mTrailList[indexY][indexX]->SetIsConnected(true);
				return;
			}
			break;

			/////////���������ְ�
		case TrailDirection::Up:
			if (indexY > 0)
			{
				//��ĭ�� Ʈ������ ���ų� �̹� ����� Ʈ�����̸� 
				if (mTrailList[indexY - 1][indexX]->GetTrailType() == TrailType::None || !mTrailList[indexY - 1][indexX]->GetIsConnected())
				{
					//���� ������
					mTrailList[indexY][indexX]->SetIsTail(true);
					mTrailList[indexY][indexX]->SetIsConnected(true);
					return;
				}
				else
				{
					//�������� �Ƴ�
					mTrailList[indexY][indexX]->SetIsTail(false);
					mTrailList[indexY][indexX]->SetIsConnected(true);
					//������ ã�ư�
					SetTrailTail(indexY - 1, indexX);
				}
			}
			//���̸�
			else
			{
				//���� ������
				mTrailList[indexY][indexX]->SetIsTail(true);
				mTrailList[indexY][indexX]->SetIsConnected(true);
				return;
			}
			break;
			//////////���ʺ����ְ�
		case TrailDirection::Left:
			if (indexX > 0)
			{
				//��ĭ�� Ʈ������ ���ų� �̹� ����� Ʈ�����̸� 
				if (mTrailList[indexY][indexX - 1]->GetTrailType() == TrailType::None || !mTrailList[indexY][indexX - 1]->GetIsConnected())
				{
					//���� ������
					mTrailList[indexY][indexX]->SetIsTail(true);
					mTrailList[indexY][indexX]->SetIsConnected(true);
					return;
				}
				else
				{
					//�������� �Ƴ�
					mTrailList[indexY][indexX]->SetIsTail(false);
					mTrailList[indexY][indexX]->SetIsConnected(true);
					//������ ã�ư�
					SetTrailTail(indexY, indexX - 1);
				}
			}
			//���̸�
			else
			{
				//���� ������
				mTrailList[indexY][indexX]->SetIsTail(true);
				mTrailList[indexY][indexX]->SetIsConnected(true);
				return;
			}
			break;
			///////////�����ʺ����ְ�
		case TrailDirection::Right:
			if (indexX < TileCountX)
			{
				//����ĭ�� Ʈ������ ���ų� �̹� ����� Ʈ�����̸� 
				if (mTrailList[indexY][indexX + 1]->GetTrailType() == TrailType::None || !mTrailList[indexY][indexX + 1]->GetIsConnected())
				{
					//���� ������
					mTrailList[indexY][indexX]->SetIsTail(true);
					mTrailList[indexY][indexX]->SetIsConnected(true);
					return;
				}
				else
				{
					//�������� �Ƴ�
					mTrailList[indexY][indexX]->SetIsTail(false);
					mTrailList[indexY][indexX]->SetIsConnected(true);
					//������ ã�ư�
					SetTrailTail(indexY, indexX + 1);
				}
			}
			//���̸�
			else
			{
				//���� ������
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