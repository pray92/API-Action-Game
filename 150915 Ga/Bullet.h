#pragma once
#include "obj.h"
class CBullet :
	public CObj
{
private:
	float			m_fAngle;						//각도
	float			m_fSpeed;						//속도
	DWORD			m_dwDuration;					//지속 시간

	FRAME			m_tFrame;
	DWORD			m_dwTime;
	
	list<CObj*>*	m_pPlayerList;
private:
	void Frame();
	void SetScene(int _iLast,int _iScene,DWORD _dwTime);
	void Motion();
public:
	void SetBullet(string _strDrawID, float _fX, float _fY, float _fCX, float _fCY, 
					float _fAngle, float _fSpeed, int _iDamage, DWORD _dwDuration);
	void SetPlayerList(list<CObj*>*	_pPlayerList)
	{
		m_pPlayerList = _pPlayerList;
	}
	list<CObj*>* GetPlayerList()
	{
		return m_pPlayerList;
	}
public:
	virtual void Initialize(void);
	virtual int Progress(void);
	virtual void Render(HDC hdc);
	virtual void Release(void);
public:
	CBullet(void);
	virtual ~CBullet(void);
};

