#include "StdAfx.h"
#include "Damaged.h"
#include "BitMap.h"

void CDamaged::Initialize(void)
{
	m_dwDuration = GetTickCount() + 400;
	m_dwTime = GetTickCount();
	m_tInfo.fCX = m_tInfo.fCY = 150;
	m_strDrawID = "Damaged";
	SetScene(5, 0, 100);
}

int CDamaged::Progress(void)
{
	if(m_dwDuration < GetTickCount())
		return -1;
	Frame();
	return 0;
}

void CDamaged::Render(HDC hdc)
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

void CDamaged::Release(void)
{

}

void CDamaged::Frame(void)
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

void CDamaged::SetScene(int _iLast,int _iScene,DWORD _dwTime)
{
	if(m_tFrame.iScene != _iScene)
		m_tFrame.iStart = 0;

	m_tFrame.iLast =  _iLast;
	m_tFrame.iScene = _iScene;
	m_tFrame.dwTime = _dwTime;
}

CDamaged::CDamaged(void)
{
}


CDamaged::~CDamaged(void)
{
}
