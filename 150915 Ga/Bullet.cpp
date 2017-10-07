#include "StdAfx.h"
#include "Bullet.h"
#include "BitMap.h"

void CBullet::Initialize(void)
{
}

void CBullet::SetBullet(string _strDrawID,float _fX, float _fY, float _fCX, float _fCY, 
					float _fAngle, float _fSpeed, int _iDamage, DWORD _dwDuration)
{
	m_strDrawID = _strDrawID;
	m_tInfo.fX = _fX;
	m_tInfo.fY = _fY;
	m_tInfo.fCX = _fCX;
	m_tInfo.fCY = _fCY;
	m_tInfo.iDamage = _iDamage;
	m_fAngle = _fAngle;
	m_fSpeed = _fSpeed;
	m_dwDuration = GetTickCount() + _dwDuration;

	m_dwTime = GetTickCount();
}

int CBullet::Progress(void)
{
	/* 보스 탄환 한정 */
	if(m_strDrawID == "Bullet")
	{
		if(m_dwDuration < GetTickCount() ||
			m_tInfo.fY > 900 || m_tInfo.fY < 200 ||
			m_tInfo.fX < 410 || m_tInfo.fX > 1750)
		{
			m_strDrawID = "";
			return -1;
		}
		else
		{
			m_tInfo.fX += cosf(m_fAngle *  PI / 180) * m_fSpeed;
			m_tInfo.fY += -sinf(m_fAngle *  PI / 180) * m_fSpeed;
			Motion();
			Frame();
			return 0;
		}
	}
	else
	{
		if(m_dwDuration < GetTickCount())
		{
			m_strDrawID = "";
			return -1;
		}
		else
		{
			m_tInfo.fX += cosf(m_fAngle *  PI / 180) * m_fSpeed;
			m_tInfo.fY += -sinf(m_fAngle *  PI / 180) * m_fSpeed;
			Motion();
			Frame();
			return 0;
		}
	}
}

void CBullet::Render(HDC hdc)
{	
	TCHAR str[128] = L"";

	if(m_strDrawID != "")
	{
		TransparentBlt(hdc,
			(int)(m_tInfo.fX - m_tInfo.fCX /2) + m_tScroll.x,
			(int)(m_tInfo.fY - m_tInfo.fCY /2) + m_tScroll.y,
			(int)m_tInfo.fCX,
			(int)m_tInfo.fCY,
			(*m_pMapBmp)[m_strDrawID]->GetMemDC(),
			(int)(m_tInfo.fCX * m_tFrame.iStart),
			(int)(m_tInfo.fCY * m_tFrame.iScene),
			(int)m_tInfo.fCX,(int)m_tInfo.fCY,
			RGB(0,0,0));	
	}
	
	if(GetAsyncKeyState(VK_SHIFT) & 0x8000)
	{
		wsprintf(str, L"%s - x : %d y : %d iStart : %d", m_strDrawID, (int)m_tInfo.fX, (int)m_tInfo.fY,(int)m_tFrame.iStart);
		TextOut(hdc, (int)m_tInfo.fX - 22 + m_tScroll.x, (int)m_tInfo.fY - 40 + m_tScroll.y, str, lstrlen(str));
	}
	/*Rectangle(hdc,
		m_tInfo.fX - m_tInfo.fCX / 2 + m_tScroll.x,
		m_tInfo.fY - m_tInfo.fCY / 2 + m_tScroll.y,
		m_tInfo.fX + m_tInfo.fCX / 2 + m_tScroll.x,
		m_tInfo.fY + m_tInfo.fCY / 2 + m_tScroll.y);*/
}

void CBullet::Release(void)
{
}

void CBullet::Frame(void)
{
	if(m_dwTime + m_tFrame.dwTime < GetTickCount())
	{
		m_dwTime = GetTickCount();
		++m_tFrame.iStart;

		if(m_tFrame.iStart >= m_tFrame.iLast )
		{
			m_tFrame.iStart = 0;
		}
	}
}

void CBullet::SetScene(int _iLast,int _iScene,DWORD _dwTime)
{
	if(m_tFrame.iScene != _iScene)
		m_tFrame.iStart = 0;

	m_tFrame.iLast =  _iLast;
	m_tFrame.iScene = _iScene;
	m_tFrame.dwTime = _dwTime;
}

void CBullet::Motion(void)
{
	/* 강타1 모션 */
	if(m_strDrawID == "Stk1")
	{
		SetScene(4,0,50);
	}
	/* 강타2 모션 */
	if(m_strDrawID == "Stk2")
	{
		SetScene(9,0,25);
	}
	/* 강타3 모션 */
	if(m_strDrawID == "Stk3")
	{
		SetScene(9,0,25);
	}
	/* 강타4 모션 */
	if(m_strDrawID == "Stk4_DOWN")
	{
		SetScene(2,0,100);
	}
	if(m_strDrawID == "Stk4_LD")
	{
		SetScene(2,0,100);
	}
	if(m_strDrawID == "Stk4_LEFT")
	{
		SetScene(2,0,100);
	}
	if(m_strDrawID == "Stk4_LU")
	{
		SetScene(2,0,100);
	}
	if(m_strDrawID == "Stk4_UP")
	{
		SetScene(2,0,100);
	}
	if(m_strDrawID == "Stk4_RU")
	{
		SetScene(2,0,100);
	}
	if(m_strDrawID == "Stk4_RIGHT")
	{
		SetScene(2,0,100);
	}
	if(m_strDrawID == "Stk4_RD")
	{
		SetScene(2,0,100);
	}	
	/* 강타5 모션 */
	if(m_strDrawID == "Stk5")
	{
		SetScene(9,0,50);
	}
	
	/* 보스몹 탄환 */
	if(m_strDrawID == "Bullet")
	{
		SetScene(2,0,100);
	}
	/* 보스몹 즉사 */
	if(m_strDrawID == "Onekill")
	{
		SetScene(9,0,50);
	}
}

CBullet::CBullet(void)
{
}

CBullet::~CBullet(void)
{
	Release();
}
