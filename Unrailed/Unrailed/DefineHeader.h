#pragma once
#define WINSTARTX 50
#define WINSTARTY 50
#define WINSIZEX 1280
#define WINSIZEY 720

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
