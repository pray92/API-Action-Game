#include "StdAfx.h"
#include "Background.h"
#include "BitMap.h"

void CBackground::Initialize(void)
{
}
int CBackground::Progress(void)
{
	return 0;
}
void CBackground::Render(HDC hdc)
{
	BitBlt(hdc,0,0,WINCX,WINCY,
		(*m_pMapBmp)["Background"]->GetMemDC(), 800 - m_tScroll.x, 600 - m_tScroll.y,SRCCOPY);
}
void CBackground::Release(void)
{

}
CBackground::CBackground(void)
{
}

CBackground::~CBackground(void)
{
	Release();
}
