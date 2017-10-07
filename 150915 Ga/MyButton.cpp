#include "StdAfx.h"
#include "MyButton.h"
#include "Mouse.h"
#include "KeyManager.h"
#include "BitMap.h"
#include "Device.h"

CMyButton::CMyButton(void)
{
}


CMyButton::~CMyButton(void)
{
	Release();
}
void CMyButton::Initialize(void)
{
	m_tInfo.fCX = 184;
	m_tInfo.fCY = 32;

	m_iScene = 0;
}
int CMyButton::Progress(void)
{
	m_iScene = 0;
	if(PtInRect(&GetRect(),CMouse::GetPos()))
	{
		m_iScene = 1;
		if(GET_SINGLE(CKeyManager)->GetKey() & KEY_LBUTTON)
		{
			if(m_strDrawID == "START")
			{
				CDevice::GetInstance()->SoundPlay(1, 1);	
				return 1;
			}
			if(m_strDrawID == "END")
			{
				CDevice::GetInstance()->SoundPlay(1, 1);	
				return 2;
			}
		}
	}
	return -1;
}
void CMyButton::Render(HDC hdc)
{
		TransparentBlt(hdc,
			(int)(m_tInfo.fX - m_tInfo.fCX /2),
			(int)(m_tInfo.fY - m_tInfo.fCY /2),
			(int)m_tInfo.fCX,
			(int)m_tInfo.fCY,
			(*m_pMapBmp)[m_strDrawID]->GetMemDC(),
			(int)(m_tInfo.fCX * m_iScene),0,
			(int)m_tInfo.fCX,(int)m_tInfo.fCY,
			RGB(255,255,255));
}
void CMyButton::Release(void)
{
}