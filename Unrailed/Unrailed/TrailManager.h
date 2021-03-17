#pragma once

//�������� �ٷ��
class Trail;
class TrailManager
{
	//vector <Trail* > mTrailList;
	vector <vector <Trail*>> mTrailList;
public:
	void Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	
	void InsertTrail(int x, int y, int type, int dir);	//���տ����ֱ�
	void TurnTrail(int indexY, int indexX);		//������
	void PickUpTrail(int indexY, int indexX);			//�ݱ�
	void PlaceTrail(int x, int y, int type, int dir);		//��ġ�ϱ�

	vector <vector <Trail*>>* GetTrailListPtr() { return &mTrailList; }

};

