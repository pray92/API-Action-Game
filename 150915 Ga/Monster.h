#pragma once
#include "obj.h"
class CMonster :
	public CObj
{
private:
	float			m_fSpeed;	
	FRAME			m_tFrame;
	DWORD			m_dwTime;

	DWORD			m_dwAtk;
private:
	void Attack();
private:
	void Frame(void);
	void Direction(void);
	void Motion(void);
	void SetScene(int _iLast,int _iScene,DWORD _dwTime);
public:
	void Initialize(void);
	int Progress(void);
	void Render(HDC hdc);
	void Release(void);
public:
	CMonster(void);
	virtual ~CMonster(void);
};

