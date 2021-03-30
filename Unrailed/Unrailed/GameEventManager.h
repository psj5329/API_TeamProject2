#pragma once

class IEvent;
class GameEventManager
{
	Singleton(GameEventManager)

private:
	queue<IEvent*> mEventQueue;	// ť�� �ٲ㺸�Ҵ�
								// �ٵ� �̺�Ʈ ���� �ұ�?

public:
	GameEventManager();
	~GameEventManager();

	void RemoveAllEvent();
	void Update();
	void Render(HDC hdc);

	void PushEvent(IEvent* event);
	
	int GetEventCount() { return mEventQueue.size(); }
};

#define GAMEEVENTMANAGER GameEventManager::GetInstance()
