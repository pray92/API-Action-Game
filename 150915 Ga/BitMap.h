#pragma once
class CBitMap
{
public:
	HDC m_hdc, m_MemDC;
	HBITMAP m_bitmap,m_Oldbitmap;

public:
	CBitMap* LoadBmp(PTCHAR pFileName);
	void Release(void);

	HDC GetMemDC(void){return  m_MemDC;}

	CBitMap(void);
	virtual ~CBitMap(void);
};

