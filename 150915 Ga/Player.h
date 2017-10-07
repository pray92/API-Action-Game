#pragma once
#include "obj.h"

class CPlayer :
	public CObj
{
private:
	int			m_temp;							//캐릭터 좌표 재설정
	float		m_fDistance;					//캐릭터 이동 거리 설정
	POINTFLOAT	m_ptMouse;						//이동 좌표
	float		m_fSpeed;

	DWORD		m_dwKey;						//키 입력
	DWORD		m_dwTime;
	
	DWORD		m_dwRecoveryMP;					//기력 회복 딜레이
	DWORD		m_dwRecoveryHP;					//체력 회복 딜레이
	DWORD		m_dwDash;						//대시
	DWORD		m_dwAtk;						//공격
	DWORD		m_dwStk1;						//강타1
	DWORD		m_dwStk2;						//강타2
	DWORD		m_dwStk3;						//강타3
	DWORD		m_dwStk4;						//강타4
	DWORD		m_dwStk5;						//강타5
	int			m_iCombo;						//콤보
	DWORD		m_dwLastPressed;				//콤보 무효화

	FRAME	m_tFrame;

	list<CObj*>* m_pBulletList;					//공격 범위
private:
	void Frame(void);
	void Direction(void);
	void Motion(void);
	void SetScene(int _iLast,int _iScene,DWORD _dwTime);
	void CheckKey();
private:
	void RecoverMP();
	void RecoverHP();
	void SkillMove(int _iTemp);								//특수 스킬 입력 시 이동
	void Scroll();
	void Attack();											//공격
	void Strike1();											//강타1
	void Strike2();											//강타2
	void Strike3();											//강타3
	void Strike4();											//강타4
	void Strike5();											//강타5
public:
	void SetBulletList(list<CObj*>* _pBulletList)
	{
		m_pBulletList = _pBulletList;
	}
public:
	void Initialize(void);
	int Progress(void);
	void Render(HDC hdc);
	void Release(void);
public:
	CPlayer(void);
	virtual ~CPlayer(void);
};


