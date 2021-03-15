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

	void PushEvent(IEvent* event);
};

