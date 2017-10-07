#include "StdAfx.h"
#include "Tile.h"
#include "BitMap.h"
#include "KeyManager.h"

void CTile::Initialize(void)
{
	SetDrawID("TILE");
}

int CTile::Progress(void)
{
	return 0;
}

void CTile::Render(HDC hdc)
{

	//Rectangle(hdc, 0, 0, WINCX, WINCY);
	TransparentBlt(hdc,
		(int)m_tInfo.fX - TILECX / 2 + m_tScroll.x,
		(int)m_tInfo.fY - TILECY / 2 + m_tScroll.y,
		TILECX,TILECY,
		(*m_pMapBmp)[m_strDrawID]->GetMemDC(),
		0,0,TILECX,TILECY,
		RGB(248,0,248));
}

void CTile::Release(void)
{

}

CTile::CTile(void)
{
}

CTile::~CTile(void)
{
	Release();
}
