#pragma once
#include "stateobj.h"

class CObj;
class CMyButton;
class CStage1 :
	public CStateObj
{
private:
	list<CObj*> m_ButtonList;

private:
	CMyButton* CreateButton(string Name,float _x,float _y);
public:
	virtual void Initialize(void);
	virtual void Progress(void);
	virtual void Render(HDC hdc);
	virtual void Release(void);
public:
	CStage1(void);
	virtual ~CStage1(void);
};

