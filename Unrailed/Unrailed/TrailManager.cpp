#include "pch.h"
//#include "Trail.h"
#include "TrailManager.h"


void TrailManager::Init()
{
	YTileCount = TileCountY;
	XTileCount = TileCountX;
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

void TrailManager::Init(int y, int x)
{
	YTileCount = y;
	XTileCount = x;
	//��Ʈ���� ����
	for (int y = 0; y < YTileCount; ++y)
	{
		mTrailList.push_back(vector <Trail*>());
		for (int x = 0; x < XTileCount; ++x)
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
			if (indexY < YTileCount)
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
			if (indexX < XTileCount)
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
	else
	{
		return TrailType::None;
	}
}

//�÷��̾ Ʈ���� ��ġ
bool TrailManager::PlaceTrail(int indexY, int indexX, int type, int dir)
{
	int counter = 4;
	bool up = true;
	bool down = true;
	bool left = true;
	bool right = true;


	//��� �����ֳ� üũ
	//�Ʒ� Ȯ������
	if (indexY < YTileCount)
	{
		if (mTrailList[indexY + 1][indexX]->GetIsConnected())
		{
			counter--;
			down = false;
		}
	}
	//��Ȯ��
	if (indexY > 0) 
	{
		if (mTrailList[indexY - 1][indexX]->GetIsConnected())
		{
			counter--;
			up = false;
		}
	}
	//������Ȯ��
	if (indexX  < XTileCount)
	{
		if (mTrailList[indexY][indexX + 1]->GetIsConnected())
		{
			counter--;
			right = false;
		}
	}
	//����Ȯ��
	if (indexX > 0)
	{
		if (mTrailList[indexY][indexX - 1]->GetIsConnected())
		{
			counter--;
			left = false;
		}
	}
	if (!counter)
	{
		return false;
	}

	//�������ϱ�
	//�Ʒ��γְ�;�
	if (dir == 0)
	{
		//�Ʒ���������
		if (!down)
		{
			//��
			dir++;
			//����������
			if (!up)
			{
				//����
				dir++;
				//�޸�������
				if (!left)
				{
					//��
					dir++;
				}
			}
		}
	}
	//���γְ�;�
	else if (dir == 1)
	{
		//����������
		if (!up)
		{
			//��
			dir++;
			//�޸�������
			if (!left)
			{
				//��
				dir++;
				//����������
				if (!right)
				{
					//�Ʒ�
					dir = 0;
				}
			}
		}
	}
	//�������γְ�;�
	else if (dir == 2)
	{
		//�޸�������
		if (!left)
		{
			//��
			dir++;
			//����������
			if (!right)
			{
				//�Ʒ�
				dir = 0;
				//�Ʒ���������
				if (!down)
				{
					//��
					dir++;
				}
			}
		}
	}
	//���������� �ְ�;�
	else if (dir == 3)
	{
		//����������
		if (!right)
		{
			//�Ʒ�
			dir = 0;
			//�Ʒ���������
			if (!down)
			{
				//��
				dir++;
				//����������
				if (!up)
				{
					//����
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
			if (indexY < YTileCount)
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
			if (indexX < XTileCount)
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