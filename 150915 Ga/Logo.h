#pragma once
#include "stateobj.h"
class CLogo :
	public CStateObj
{
private:
	HWND m_hVideo;
	DWORD dwTime;
public:
	virtual void Initialize(void);
	virtual void Progress(void);
	virtual void Render(HDC hdc);
	virtual void Release(void);
public:
	CLogo(void);
	virtual ~CLogo(void);
};

