#include "pch.h"
#include "TrailManager.h"
#include "Trail.h"

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
void TrailManager::InsertTrail(int x, int y, int type, int dir)
{
	//�ش��ε����� �Ӽ�
	mTrailList[y][x]->SetTrailType(type);
	mTrailList[y][x]->SetDirection(dir);

	//�ٲ����
	mTrailList[y][x]->SetIsConnected(true);
}




//���� �ִ� ������ ������	//Ʈ������ ���� ���⿡ Ÿ�����ֳ�, �̹� Ŀ��Ƽ��� Ÿ���� �ƴϸ� ��Ÿ���� Ŀ��Ƽ���
void TrailManager::TurnTrail(int indexY, int indexX)
{
	//mTrailList.back()->Turn();
	//�������°� ������
	if (mTrailList[indexY][indexX]->GetIsTail())
	{
		//����ٲٰ�
		mTrailList[indexY][indexX]->Turn();



		//Ÿ��ã��
		//�Ʒ��������ְ�
		if (mTrailList[indexY][indexX]->GetDirection() == TrailDirection::Down)
		{
			//�ǾƷ����� �ƴϰ�
			if (indexY < TileCountY)
			{
				//�Ʒ��ٳ༮��
				if (mTrailList[indexY + 1][indexX]->GetTrailType() != TrailType::None)
				{
					//�̹� ����Ȱ� �ƴϸ�
					if (!mTrailList[indexY + 1][indexX]->GetIsConnected())
					{
						//�����Ű�� ����ü����
						mTrailList[indexY][indexX]->SetIsTail(false);
						mTrailList[indexY + 1][indexX]->SetIsTail(true);
						mTrailList[indexY + 1][indexX]->SetIsConnected(true);
					}
				}

			}
		}
		//���������ִµ�
		if (mTrailList[indexY][indexX]->GetDirection() == TrailDirection::Up)
		{
			//�������̾ƴϰ�
			if (indexY > 0)
			{
				//���ٳ༮��
				if (mTrailList[indexY - 1][indexX]->GetTrailType() != TrailType::None)
				{
					//�̹� ����Ȱ� �ƴϸ�
					if (!mTrailList[indexY - 1][indexX]->GetIsConnected())
					{
						//�����Ű�� ����ü����
						mTrailList[indexY][indexX]->SetIsTail(false);
						mTrailList[indexY - 1][indexX]->SetIsTail(true);
						mTrailList[indexY - 1][indexX]->SetIsConnected(true);
					}
				}
			}

		}
		//�����������ִµ�
		if (mTrailList[indexY][indexX]->GetDirection() == TrailDirection::Left)
		{
			//�ǿ������̾ƴϰ�
			if (indexX > 0)
			{
				//���ʳ༮��
				if (mTrailList[indexY][indexX - 1]->GetTrailType() != TrailType::None)
				{
					//�̹� ����Ȱ� �ƴϸ�
					if (!mTrailList[indexY][indexX - 1]->GetIsConnected())
					{
						//�����Ű�� ����ü����
						mTrailList[indexY][indexX]->SetIsTail(false);
						mTrailList[indexY][indexX - 1]->SetIsTail(true);
						mTrailList[indexY][indexX - 1]->SetIsConnected(true);
					}
				}
			}

		}
		//�������������ִµ�
		if (mTrailList[indexY][indexX]->GetDirection() == TrailDirection::Right)
		{
			//�ǿ��������̾ƴϰ�
			if (indexX > TileCountX)
			{
				//�����ʳ༮��
				if (mTrailList[indexY][indexX + 1]->GetTrailType() != TrailType::None)
				{
					//�̹� ����Ȱ� �ƴϸ�
					if (!mTrailList[indexY][indexX + 1]->GetIsConnected())
					{
						//�����Ű�� ����ü����
						mTrailList[indexY][indexX]->SetIsTail(false);
						mTrailList[indexY][indexX + 1]->SetIsTail(true);
						mTrailList[indexY][indexX + 1]->SetIsConnected(true);
					}
				}
			}

		}
	}
}

//��Ʈ�Ӹ��� �������� �ݱ�
void TrailManager::PickUpTrail(int indexY, int indexX)
{
	//mTrailList.pop_back();
	//�ǵ帰�� ������
	if (mTrailList[indexY][indexX]->GetIsTail())
	{
		mTrailList[indexY][indexX]->SetTrailType(0);
		mTrailList[indexY][indexX]->SetIsActive(false);
		mTrailList[indexY][indexX]->SetIsTail(false);

		//��������ֺ� üũ, ���������ϱ�
		if (indexY > 0)
		{
			//������ ����ȳ��̰�
			if (mTrailList[indexY - 1][indexX]->GetIsConnected())
			{
				//�Ʒ��� ����������
				if (mTrailList[indexY - 1][indexX]->GetDirection() == TrailDirection::Down)
				{
					//�׳��� ����
					mTrailList[indexY - 1][indexX]->SetIsTail(true);
				}
			}

		}
		if (indexY < TileCountY)
		{
			//�Ʒ����� ����ȳ��̰�
			if (mTrailList[indexY + 1][indexX]->GetIsConnected())
			{
				//���� ����������
				if (mTrailList[indexY + 1][indexX]->GetDirection() == TrailDirection::Up)
				{
					//�׳��� ����
					mTrailList[indexY][indexX]->SetIsTail(true);
				}
			}
		}
		if (indexX > 0)
		{
			//���ʳ��� ����ȳ��̰�
			if (mTrailList[indexY][indexX - 1]->GetIsConnected())
			{
				//�������� ����������
				if (mTrailList[indexY][indexX - 1]->GetDirection() == TrailDirection::Right)
				{
					//�׳��� ����
					mTrailList[indexY][indexX]->SetIsTail(true);
				}
			}
		}
		if (indexX < TileCountX)
		{
			//�����ʳ��� ����ȳ��̰�
			if (mTrailList[indexY][indexX + 1]->GetIsConnected())
			{
				//������ ����������
				if (mTrailList[indexY][indexX + 1]->GetDirection() == TrailDirection::Left)
				{
					//�׳��� ����
					mTrailList[indexY][indexX]->SetIsTail(true);
				}
			}
		}
	}
}

//�÷��̾ Ʈ���� ��ġ
void TrailManager::PlaceTrail(int x, int y, int type, int dir)
{
	mTrailList[y][x]->SetTrailType(type);
	mTrailList[y][x]->SetDirection(dir);
	mTrailList[y][x]->SetIsActive(true);

	//��ġ�ѳ༮�� ����üũ
	if (y > 0)
	{
		//������ ����ȳ��̰�
		if (mTrailList[y - 1][x]->GetIsConnected())
		{
			//�Ʒ��� ����������
			if (mTrailList[y - 1][x]->GetDirection() == TrailDirection::Down)
			{
				//��ġ�ѳ��� ����ȳ�
				mTrailList[y][x]->SetIsConnected(true);

				//������ü
				mTrailList[y - 1][x]->SetIsTail(false);
				mTrailList[y][x]->SetIsTail(true);
			}
		}

	}
	if (y < TileCountY)
	{
		//�Ʒ����� ����ȳ��̰�
		if (mTrailList[y + 1][x]->GetIsConnected())
		{
			//���� ����������
			if (mTrailList[y + 1][x]->GetDirection() == TrailDirection::Up)
			{
				//��ġ�ѳ��� ����ȳ�
				mTrailList[y][x]->SetIsConnected(true);

				//������ü
				mTrailList[y + 1][x]->SetIsTail(false);
				mTrailList[y][x]->SetIsTail(true);
			}
		}
	}
	if (x > 0)
	{
		//���ʳ��� ����ȳ��̰�
		if (mTrailList[y][x - 1]->GetIsConnected())
		{
			//�������� ����������
			if (mTrailList[y][x - 1]->GetDirection() == TrailDirection::Right)
			{
				//��ġ�ѳ��� ����ȳ�
				mTrailList[y][x]->SetIsConnected(true);

				//������ü
				mTrailList[y][x - 1]->SetIsTail(false);
				mTrailList[y][x]->SetIsTail(true);
			}
		}
	}
	if (x < TileCountX)
	{
		//�����ʳ��� ����ȳ��̰�
		if (mTrailList[y][x + 1]->GetIsConnected())
		{
			//������ ����������
			if (mTrailList[y][x + 1]->GetDirection() == TrailDirection::Left)
			{
				//��ġ�ѳ��� ����ȳ�
				mTrailList[y][x]->SetIsConnected(true);


				//������ü
				mTrailList[y][x + 1]->SetIsTail(false);
				mTrailList[y][x]->SetIsTail(true);
			}
		}
	}

}

//���� Ʈ������ �ε����� �־� ������ã��
//void TrailManager::SetTrailTail(int indexY, int indexX)
//{
//	//����� Ʈ�����̰�
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