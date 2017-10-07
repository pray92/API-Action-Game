#pragma once
#include "obj.h"
class CDamaged :
	public CObj
{
private:	
	DWORD			m_dwDuration;					//지속 시간
	FRAME			m_tFrame;
	DWORD			m_dwTime;
private:
	void Frame();
	void SetScene(int _iLast,int _iScene,DWORD _dwTime);
public:
	virtual void Initialize(void);
	virtual int Progress(void);
	virtual void Render(HDC hdc);
	virtual void Release(void);
public:
	CDamaged(void);
	virtual ~CDamaged(void);
};

