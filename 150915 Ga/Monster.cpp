#include "StdAfx.h"
#include "Monster.h"
#include "BitMap.h"
#include "MathMgr.h"
#include "Bullet.h"
#include "Device.h"

CMonster::CMonster(void)
{
}


CMonster::~CMonster(void)
{
	Release();
}

void CMonster::Initialize(void)
{
	m_tInfo.fX =  rand() % 1750 + 410;
	m_tInfo.fY = rand() % 900 + 200;


	m_tInfo.fCX = 300;
	m_tInfo.fCY = 300;

	/* 기본 스테이터스 */
	m_fSpeed = 3.f;
	m_tInfo.iCurHP = 1000;
	m_tInfo.iMaxHP = m_tInfo.iCurHP;
	m_tInfo.iCurMP = 0;
	m_tInfo.iMaxMP = 100;
	m_tInfo.iDamage = 10;
	m_tInfo.dwDamaged = GetTickCount();
	m_tInfo.bDamaged = false;

	m_dwAtk = GetTickCount() + 2000;
	m_dwTime = 0;
	m_dwState = STATE_STAND;
}

void CMonster::Attack()
{
	
	list<CObj*>::iterator PlayerInfo = m_pPlayerList->begin();
	if((*PlayerInfo)->GetState() != STATE_DEAD)
	{
		/* 일반 공격 */
		if(m_dwAtk < GetTickCount())
		{
			CDevice::GetInstance()->SoundPlay(4,0);
			m_dwAtk = GetTickCount() + 2000;	
			m_dwState = STATE_ATTACK;
			CBullet* TempObj = new CBullet;
			TempObj->SetBullet("", m_tInfo.fX, m_tInfo.fY, 500, 500, m_fAngle, 0, m_tInfo.iDamage, 0);
			m_pBulletList->push_back(TempObj);
		}
	}
}

int CMonster::Progress(void)
{
	if(m_tInfo.iCurHP <= 0 && m_dwState != STATE_DEAD)
	{
		m_dwState = STATE_DEAD;
		m_tInfo.dwDeadRemain = GetTickCount() + 900;
	}
	if(m_dwState != STATE_DEAD)
	{
		if(!m_tInfo.bDamaged)
		{
			m_fAngle = CMathMgr::GetAngle(*m_pPlayerList->begin(),this);		
			if(CMathMgr::GetDistance(this, *m_pPlayerList->begin()) <= 200)	
			{
				Attack();
			}
			else
			{
				m_dwState = STATE_MOVE;
				m_tInfo.fX += cosf(m_fAngle * PI / 180) * m_fSpeed;
				m_tInfo.fY += -sinf(m_fAngle * PI / 180) * m_fSpeed;
			}
			Frame();
			Motion();
			Direction();
		}
		else
		{
			if(m_tInfo.dwDamaged < GetTickCount())
				m_tInfo.bDamaged = false;
		}
		BoundaryCheck();
		return 0;
	}
	if(m_dwState == STATE_DEAD)
	{
		if(m_tInfo.dwDeadRemain < GetTickCount())
		{
			return -1;
		}
		Motion();
	}
}

void CMonster::Render(HDC hdc)
{
	TCHAR str[50] = L"";

	TransparentBlt(hdc,
		(int)(m_tInfo.fX - m_tInfo.fCX / 2)  + m_tScroll.x,
		(int)(m_tInfo.fY - m_tInfo.fCY / 2) + m_tScroll.y,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		(*m_pMapBmp)[m_strDrawID]->GetMemDC(),
		(int)(m_tInfo.fCX * m_tFrame.iStart),
		(int)(m_tInfo.fCY * m_tFrame.iScene),
		(int)m_tInfo.fCX,(int)m_tInfo.fCY,
		RGB(0,0,0));
	
	float fPerHP = (float)m_tInfo.iCurHP / m_tInfo.iMaxHP;
	float fPerMP = (float)m_tInfo.iCurMP / m_tInfo.iMaxMP;

	/* 상태 바 */
	TransparentBlt(hdc,
		(int)(m_tInfo.fX - 50) + m_tScroll.x,
		(int)(m_tInfo.fY - 10) + 100 + m_tScroll.y,
		100,
		15,
		(*m_pMapBmp)["Bar_BG"]->GetMemDC(),
		0,0,
		100,15,
		RGB(255,255,255));	
	TransparentBlt(hdc,
		(int)(m_tInfo.fX - 50) + m_tScroll.x,
		(int)(m_tInfo.fY - 10) + 100 + m_tScroll.y,
		100 * fPerHP,
		10,
		(*m_pMapBmp)["Bar_HP"]->GetMemDC(),
		0,0,
		100, 10,
		RGB(255,255,255));	
	TransparentBlt(hdc,
		(int)(m_tInfo.fX - 50) + m_tScroll.x,
		(int)(m_tInfo.fY - 10) + 110 + m_tScroll.y,
		100 * fPerMP,
		5,
		(*m_pMapBmp)["Bar_MP"]->GetMemDC(),
		0,0,
		100,5,
		RGB(255,255,255));	

	if(GetAsyncKeyState(VK_SHIFT) & 0x8000)
	{
		wsprintf(str, L"HP : %d Angle : %d", (int)m_tInfo.iCurHP, (int)m_fAngle);
		TextOut(hdc, (int)m_tInfo.fX - 22 + m_tScroll.x, (int)m_tInfo.fY - 40 + m_tScroll.y, str, lstrlen(str));
	}
}

void CMonster::Motion(void)
{
	
	/* 대기 모션 */
	if(m_dwState  & STATE_STAND)
	{
		SetScene(5,0,100);
	}
	/* 이동 모션 */
	else if(m_dwState  & STATE_MOVE)
	{
		SetScene(9,1,100);
	}
	/* 공격 모션 */
	else if(m_dwState  & STATE_ATTACK)
	{
		SetScene(5,2,50);
	}
	/* 사망 모션 */
	else if(m_dwState  & STATE_DEAD)
	{
		SetScene(2,4,100);
	}
	/* 피격 모션 */
	if(m_tInfo.bDamaged == true)
	{
		SetScene(0,3,300);
	}
}

void CMonster::Frame(void)
{
	if(m_dwTime + m_tFrame.dwTime < GetTickCount())
	{
		m_dwTime = GetTickCount();
		++m_tFrame.iStart;

		if(m_tFrame.iStart >= m_tFrame.iLast )
		{
			m_tFrame.iStart = 0;
			if(m_dwState != STATE_STAND)
				m_dwState = STATE_STAND;
		}
	}
}

void CMonster::Direction(void)
{
	if(0 <= m_fAngle && m_fAngle < 90)
	{
		m_strDrawID =  "Monster_RU";
	}
	else if(90 <= m_fAngle && m_fAngle < 180)
	{
		m_strDrawID =  "Monster_LU";
	}
	else if(180 <= m_fAngle && m_fAngle < 270)
	{
		m_strDrawID =  "Monster_LD";
	}
	else if(270 <= m_fAngle && m_fAngle < 360)
	{
		m_strDrawID =  "Monster_RD";
	}
}

void CMonster::SetScene(int _iLast,int _iScene,DWORD _dwTime)
{
	if(m_tFrame.iScene != _iScene)
		m_tFrame.iStart = 0;

	m_tFrame.iLast =  _iLast;
	m_tFrame.iScene = _iScene;
	m_tFrame.dwTime = _dwTime;
}

void CMonster::Release(void)
{
}