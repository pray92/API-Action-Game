#include "StdAfx.h"
#include "Obstacle.h"
#include "BitMap.h"

void CObstacle::Initialize(void)
{	
	m_tInfo.fX =  rand() % 1750 + 410;
	m_tInfo.fY = rand() % 900 + 200;
	m_tInfo.fCX = 60;
	m_tInfo.fCY = 130;
	
	m_strDrawID = "Obstacle";
	m_tInfo.iCurHP = 500;
	m_tInfo.iMaxHP = m_tInfo.iCurHP;
	m_tInfo.iCurMP = 0;
	m_tInfo.iMaxMP = 100;
	m_tInfo.dwDamaged = GetTickCount();
	m_tInfo.bDamaged = false;
	BoundaryCheck();
}

int CObstacle::Progress(void)
{
	if(m_tInfo.iCurHP <= 0)
		return -1;
	else
	{
		if(m_tInfo.dwDamaged < GetTickCount())
			m_tInfo.bDamaged = false;
		return 0;
	}
}

void CObstacle::Render(HDC hdc)
{
	TCHAR str[128];

	TransparentBlt(hdc,
		(int)(m_tInfo.fX - m_tInfo.fCX / 2)  + m_tScroll.x,
		(int)(m_tInfo.fY - m_tInfo.fCY / 2) + m_tScroll.y,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		(*m_pMapBmp)[m_strDrawID]->GetMemDC(),
		0,0,
		(int)m_tInfo.fCX,(int)m_tInfo.fCY,
		RGB(0,0,0));
	
	float fPerHP = (float)m_tInfo.iCurHP / m_tInfo.iMaxHP;
	float fPerMP = (float)m_tInfo.iCurMP / m_tInfo.iMaxMP;

	/* »óÅÂ ¹Ù */
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

void CObstacle::Release(void)
{

}

CObstacle::CObstacle(void)
{
}


CObstacle::~CObstacle(void)
{
	Release();
}
