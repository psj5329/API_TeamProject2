#pragma once

enum ObjectLayer { BACKGROUND, ITEM, TRAIL, PLAYER, ENEMY, UI, End };

class GameObject;
class ObjectManager
{
	Singleton(ObjectManager)

private:
	typedef map<ObjectLayer, vector<GameObject*>>::iterator ObjectIter;
	map<ObjectLayer, vector<GameObject*>> mObjectList;
public:
	ObjectManager();

	void Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	void AddObject(ObjectLayer layer, class GameObject* object);
	GameObject* FindObject(const string& name);
	GameObject* FindObject(ObjectLayer layer, const string& name);
	vector<GameObject*> FindObjects(const string& name);
	vector<GameObject*> FindObjects(ObjectLayer layer, const string& name);
	vector<GameObject*> GetObjectList(ObjectLayer layer);
};

#define OBJECTMANAGER ObjectManager::GetInstance()