#pragma once
#include "stateobj.h"
class CObj;
class CPlayer;
class CBitMap;
class CLineEdit;
class CMapEdit; 
class CStage2 :
	public CStateObj
{
	CMapEdit*  m_pMapEdit;
	list<CObj*>	m_ObjList[OBJ_END];
public:
	virtual void Initialize(void);
	virtual void Progress(void);
	virtual void Render(HDC hdc);
	virtual void Release(void);
public:
	CStage2(void);
	virtual ~CStage2(void);
};

