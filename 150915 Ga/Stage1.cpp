#include "StdAfx.h"
#include "Stage1.h"

#include "Obj.h"
#include "StageMgr.h"
#include "BitMap.h"
#include "MyButton.h"
#include "Device.h"


CStage1::CStage1(void)
{
}


CStage1::~CStage1(void)
{
	Release();
}
CMyButton* CStage1::CreateButton(string Name,float _x,float _y)
{
	CMyButton* TempObj = new CMyButton();
	TempObj->Initialize();
	TempObj->SetDrawID(Name);
	TempObj->SetPos(_x,_y);
	return TempObj;
}
void CStage1::Initialize(void)
{
	CDevice::GetInstance()->Init();
	CDevice::GetInstance()->LoadWave(L"../Sound/Title.wav");	
	CDevice::GetInstance()->LoadWave(L"../Sound/0119.wav");	
	CDevice::GetInstance()->SoundPlay(0,1);
	m_MapBmp["BackBuffer"] = (new CBitMap)->LoadBmp(L"../Texture/BackBuffer.bmp");
	m_MapBmp["Title"] = (new CBitMap)->LoadBmp(L"../Texture/Title.bmp");
	m_MapBmp["START"] = (new CBitMap)->LoadBmp(L"../Texture/START.bmp");
	m_MapBmp["END"] = (new CBitMap)->LoadBmp(L"../Texture/END.bmp");
	m_MapBmp["CREDIT"] = (new CBitMap)->LoadBmp(L"../Texture/CREDIT.bmp");
	m_MapBmp["OPENING"] = (new CBitMap)->LoadBmp(L"../Texture/OPENING.bmp");

	
	m_ButtonList.push_back(CreateButton("START",644, 541));
	m_ButtonList.push_back(CreateButton("OPENING",644, 572));
	m_ButtonList.push_back(CreateButton("CREDIT",644, 604));
	m_ButtonList.push_back(CreateButton("END", 644, 636));
	CObj::SetBmp(&m_MapBmp);
}
void CStage1::Progress(void)
{
	//CStageMgr::GetInst()->SetState(SI_STAGE2);
	for(list<CObj*>::iterator iter = m_ButtonList.begin();
		iter != m_ButtonList.end(); ++iter)
	{
		int iResult= (*iter)->Progress();
		if(iResult == 1)
		{
			CDevice::GetInstance()->SoundStop(0);
			CStageMgr::GetInst()->SetState(SI_STAGE2);
			return;
		}
		if(iResult == 2)
		{
			DestroyWindow(g_hWnd);
			return;
		}
	}

}
void CStage1::Render(HDC hdc)
{
	BitBlt(m_MapBmp["BackBuffer"]->GetMemDC(),0,0,WINCX,WINCY,
		m_MapBmp["Title"]->GetMemDC(),0,0,SRCCOPY);

	for(list<CObj*>::iterator iter = m_ButtonList.begin();
		iter != m_ButtonList.end(); ++iter)
	{
		(*iter)->Render(m_MapBmp["BackBuffer"]->GetMemDC());
	}

	BitBlt(hdc,0,0,WINCX,WINCY,
		m_MapBmp["BackBuffer"]->GetMemDC(),0,0,SRCCOPY);
}
void CStage1::Release(void)
{
	for (list<CObj*>::iterator iter = m_ButtonList.begin();
		iter != m_ButtonList.end(); ++iter)
	{
		SAFE_DELETE(*iter);
	}
	m_ButtonList.clear();
	CDevice::GetInstance()->DestroyInst();
}
