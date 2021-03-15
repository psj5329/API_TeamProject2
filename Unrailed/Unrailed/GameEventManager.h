#pragma once

class IEvent;
class GameEventManager
{
	Singleton(GameEventManager)

private:
	queue<IEvent*> mEventQueue;	// 큐로 바꿔보았다
								// 근데 이벤트 쓰긴 할까?

public:
	GameEventManager();
	~GameEventManager();

	void RemoveAllEvent();
	void Update();

	void PushEvent(IEvent* event);
};

