#pragma once
#define WINSTARTX 50
#define WINSTARTY 50
#define WINSIZEX 1280
#define WINSIZEY 720

//�ʰ���
#define TileCountX 10
#define TileCountY 10
#define TileSize 48
#define PalleteCountX 12
#define PalleteCountY 6
#define PalleteSize 27
#define TypePalleteCount 3
#define ObjectPalletCountX 6
#define ObjectPalletCountY 3

//#define DEBUG

#define SafeDelete(p)\
{\
	if(p)\
	{\
		delete (p);\
		(p) = NULL;\
	}\
}

#define Singleton(ClassName)\
static ClassName* instance;\
public:\
	static ClassName* GetInstance()\
	{\
		if (instance == NULL)\
		{\
			instance = new ClassName();\
		}\
		return instance;\
	}\
	static void ReleaseInstance()\
	{\
		if (instance != NULL)\
		{\
			delete instance;\
			instance = NULL;\
		}\
	}

#define Singleton_NULL(ClassName)\
ClassName* ClassName::instance = NULL;

#define Resources(Path) (wstring(L"../Resources/").append(Path).append(L".bmp"))

#define Synthesize(VarType,VarName,FuncName)\
protected: VarType VarName;\
public: inline VarType Get##FuncName(void)const{return VarName;}\
public: inline void Set##FuncName(VarType value){VarName = value;}
