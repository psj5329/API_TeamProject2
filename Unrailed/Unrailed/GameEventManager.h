#pragma once

class GameEventManager
{
	Singleton(GameEventManager)

private:
	vector<class IEvent*> mEventQueue;

public:
	GameEventManager();
	~GameEventManager();

	void RemoveAllEvent();
	void Update();

	void PushEvent(class IEvent* event);
};

