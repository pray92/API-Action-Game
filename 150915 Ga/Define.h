#pragma once
#define WINCX 1280
#define WINCY 800

extern HWND g_hWnd;

#define PI		3.141592


const int TILEX = 13;
const int TILEY = 15;
const int TILECX = 190;
const int TILECY = 94;

typedef struct tagTile
{
	float	fX;
	float	fY;
	float	fCX;
	float	fCY;


	int	    iOption;
	int		iDrawID;
}TILE;

enum OBJID
{
	OBJ_BACKGROUD,
	OBJ_TILE,
	OBJ_PLAYER,
	OBJ_BULLET,
	OBJ_BOSS,
	OBJ_OBSTACLE,
	OBJ_MONSTER,
	OBJ_MONBULLET,
	OBJ_DAMAGED,
	OBJ_END,
};


enum STATEID
{
	SI_LOGO,
	SI_STAGE1,
	SI_STAGE2,
};

typedef struct tagInfo
{
	float	fX;
	float	fY;
	float	fCX;
	float	fCY;
	
	int		iCurHP;
	int		iMaxHP;	
	int		iCurMP;
	int		iMaxMP;
	int		iDamage;

	bool	bDamaged;
	DWORD	dwDamaged;

	DWORD	dwDeadRemain;
}INFO;

typedef struct tagLine
{
	POINTFLOAT tLPoint;
	POINTFLOAT tRPoint;

	tagLine(float LPX, float LPY, float RPX, float RPY)
	{
		tLPoint.x = LPX;
		tLPoint.y = LPY;
		tRPoint.x = RPX;
		tRPoint.y = RPY;
	}

}LINE;

template <typename T>
inline void SAFE_DELETE(T& Temp)
{
	if(Temp)
	{
		delete Temp;
		Temp = NULL;
	}
}

typedef struct tagFrame
{
	int		iStart;
	int		iLast;
	int		iScene;
	DWORD	dwTime;

	tagFrame(){}
	tagFrame(int		_iStart,
			 int		_iLast,
			 int		_iScene,
			 DWORD	_dwTime)
	{
		iStart  = _iStart;
		iLast	= _iLast;
		iScene	= _iScene;
		dwTime	= _dwTime;
	}
}FRAME;

#define DECLARE_SINGLETON(type)\
	static type** GetInstance(void)\
	{\
		static type*	pInstance = new type;\
		if(pInstance == NULL)\
			pInstance = new type;\
		return &pInstance;\
	}\
	static void DestroyInstance(void)\
	{\
		type**	ppInstance = GetInstance();\
		if(*ppInstance != NULL)\
		{\
			delete *ppInstance;\
			*ppInstance = NULL;\
		}\
	}
#define GET_SINGLE(type) (*type::GetInstance())

#define PRESS_KEY(type) (GET_SINGLE(CKeyManager)->PressKey(type))