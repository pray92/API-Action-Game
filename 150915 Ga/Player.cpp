#include "StdAfx.h"
#include "Player.h"
#include "BitMap.h"
#include "KeyManager.h"
#include "MathMgr.h"
#include "Mouse.h"
#include "Bullet.h"
#include "Device.h"

CPlayer::CPlayer(void)
{
}


CPlayer::~CPlayer(void)
{
	Release();
}

void CPlayer::Initialize(void)
{
	/* 위치 및 크기 */
	m_tInfo.fX = 900;
	m_tInfo.fY = 400;
	m_tInfo.fCX = 300; 
	m_tInfo.fCY = 300;
	
	/* 상태 및 속도 */
	m_dwState = STATE_STAND;

	m_fSpeed = 5.f;
	m_dwKey = 0;
	m_dwTime = GetTickCount();

	m_strDrawID = "Player_RD";

	m_temp = 34;

	m_dwDash = 0;
	m_dwAtk = 0;
	m_dwRecoveryMP = GetTickCount();
	m_dwRecoveryHP = GetTickCount();

	m_dwStk1 = 0;
	m_dwStk2 = 0;
	m_dwStk3 = 0;
	m_dwStk4 = 0;
	m_dwStk5 = 0;

	m_iCombo = 1;
	m_iCombo = 10;

	/* 기본 스테이터스 */
	
	m_tInfo.iCurHP = 200;
	m_tInfo.iMaxHP = m_tInfo.iCurHP;
	
	m_tInfo.iCurMP = 120;
	m_tInfo.iMaxMP = m_tInfo.iCurMP;

	m_tInfo.iDamage = 50;
}

int CPlayer::Progress(void)
{
	if(m_tInfo.iCurHP <= 0 && m_dwState != STATE_DEAD)
	{
		CDevice::GetInstance()->SoundStop(7);
		CDevice::GetInstance()->SoundPlay(8,1);
		m_dwState = STATE_DEAD;
		m_tInfo.dwDeadRemain = GetTickCount() + 5000;
	}
	if(m_dwState != STATE_DEAD)
	{
		Scroll();
		CheckKey();
		Motion();
		Frame();
		Direction();
		RecoverMP();
		RecoverHP();
		return 0;
	}
	if(m_dwState == STATE_DEAD)
	{
		if(m_tInfo.dwDeadRemain < GetTickCount())
		{
			if(GetAsyncKeyState(VK_ESCAPE))
			{
				CDevice::GetInstance()->SoundStop(8);
				CDevice::GetInstance()->SoundPlay(7,1);
				Initialize();
			}
		}
		Motion();
	}
}

//void CPlayer::Move()
//{
//	if(m_fDistance >= 1)
//	{
//		m_tInfo.fX += cosf(m_fAngle *  PI / 180) * m_fSpeed;
//		m_tInfo.fY += -sinf(m_fAngle *  PI / 180) * m_fSpeed;
//		m_fDistance -= (sqrt(pow(cosf(m_fAngle *  PI / 180) * m_fSpeed, 2) +
//							pow(sinf(m_fAngle *  PI / 180) * m_fSpeed, 2)))*2.f; 
//	}
//}

void CPlayer::Render(HDC hdc)
{
	TCHAR str[128] = L"";

	TransparentBlt(hdc,
		(int)(m_tInfo.fX - m_tInfo.fCX /2) + m_tScroll.x,
		(int)(m_tInfo.fY - m_tInfo.fCY /2) + m_temp + m_tScroll.y,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		(*m_pMapBmp)[m_strDrawID]->GetMemDC(),
		(int)(m_tInfo.fCX * m_tFrame.iStart),
		(int)(m_tInfo.fCY * m_tFrame.iScene),
		(int)m_tInfo.fCX,(int)m_tInfo.fCY,
		RGB(255,255,255));	
	
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

	//MoveToEx(hdc,m_tInfo.fX+ m_tScroll.x,m_tInfo.fY + m_temp + m_tScroll.y,NULL);
	//float fx = m_tInfo.fX + cosf(m_fAngle *  PI / 180) * 50 + m_tScroll.x;
	//float fy = m_tInfo.fY + -sinf(m_fAngle * PI / 180) * 50 + m_temp + m_tScroll.y;
	//LineTo(hdc,fx,fy);

	if(GetAsyncKeyState(VK_SHIFT) & 0x8000)
	{
		
		wsprintf(str, L"HP : %d / %d MP : %d / %d", (int)m_tInfo.iCurHP, (int)m_tInfo.iMaxHP,(int)m_tInfo.iCurMP, (int)m_tInfo.iMaxMP);
		//wsprintf(str, L"x : %d y : %d Offset.x : %d Offset.y : %d  Scroll.x : %d Scroll.y : %d  iStart : %d", (int)m_tInfo.fX, (int)m_tInfo.fY,(int)m_tOffset.x, (int)m_tOffset.y,(int)m_tScroll.x, (int)m_tScroll.y,(int)m_tFrame.iStart);
		TextOut(hdc, (int)m_tInfo.fX - 22 + m_tScroll.x, (int)m_tInfo.fY - 40 + m_tScroll.y, str, lstrlen(str));
	}
}
void CPlayer::Release(void)
{
}

void CPlayer::RecoverHP()
{
	if(0 < m_tInfo.iCurHP && m_tInfo.iCurHP < m_tInfo.iMaxHP)
	{
		if(m_dwRecoveryHP < GetTickCount())
		{
			m_dwRecoveryHP = GetTickCount() + 5000;
			m_tInfo.iCurHP += 100;
			if(m_tInfo.iCurHP > m_tInfo.iMaxHP)
				m_tInfo.iCurHP = m_tInfo.iMaxHP;
		}
	}
}
void CPlayer::RecoverMP()
{
	if(m_dwRecoveryMP < GetTickCount())
	{
		if(m_tInfo.iCurMP < m_tInfo.iMaxMP)
		{
			m_tInfo.iCurMP += 5;
			if(m_tInfo.iCurMP > m_tInfo.iMaxMP)
			{
				m_tInfo.iCurMP = m_tInfo.iMaxMP;
			}
		}
	}
}

void CPlayer::SkillMove(int _iTemp)
{
	if(m_strDrawID == "Player_RIGHT")
	{
		if(410 <= m_tInfo.fX && m_tInfo.fX <= 1750)
		{
			m_tInfo.fX += m_fSpeed	* _iTemp;
			if(_iTemp >= 0)
			{
				m_tScroll.x -= m_fSpeed * (_iTemp - 1);
				m_tOffset.x += m_fSpeed * (_iTemp - 1);
			}
			else
			{
				m_tScroll.x -= m_fSpeed * (_iTemp + 1);
				m_tOffset.x += m_fSpeed * (_iTemp + 1);
			}
		}
	}
	if(m_strDrawID == "Player_RU")
	{
		if(410 <= m_tInfo.fX && m_tInfo.fX <= 1750)
		{			
			m_tInfo.fX += m_fSpeed	* _iTemp;
			if(_iTemp >= 0)
			{
				m_tScroll.x -= m_fSpeed * (_iTemp - 1);
				m_tOffset.x += m_fSpeed * (_iTemp - 1);
			}
			else
			{
				m_tScroll.x -= m_fSpeed * (_iTemp + 1);
				m_tOffset.x += m_fSpeed * (_iTemp + 1);
			}
		}

		if(200 <= m_tInfo.fY && m_tInfo.fY <= 900)
		{
			m_tInfo.fY -= m_fSpeed	* _iTemp;
			if(_iTemp >= 0)
			{
				m_tScroll.y += m_fSpeed * (_iTemp - 1);
				m_tOffset.y -= m_fSpeed * (_iTemp - 1);
			}
			else
			{		
				m_tScroll.y += m_fSpeed * (_iTemp + 1);
				m_tOffset.y -= m_fSpeed * (_iTemp + 1);
			}
		}
	}
	if(m_strDrawID == "Player_UP")
	{
		if(200 <= m_tInfo.fY && m_tInfo.fY <= 900)
		{
			m_tInfo.fY -= m_fSpeed	* _iTemp;
			if(_iTemp >= 0)
			{
				m_tScroll.y += m_fSpeed * (_iTemp - 1);
				m_tOffset.y -= m_fSpeed * (_iTemp - 1);
			}
			else
			{		
				m_tScroll.y += m_fSpeed * (_iTemp + 1);
				m_tOffset.y -= m_fSpeed * (_iTemp + 1);
			}
		}
	}
	if(m_strDrawID == "Player_LU")
	{			
		if(410 <= m_tInfo.fX && m_tInfo.fX <= 1750)
		{
			m_tInfo.fX -= m_fSpeed	* _iTemp;
			if(_iTemp >= 0)
			{
				m_tScroll.x += m_fSpeed * (_iTemp - 1);
				m_tOffset.x -= m_fSpeed * (_iTemp - 1);
			}
			else
			{
				m_tScroll.x += m_fSpeed * (_iTemp + 1);
				m_tOffset.x -= m_fSpeed * (_iTemp + 1);
			}
		}

		if(200 <= m_tInfo.fY && m_tInfo.fY <= 900)
		{
			m_tInfo.fY -= m_fSpeed	* _iTemp;
			if(_iTemp >= 0)
			{
				m_tScroll.y += m_fSpeed * (_iTemp - 1);
				m_tOffset.y -= m_fSpeed * (_iTemp - 1);
			}
			else
			{		
				m_tScroll.y += m_fSpeed * (_iTemp + 1);
				m_tOffset.y -= m_fSpeed * (_iTemp + 1);
			}
		}
	}
	if(m_strDrawID == "Player_LEFT")
	{
		if(410 <= m_tInfo.fX && m_tInfo.fX <= 1750)
		{
			m_tInfo.fX -= m_fSpeed	* _iTemp;
			if(_iTemp >= 0)
			{
				m_tScroll.x += m_fSpeed * (_iTemp - 1);
				m_tOffset.x -= m_fSpeed * (_iTemp - 1);
			}
			else
			{
				m_tScroll.x += m_fSpeed * (_iTemp + 1);
				m_tOffset.x -= m_fSpeed * (_iTemp + 1);
			}
		}
	}
	if(m_strDrawID == "Player_LD")
	{
		if(410 <= m_tInfo.fX && m_tInfo.fX <= 1750)
		{
			m_tInfo.fX -= m_fSpeed	* _iTemp;
			if(_iTemp >= 0)
			{
				m_tScroll.x += m_fSpeed * (_iTemp - 1);
				m_tOffset.x -= m_fSpeed * (_iTemp - 1);
			}
			else
			{
				m_tScroll.x += m_fSpeed * (_iTemp + 1);
				m_tOffset.x -= m_fSpeed * (_iTemp + 1);
			}
		}
		if(200 <= m_tInfo.fY && m_tInfo.fY <= 900)
		{
			m_tInfo.fY += m_fSpeed	* _iTemp;
			if(_iTemp >= 0)
			{
				m_tScroll.y -= m_fSpeed * (_iTemp - 1);
				m_tOffset.y += m_fSpeed * (_iTemp - 1);
			}
			else
			{
				m_tScroll.y -= m_fSpeed * (_iTemp + 1);
				m_tOffset.y += m_fSpeed * (_iTemp + 1);
			}
		}
	}
	if(m_strDrawID == "Player_DOWN")
	{
		if(200 <= m_tInfo.fY && m_tInfo.fY <= 900)
		{
			m_tInfo.fY += m_fSpeed	* _iTemp;
			if(_iTemp >= 0)
			{
				m_tScroll.y -= m_fSpeed * (_iTemp - 1);
				m_tOffset.y += m_fSpeed * (_iTemp - 1);
			}
			else
			{
				m_tScroll.y -= m_fSpeed * (_iTemp + 1);
				m_tOffset.y += m_fSpeed * (_iTemp + 1);
			}
		}
	}
	if(m_strDrawID == "Player_RD")
	{
		if(410 <= m_tInfo.fX && m_tInfo.fX <= 1750)
		{
			m_tInfo.fX += m_fSpeed	* _iTemp;
			if(_iTemp >= 0)
			{
				m_tScroll.x -= m_fSpeed * (_iTemp - 1);
				m_tOffset.x += m_fSpeed * (_iTemp - 1);
			}
			else
			{
				m_tScroll.x -= m_fSpeed * (_iTemp + 1);
				m_tOffset.x += m_fSpeed * (_iTemp + 1);
			}
		}

		if(200 <= m_tInfo.fY && m_tInfo.fY <= 900)
		{
			m_tInfo.fY += m_fSpeed	* _iTemp;
			if(_iTemp >= 0)
			{
				m_tScroll.y -= m_fSpeed * (_iTemp - 1);
				m_tOffset.y += m_fSpeed * (_iTemp - 1);
			}
			else
			{
				m_tScroll.y -= m_fSpeed * (_iTemp + 1);
				m_tOffset.y += m_fSpeed * (_iTemp + 1);
			}
		}
	}
}

void CPlayer::Attack()
{
	if(CDevice::GetInstance()->SoundPlaying(0) == false)
			CDevice::GetInstance()->SoundPlay(0,1);
	if(m_strDrawID == "Player_RIGHT")
	{
		CBullet* TempObj = new CBullet;
		TempObj->SetBullet("", m_tInfo.fX + 80, m_tInfo.fY + m_temp, 160, 160, 0.f, 0, m_tInfo.iDamage, 0);
		m_pBulletList->push_back(TempObj);
	}
	if(m_strDrawID == "Player_RU")
	{
		CBullet* TempObj = new CBullet;
		TempObj->SetBullet("", m_tInfo.fX + 80, m_tInfo.fY + m_temp - 80, 160, 160, 45.f, 0, m_tInfo.iDamage, 0);
		m_pBulletList->push_back(TempObj);
	}
	if(m_strDrawID == "Player_UP")
	{
		CBullet* TempObj = new CBullet;
		TempObj->SetBullet("", m_tInfo.fX, m_tInfo.fY + m_temp - 80, 160, 160, 90.f, 0, m_tInfo.iDamage, 0);
		m_pBulletList->push_back(TempObj);
	}
	if(m_strDrawID == "Player_LU")
	{			
		CBullet* TempObj = new CBullet;
		TempObj->SetBullet("", m_tInfo.fX - 80, m_tInfo.fY + m_temp - 80, 160, 160, 135.f, 0, m_tInfo.iDamage, 0);
		m_pBulletList->push_back(TempObj);
	}
	if(m_strDrawID == "Player_LEFT")
	{
		CBullet* TempObj = new CBullet;
		TempObj->SetBullet("", m_tInfo.fX - 80, m_tInfo.fY + m_temp, 160, 160, 180.f, 0, m_tInfo.iDamage, 0);
		m_pBulletList->push_back(TempObj);
	}
	if(m_strDrawID == "Player_LD")
	{
		CBullet* TempObj = new CBullet;
		TempObj->SetBullet("", m_tInfo.fX - 80, m_tInfo.fY + m_temp + 80, 160, 160, 225.f, 0, m_tInfo.iDamage, 0);
		m_pBulletList->push_back(TempObj);
	}
	if(m_strDrawID == "Player_DOWN")
	{
		CBullet* TempObj = new CBullet;
		TempObj->SetBullet("", m_tInfo.fX, m_tInfo.fY + m_temp + 80, 160, 160, 270.f, 0, m_tInfo.iDamage, 0);
		m_pBulletList->push_back(TempObj);

	}
	if(m_strDrawID == "Player_RD")
	{
		CBullet* TempObj = new CBullet;
		TempObj->SetBullet("", m_tInfo.fX + 80, m_tInfo.fY + m_temp + 80, 160, 160, 315.f, 0, m_tInfo.iDamage, 0);
		m_pBulletList->push_back(TempObj);
	}
}

void CPlayer::Strike1()
{
	CBullet* TempObj = new CBullet;
	TempObj->SetBullet("Stk1", m_tInfo.fX, m_tInfo.fY, 200, 200, 0.f, 0, m_tInfo.iDamage, 250);
	m_pBulletList->push_back(TempObj);
}

void CPlayer::Strike2()
{
	//CDevice::GetInstance()->SoundPlay(8,0);
	if(m_strDrawID == "Player_RIGHT")
	{
		CBullet* TempObj1 = new CBullet;
		TempObj1->SetBullet("Stk2", m_tInfo.fX + 110, m_tInfo.fY + m_temp, 100, 100, 0.f, 0, m_tInfo.iDamage * 2, 250);
		CBullet* TempObj2 = new CBullet;
		TempObj2->SetBullet("Stk2", m_tInfo.fX + 110 + 130 * 1, m_tInfo.fY + m_temp, 100, 100, 0.f, 0, m_tInfo.iDamage * 2, 250);
		CBullet* TempObj3 = new CBullet;
		TempObj3->SetBullet("Stk2", m_tInfo.fX + 110 + 130 * 2, m_tInfo.fY + m_temp, 100, 100, 0.f, 0, m_tInfo.iDamage * 2, 250);
		CBullet* TempObj4 = new CBullet;
		TempObj4->SetBullet("Stk2", m_tInfo.fX + 110 + 130 * 3, m_tInfo.fY + m_temp, 100, 100, 0.f, 0, m_tInfo.iDamage * 2, 250);

		m_pBulletList->push_back(TempObj1);
		m_pBulletList->push_back(TempObj2);
		m_pBulletList->push_back(TempObj3);
		m_pBulletList->push_back(TempObj4);
	}
	if(m_strDrawID == "Player_RU")
	{
		CBullet* TempObj1 = new CBullet;
		TempObj1->SetBullet("Stk2", m_tInfo.fX + 110, m_tInfo.fY + m_temp - 110, 100, 100, 0.f, 0, m_tInfo.iDamage * 2, 250);
		CBullet* TempObj2 = new CBullet;
		TempObj2->SetBullet("Stk2", m_tInfo.fX + 210, m_tInfo.fY + m_temp - 210, 100, 100, 0.f, 0, m_tInfo.iDamage * 2, 250);
		CBullet* TempObj3 = new CBullet;
		TempObj3->SetBullet("Stk2", m_tInfo.fX + 310, m_tInfo.fY + m_temp - 310, 100, 100, 0.f, 0, m_tInfo.iDamage * 2, 250);
		CBullet* TempObj4 = new CBullet;
		TempObj4->SetBullet("Stk2", m_tInfo.fX + 410, m_tInfo.fY + m_temp - 410, 100, 100, 0.f, 0, m_tInfo.iDamage * 2, 250);

		m_pBulletList->push_back(TempObj1);
		m_pBulletList->push_back(TempObj2);
		m_pBulletList->push_back(TempObj3);
		m_pBulletList->push_back(TempObj4);
	}
	if(m_strDrawID == "Player_UP")
	{
		CBullet* TempObj1 = new CBullet;
		TempObj1->SetBullet("Stk2", m_tInfo.fX, m_tInfo.fY + m_temp - 110, 100, 100, 0.f, 0, m_tInfo.iDamage * 2, 250);
		CBullet* TempObj2 = new CBullet;
		TempObj2->SetBullet("Stk2", m_tInfo.fX, m_tInfo.fY + m_temp - 110 - 130 * 1, 100, 100, 0.f, 0, m_tInfo.iDamage * 2, 250);
		CBullet* TempObj3 = new CBullet;
		TempObj3->SetBullet("Stk2", m_tInfo.fX, m_tInfo.fY + m_temp - 110 - 130  * 2, 100, 100, 0.f, 0, m_tInfo.iDamage * 2, 250);
		CBullet* TempObj4 = new CBullet;
		TempObj4->SetBullet("Stk2", m_tInfo.fX, m_tInfo.fY + m_temp - 110 - 130  * 3, 100, 100, 0.f, 0, m_tInfo.iDamage * 2, 250);

		m_pBulletList->push_back(TempObj1);
		m_pBulletList->push_back(TempObj2);
		m_pBulletList->push_back(TempObj3);
		m_pBulletList->push_back(TempObj4);
	}
	if(m_strDrawID == "Player_LU")
	{			
		CBullet* TempObj1 = new CBullet;
		TempObj1->SetBullet("Stk2", m_tInfo.fX - 110, m_tInfo.fY + m_temp - 110, 100, 100, 0.f, 0, m_tInfo.iDamage * 2, 250);
		CBullet* TempObj2 = new CBullet;
		TempObj2->SetBullet("Stk2", m_tInfo.fX - 210, m_tInfo.fY + m_temp - 210, 100, 100, 0.f, 0, m_tInfo.iDamage * 2, 250);
		CBullet* TempObj3 = new CBullet;
		TempObj3->SetBullet("Stk2", m_tInfo.fX - 310, m_tInfo.fY + m_temp - 310, 100, 100, 0.f, 0, m_tInfo.iDamage * 2, 250);
		CBullet* TempObj4 = new CBullet;
		TempObj4->SetBullet("Stk2", m_tInfo.fX - 410, m_tInfo.fY + m_temp - 410, 100, 100, 0.f, 0, m_tInfo.iDamage * 2, 250);

		m_pBulletList->push_back(TempObj1);
		m_pBulletList->push_back(TempObj2);
		m_pBulletList->push_back(TempObj3);
		m_pBulletList->push_back(TempObj4);
	}
	if(m_strDrawID == "Player_LEFT")
	{
		CBullet* TempObj1 = new CBullet;
		TempObj1->SetBullet("Stk2", m_tInfo.fX - 110, m_tInfo.fY + m_temp, 100, 100, 0.f, 0, m_tInfo.iDamage * 2, 250);
		CBullet* TempObj2 = new CBullet;
		TempObj2->SetBullet("Stk2", m_tInfo.fX - 110 - 130 * 1, m_tInfo.fY + m_temp, 100, 100, 0.f, 0, m_tInfo.iDamage * 2, 250);
		CBullet* TempObj3 = new CBullet;
		TempObj3->SetBullet("Stk2", m_tInfo.fX - 110 - 130 * 2, m_tInfo.fY + m_temp, 100, 100, 0.f, 0, m_tInfo.iDamage * 2, 250);
		CBullet* TempObj4 = new CBullet;
		TempObj4->SetBullet("Stk2", m_tInfo.fX - 110 - 130 * 3, m_tInfo.fY + m_temp, 100, 100, 0.f, 0, m_tInfo.iDamage * 2, 250);

		m_pBulletList->push_back(TempObj1);
		m_pBulletList->push_back(TempObj2);
		m_pBulletList->push_back(TempObj3);
		m_pBulletList->push_back(TempObj4);
	}
	if(m_strDrawID == "Player_LD")
	{
		CBullet* TempObj1 = new CBullet;
		TempObj1->SetBullet("Stk2", m_tInfo.fX - 110, m_tInfo.fY + m_temp + 110, 100, 100, 0.f, 0, m_tInfo.iDamage * 2, 250);
		CBullet* TempObj2 = new CBullet;
		TempObj2->SetBullet("Stk2", m_tInfo.fX - 210, m_tInfo.fY + m_temp + 210, 100, 100, 0.f, 0, m_tInfo.iDamage * 2, 250);
		CBullet* TempObj3 = new CBullet;
		TempObj3->SetBullet("Stk2", m_tInfo.fX - 310, m_tInfo.fY + m_temp + 310, 100, 100, 0.f, 0, m_tInfo.iDamage * 2, 250);
		CBullet* TempObj4 = new CBullet;
		TempObj4->SetBullet("Stk2", m_tInfo.fX - 410, m_tInfo.fY + m_temp + 410, 100, 100, 0.f, 0, m_tInfo.iDamage * 2, 250);

		m_pBulletList->push_back(TempObj1);
		m_pBulletList->push_back(TempObj2);
		m_pBulletList->push_back(TempObj3);
		m_pBulletList->push_back(TempObj4);
	}
	if(m_strDrawID == "Player_DOWN")
	{
		CBullet* TempObj1 = new CBullet;
		TempObj1->SetBullet("Stk2", m_tInfo.fX, m_tInfo.fY + m_temp + 110, 100, 100, 0.f, 0, m_tInfo.iDamage * 2, 250);
		CBullet* TempObj2 = new CBullet;
		TempObj2->SetBullet("Stk2", m_tInfo.fX, m_tInfo.fY + m_temp + 110 + 130 * 1, 100, 100, 0.f, 0, m_tInfo.iDamage * 2, 250);
		CBullet* TempObj3 = new CBullet;
		TempObj3->SetBullet("Stk2", m_tInfo.fX, m_tInfo.fY + m_temp + 110 + 130 * 2, 100, 100, 0.f, 0, m_tInfo.iDamage * 2, 250);
		CBullet* TempObj4 = new CBullet;
		TempObj4->SetBullet("Stk2", m_tInfo.fX, m_tInfo.fY + m_temp + 110 + 130 * 3, 100, 100, 0.f, 0, m_tInfo.iDamage * 2, 250);

		m_pBulletList->push_back(TempObj1);
		m_pBulletList->push_back(TempObj2);
		m_pBulletList->push_back(TempObj3);
		m_pBulletList->push_back(TempObj4);

	}
	if(m_strDrawID == "Player_RD")
	{
		CBullet* TempObj1 = new CBullet;
		TempObj1->SetBullet("Stk2", m_tInfo.fX + 110, m_tInfo.fY + m_temp + 110, 100, 100, 0.f, 0, m_tInfo.iDamage * 2, 250);
		CBullet* TempObj2 = new CBullet;
		TempObj2->SetBullet("Stk2", m_tInfo.fX + 210, m_tInfo.fY + m_temp + 210, 100, 100, 0.f, 0, m_tInfo.iDamage * 2, 250);
		CBullet* TempObj3 = new CBullet;
		TempObj3->SetBullet("Stk2", m_tInfo.fX + 310, m_tInfo.fY + m_temp + 310, 100, 100, 0.f, 0, m_tInfo.iDamage * 2, 250);
		CBullet* TempObj4 = new CBullet;
		TempObj4->SetBullet("Stk2", m_tInfo.fX + 410, m_tInfo.fY + m_temp + 410, 100, 100, 0.f, 0, m_tInfo.iDamage * 2, 250);

		m_pBulletList->push_back(TempObj1);
		m_pBulletList->push_back(TempObj2);
		m_pBulletList->push_back(TempObj3);
		m_pBulletList->push_back(TempObj4);
	}
}

void CPlayer::Strike3()
{
	if(m_strDrawID == "Player_RIGHT")
	{
		CBullet* TempObj = new CBullet;
		TempObj->SetBullet("Stk3", m_tInfo.fX + 150, m_tInfo.fY + m_temp, 300, 300, 0.f, 0, m_tInfo.iDamage * 3, 250);
		m_pBulletList->push_back(TempObj);
	}
	if(m_strDrawID == "Player_RU")
	{
		CBullet* TempObj = new CBullet;
		TempObj->SetBullet("Stk3", m_tInfo.fX + 150, m_tInfo.fY + m_temp - 150, 300, 300, 45.f, 0, m_tInfo.iDamage * 3, 250);
		m_pBulletList->push_back(TempObj);
	}
	if(m_strDrawID == "Player_UP")
	{
		CBullet* TempObj = new CBullet;
		TempObj->SetBullet("Stk3", m_tInfo.fX, m_tInfo.fY + m_temp - 150, 300, 300, 90.f, 0, m_tInfo.iDamage * 3, 250);
		m_pBulletList->push_back(TempObj);
	}
	if(m_strDrawID == "Player_LU")
	{			
		CBullet* TempObj = new CBullet;
		TempObj->SetBullet("Stk3", m_tInfo.fX - 150, m_tInfo.fY + m_temp - 150, 300, 300, 135.f, 0, m_tInfo.iDamage * 3, 250);
		m_pBulletList->push_back(TempObj);
	}
	if(m_strDrawID == "Player_LEFT")
	{
		CBullet* TempObj = new CBullet;
		TempObj->SetBullet("Stk3", m_tInfo.fX - 150, m_tInfo.fY + m_temp, 300, 300, 180.f, 0, m_tInfo.iDamage * 3, 250);
		m_pBulletList->push_back(TempObj);
	}
	if(m_strDrawID == "Player_LD")
	{
		CBullet* TempObj = new CBullet;
		TempObj->SetBullet("Stk3", m_tInfo.fX - 150, m_tInfo.fY + 150, 300, 300, 225.f, 0, m_tInfo.iDamage * 3, 250);
		m_pBulletList->push_back(TempObj);
	}
	if(m_strDrawID == "Player_DOWN")
	{
		CBullet* TempObj = new CBullet;
		TempObj->SetBullet("Stk3", m_tInfo.fX, m_tInfo.fY + 150, 300, 300, 270.f, 0, m_tInfo.iDamage * 3, 250);
		m_pBulletList->push_back(TempObj);
	}
	if(m_strDrawID == "Player_RD")
	{
		CBullet* TempObj = new CBullet;
		TempObj->SetBullet("Stk3", m_tInfo.fX + 150, m_tInfo.fY + 150, 300, 300, 315.f, 0, m_tInfo.iDamage * 3, 250);
		m_pBulletList->push_back(TempObj);
	}
}

void CPlayer::Strike4()
{
	if(m_strDrawID == "Player_RIGHT")
	{
		CBullet* TempObj = new CBullet;
		TempObj->SetBullet("Stk4_RIGHT", m_tInfo.fX + 100, m_tInfo.fY + m_temp, 200, 200, 0.f, 0, m_tInfo.iDamage * 4, 300);
		m_pBulletList->push_back(TempObj);
	}
	if(m_strDrawID == "Player_RU")
	{
		CBullet* TempObj = new CBullet;
		TempObj->SetBullet("Stk4_RU", m_tInfo.fX + 100, m_tInfo.fY + m_temp - 100, 200, 200, 45.f, 0, m_tInfo.iDamage * 4, 300);
		m_pBulletList->push_back(TempObj);
	}
	if(m_strDrawID == "Player_UP")
	{
		CBullet* TempObj = new CBullet;
		TempObj->SetBullet("Stk4_UP", m_tInfo.fX, m_tInfo.fY + m_temp - 100, 200, 200, 90.f, 0, m_tInfo.iDamage * 4, 300);
		m_pBulletList->push_back(TempObj);
	}
	if(m_strDrawID == "Player_LU")
	{			
		CBullet* TempObj = new CBullet;
		TempObj->SetBullet("Stk4_LU", m_tInfo.fX - 100, m_tInfo.fY + m_temp - 100, 200, 200, 135.f, 0, m_tInfo.iDamage * 4, 300);
		m_pBulletList->push_back(TempObj);
	}
	if(m_strDrawID == "Player_LEFT")
	{
		CBullet* TempObj = new CBullet;
		TempObj->SetBullet("Stk4_LEFT", m_tInfo.fX - 100, m_tInfo.fY + m_temp, 200, 200, 180.f, 0, m_tInfo.iDamage * 4, 300);
		m_pBulletList->push_back(TempObj);
	}
	if(m_strDrawID == "Player_LD")
	{
		CBullet* TempObj = new CBullet;
		TempObj->SetBullet("Stk4_LD", m_tInfo.fX - 100, m_tInfo.fY + m_temp + 100, 200, 200, 225.f, 0, m_tInfo.iDamage * 4, 300);
		m_pBulletList->push_back(TempObj);
	}
	if(m_strDrawID == "Player_DOWN")
	{
		CBullet* TempObj = new CBullet;
		TempObj->SetBullet("Stk4_DOWN", m_tInfo.fX, m_tInfo.fY + m_temp + 100, 200, 200, 270.f, 0, m_tInfo.iDamage * 4, 300);
		m_pBulletList->push_back(TempObj);

	}
	if(m_strDrawID == "Player_RD")
	{
		CBullet* TempObj = new CBullet;
		TempObj->SetBullet("Stk4_RD", m_tInfo.fX + 100, m_tInfo.fY + m_temp + 100, 200, 200, 315.f, 0, m_tInfo.iDamage / 2, 300);
		m_pBulletList->push_back(TempObj);
	}
}

void CPlayer::Strike5()
{
	CBullet* TempObj = new CBullet;
	TempObj->SetBullet("Stk5", m_tInfo.fX, m_tInfo.fY + m_temp, 500, 500, 0.f, 0, m_tInfo.iDamage * 5, 500);
	m_pBulletList->push_back(TempObj);
}

void CPlayer::CheckKey()
{
	m_dwKey = GET_SINGLE(CKeyManager)->GetKey();
	
	/* 마우스 좌클릭을 이용한 이동 */
	//if(m_dwKey & KEY_LBUTTON)
	//{
	//	m_ptMouse = CMouse::GetPosf();		
	//	m_fAngle = CMathMgr::GetPlayerMovingAngle(this, &m_ptMouse);

	//	/*이동 거리 구하기*/
	//	float fWidth = m_ptMouse.x - m_tInfo.fX;
	//	float fHeight = m_ptMouse.y - m_tInfo.fY;
	//	m_fDistance = sqrt(fWidth * fWidth + fHeight * fHeight); 
	//}	
	
	if(	m_dwState != STATE_DASH && 
		m_dwState != STATE_ATTACK1 && m_dwState != STATE_ATTACK2 &&
		m_dwState != STATE_ATTACK3 && m_dwState != STATE_ATTACK4 &&
		m_dwState != STATE_STRIKE1 && m_dwState != STATE_STRIKE2 &&
		m_dwState != STATE_STRIKE3 && m_dwState != STATE_STRIKE4 &&
		m_dwState != STATE_STRIKE5 && m_dwState != STATE_DEAD)
	{
		if( m_dwKey & KEY_UP)
		{
			m_tInfo.fY -= m_fSpeed;
		}

		if( m_dwKey & KEY_DOWN)
		{
			m_tInfo.fY += m_fSpeed;
		}

		if( m_dwKey & KEY_LEFT)
		{
			m_tInfo.fX -= m_fSpeed;
		}

		if( m_dwKey & KEY_RIGHT)
		{
			m_tInfo.fX += m_fSpeed;
		}

		if( m_dwKey & KEY_A)
		{
			if(m_dwDash < GetTickCount())
			{
				if(m_tInfo.iCurMP >= 10)
				{		
					CDevice::GetInstance()->SoundPlay(0,0);
					m_tInfo.iCurMP -= 10;
					/* 기력 회복 불가 */
					m_dwRecoveryMP = GetTickCount() + 500;
					m_dwDash = GetTickCount() + 500;
					m_dwState = STATE_DASH;
					m_tFrame.iStart = 0;
					SkillMove(50);
					/* 착지 지점에 데미지 */
					CBullet* TempObj = new CBullet;
					TempObj->SetBullet("", m_tInfo.fX, m_tInfo.fY + m_temp, 200, 200, 180.f, 0, m_tInfo.iDamage / 2, 0);
					m_pBulletList->push_back(TempObj);
				}
			}
		}
	}

	if(m_dwKey & KEY_S)
	{
		/* 기력 회복 불가 */
		m_dwRecoveryMP = GetTickCount() + 500;
		if(m_tInfo.iCurMP > 0)
		{	
			if(m_iCombo > 5)
				m_iCombo = 1;

			if(m_iCombo == 1)
			{
				if(m_dwAtk < GetTickCount())
				{
					CDevice::GetInstance()->SoundPlay(0,0);
					++m_iCombo;
					m_dwLastPressed = GetTickCount() + 500;
					m_dwAtk = GetTickCount() + 250;
					m_dwState = STATE_ATTACK1;
					m_tFrame.iStart = 0;
					SkillMove(1);
					Attack();
				}
			}
			else if(m_iCombo == 2)
			{
				if(m_dwAtk < GetTickCount())
				{
					CDevice::GetInstance()->SoundPlay(0,0);
					++m_iCombo;
					m_dwLastPressed = GetTickCount() + 500;
					m_dwAtk = GetTickCount() + 250;
					m_dwState = STATE_ATTACK2;
					m_tFrame.iStart = 0;
					SkillMove(1);
					Attack();
				}
			}
			else if(m_iCombo == 3)
			{
				if(m_dwAtk < GetTickCount())
				{
					CDevice::GetInstance()->SoundPlay(0,0);
					++m_iCombo;
					m_dwLastPressed = GetTickCount() + 500;
					m_dwAtk = GetTickCount() + 250;
					m_dwState = STATE_ATTACK3;
					m_tFrame.iStart = 0;
					SkillMove(1);
					Attack();
				}
			}
			else if(m_iCombo == 4)
			{
				if(m_dwAtk < GetTickCount())
				{
					CDevice::GetInstance()->SoundPlay(0,0);
					++m_iCombo;
					m_dwLastPressed = GetTickCount() + 500;
					m_dwAtk = GetTickCount() + 400;
					m_dwState = STATE_ATTACK4;
					m_tFrame.iStart = 0;
					SkillMove(1);
					Attack();
				}
			}
		}
	}
	
	if(m_dwKey & KEY_D)
	{
		/* 기력 회복 불가 */
		m_dwRecoveryMP = GetTickCount() + 2000;
		switch(m_iCombo)
		{
		case 1:			
			if(m_dwStk1 < GetTickCount())
			{
				if(m_tInfo.iCurMP >= 10)
				{	
					CDevice::GetInstance()->SoundPlay(1,0);
					m_tInfo.iCurMP -= 10;
					m_dwState = STATE_STRIKE1;
					m_dwStk1 = GetTickCount() + 500;
					m_tFrame.iStart = 0;
					Strike1();
					SkillMove(-30);
				}
			}
			break;
		case 2:		
			if(m_dwStk2 < GetTickCount())
			{
				if(m_tInfo.iCurMP >= 20)
				{
					CDevice::GetInstance()->SoundPlay(1,0);
					m_tInfo.iCurMP -= 20;
					m_dwState = STATE_STRIKE2;
					m_dwStk2 = GetTickCount() + 500;
					m_dwStk1 = GetTickCount() + 500;
					m_tFrame.iStart = 0;
					Strike2();
				}
			}
			break;
		case 3:
			if(m_dwStk3 < GetTickCount())
			{
				if(m_tInfo.iCurMP >= 30)
				{
					CDevice::GetInstance()->SoundPlay(1,0);
					m_tInfo.iCurMP -= 30;
					m_dwState = STATE_STRIKE3;
					m_dwStk3 = GetTickCount() + 500;
					m_dwStk1 = GetTickCount() + 500;
					m_tFrame.iStart = 0;
					Strike3();
				}
			}
			break;
		case 4:
			if(m_dwStk4 < GetTickCount())
			{
				if(m_tInfo.iCurMP >= 40)
				{
					CDevice::GetInstance()->SoundPlay(1,0);
					m_tInfo.iCurMP -= 40;
					m_dwState = STATE_STRIKE4;
					m_dwStk4 = GetTickCount() + 500;
					m_dwStk1 = GetTickCount() + 500;
					m_tFrame.iStart = 0;
					Strike4();
					SkillMove(50);
				}
			}
			break;
		case 5:
			if(m_dwStk5 < GetTickCount())
			{
				if(m_tInfo.iCurMP >= 50)
				{
					CDevice::GetInstance()->SoundPlay(1,0);
					m_tInfo.iCurMP -= 50;
					m_dwState = STATE_STRIKE5;
					m_dwStk5 = GetTickCount() + 500;
					m_dwStk1 = GetTickCount() + 500;
					m_tFrame.iStart = 0;
					Strike5();
				}
			}
			break;
		}
	}
	BoundaryCheck();

	/* 콤보 초기화 */
	if(GetTickCount() >= m_dwLastPressed)
		m_iCombo = 1;
}


void CPlayer::Motion(void)
{
	/* 대기 중 */	
	if(!m_dwKey && 	m_dwState != STATE_DASH && 
		m_dwState != STATE_ATTACK1 && m_dwState != STATE_ATTACK2 &&
		m_dwState != STATE_ATTACK3 && m_dwState != STATE_ATTACK4 &&
		m_dwState != STATE_STRIKE1 && m_dwState != STATE_STRIKE2 &&
		m_dwState != STATE_STRIKE3 && m_dwState != STATE_STRIKE4 &&
		m_dwState != STATE_STRIKE5 && m_dwState != STATE_DEAD)
		m_dwState = STATE_STAND;
	else if(m_dwState != STATE_DASH && 
		m_dwState != STATE_ATTACK1 && m_dwState != STATE_ATTACK2 &&
		m_dwState != STATE_ATTACK3 && m_dwState != STATE_ATTACK4 &&
		m_dwState != STATE_STRIKE1 && m_dwState != STATE_STRIKE2 &&
		m_dwState != STATE_STRIKE3 && m_dwState != STATE_STRIKE4 &&
		m_dwState != STATE_STRIKE5 && m_dwState != STATE_DEAD)
		m_dwState = STATE_MOVE;

	/* 대기 모션 */
	if(m_dwState  & STATE_STAND)
	{
		SetScene(9,0,100);
	}
	
	/* 이동 모션 */
	if(m_dwState  & STATE_MOVE)
	{
		SetScene(7,1,100);
	}

	/* 대시 모션 */
	if(m_dwState  & STATE_DASH)
	{
		SetScene(0,1,0);
	}

	/* 공격1 모션 */
	if(m_dwState  & STATE_ATTACK1)
	{
		SetScene(6,2,50);
	}

	/* 공격2 모션 */
	if(m_dwState  & STATE_ATTACK2)
	{
		SetScene(4,3,50);
	}

	/* 공격3 모션 */
	if(m_dwState  & STATE_ATTACK3)
	{
		SetScene(6,4,50);
	}
	
	/* 공격4 모션 */
	if(m_dwState  & STATE_ATTACK4)
	{
		SetScene(4,5,50);
	}
	/* 강타1 모션 */
	if(m_dwState  & STATE_STRIKE1)
	{
		SetScene(4,0,100);
	}
	/* 강타2 모션 */
	if(m_dwState  & STATE_STRIKE2)
	{
		SetScene(6,4,100);
	}
	/* 강타3 모션 */
	if(m_dwState  & STATE_STRIKE3)
	{
		SetScene(4,5,100);
	}
	/* 강타4 모션 */
	if(m_dwState  & STATE_STRIKE4)
	{
		SetScene(0,1,500);
	}
	/* 강타5 모션 */
	if(m_dwState  & STATE_STRIKE5)
	{
		SetScene(4,3,100);
	}
	/* 사망 모션 */
	if(m_dwState  & STATE_DEAD)
	{
		m_strDrawID = "Player_DEAD";
		SetScene(1,0,5000);
	}
}

void CPlayer::Frame(void)
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

void CPlayer::Direction(void)
{
	/* 마우스 좌클릭 시 방향 조정 */
	//if(	(0 <= m_fAngle && m_fAngle < 22) ||
	//	(337 <= m_fAngle && m_fAngle < 360))
	//{
	//	m_strDrawID =  "Player_RIGHT";
	//}
	//if(22 <= m_fAngle && m_fAngle < 67)
	//{
	//	m_strDrawID =  "Player_RU";
	//}
	//if(67 <= m_fAngle && m_fAngle < 112)
	//{
	//	m_strDrawID =  "Player_UP";
	//}
	//if(112 <= m_fAngle && m_fAngle < 157)
	//{
	//	m_strDrawID =  "Player_LU";
	//}
	//if(157 <= m_fAngle && m_fAngle < 202)
	//{
	//	m_strDrawID =  "Player_LEFT";
	//}
	//if(202 <= m_fAngle && m_fAngle < 247)
	//{
	//	m_strDrawID =  "Player_LD";
	//}
	//if(247 <= m_fAngle && m_fAngle < 292)
	//{
	//	m_strDrawID =  "Player_DOWN";
	//}
	//if(292 <= m_fAngle && m_fAngle < 337)
	//{
	//	m_strDrawID =  "Player_RD";
	//}
	if(	m_dwState != STATE_DASH && 
		m_dwState != STATE_ATTACK1 && m_dwState != STATE_ATTACK2 &&
		m_dwState != STATE_ATTACK3 && m_dwState != STATE_ATTACK4 &&
		m_dwState != STATE_STRIKE1 && m_dwState != STATE_STRIKE2 &&
		m_dwState != STATE_STRIKE3 && m_dwState != STATE_STRIKE4)
	{
		if(KEY_LEFT & m_dwKey && !(KEY_UP & m_dwKey) && !(KEY_DOWN & m_dwKey))
		{
			m_strDrawID =  "Player_LEFT";
		}
		else if(KEY_LEFT & m_dwKey && KEY_UP & m_dwKey)
		{
			m_strDrawID =  "Player_LU";
		}
		else if(KEY_RIGHT & m_dwKey && KEY_UP & m_dwKey)
		{
			m_strDrawID =  "Player_RU";
		}
		else if(KEY_LEFT & m_dwKey && KEY_DOWN & m_dwKey)
		{
			m_strDrawID =  "Player_LD";
		}
		else if(KEY_RIGHT & m_dwKey && KEY_DOWN & m_dwKey)
		{
			m_strDrawID =  "Player_RD";
		}
		else if(KEY_RIGHT & m_dwKey)
		{
			m_strDrawID =  "Player_RIGHT";
		}
		else if(KEY_UP & m_dwKey)
		{
			m_strDrawID =  "Player_UP";
		}
		else if(KEY_DOWN & m_dwKey)
		{
			m_strDrawID =  "Player_DOWN";
		}
	}
}

void CPlayer::SetScene(int _iLast,int _iScene,DWORD _dwTime)
{
	if(m_tFrame.iScene != _iScene)
		m_tFrame.iStart = 0;

	m_tFrame.iLast =  _iLast;
	m_tFrame.iScene = _iScene;
	m_tFrame.dwTime = _dwTime;

}

void CPlayer::Scroll()
{
	/*마우스 좌클릭을 이용한 스크롤 이동*/
	/*if(m_tInfo.fX > m_tOffset.x )
	{
		m_tScroll.x -= cosf(m_fAngle *  PI / 180) * m_fSpeed;
		m_tOffset.x += cosf(m_fAngle *  PI / 180) * m_fSpeed;
	}

	if(m_tInfo.fX < m_tOffset.x)
	{
		m_tScroll.x -= cosf(m_fAngle *  PI / 180) * m_fSpeed;
		m_tOffset.x += cosf(m_fAngle *  PI / 180) * m_fSpeed;
	}

	if(m_tInfo.fY > m_tOffset.y)
	{
		m_tScroll.y -= -sinf(m_fAngle *  PI / 180) * m_fSpeed;
		m_tOffset.y += -sinf(m_fAngle *  PI / 180) * m_fSpeed;
	}

	if(m_tInfo.fY < m_tOffset.y)
	{
		m_tScroll.y -= -sinf(m_fAngle *  PI / 180) * m_fSpeed;
		m_tOffset.y += -sinf(m_fAngle *  PI / 180) * m_fSpeed;
	}*/

	if(m_tInfo.fX > m_tOffset.x )
	{
		m_tScroll.x -= m_fSpeed;
		m_tOffset.x += m_fSpeed;
	}

	if(m_tInfo.fX < m_tOffset.x)
	{
		m_tScroll.x += m_fSpeed;
		m_tOffset.x -= m_fSpeed;
	}

	if(m_tInfo.fY > m_tOffset.y)
	{
		m_tScroll.y -= m_fSpeed;
		m_tOffset.y += m_fSpeed;
	}

	if(m_tInfo.fY < m_tOffset.y)
	{
		m_tScroll.y += m_fSpeed;
		m_tOffset.y -= m_fSpeed;
	}
}