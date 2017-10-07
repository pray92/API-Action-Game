#pragma once
#include "obj.h"
class CBoss :
	public CObj
{
private:
	FRAME			m_tFrame;
	DWORD			m_dwTime;

	DWORD			m_dwAtk;

	int				m_iCombo;
	bool			m_bFirstPhase;
	bool			m_bSecondPhase;
	bool			m_bThirdPhase;
	int				m_iSummonNumber;
	DWORD			m_dwSummon;
private:
	void Frame(void);
	void Direction(void);
	void Motion(void);
	void SetScene(int _iLast,int _iScene,DWORD _dwTime);
private:
	void Attack();
public:
	void Initialize(void);
	int Progress(void);
	void Render(HDC hdc);
	void Release(void);
public:
	CBoss(void);
	virtual ~CBoss(void);
};

