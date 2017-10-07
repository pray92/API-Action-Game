#include "StdAfx.h"
#include "Logo.h"
#include "StageMgr.h"
#include <vfw.h>
#pragma comment(lib, "vfw32.lib")

CLogo::CLogo(void)
{

}


CLogo::~CLogo(void)
{
	Release();
}

void CLogo::Initialize(void)
{
	/*m_hVideo = MCIWndCreate(g_hWnd,NULL,MCIWNDF_NOPLAYBAR | WS_VISIBLE | WS_CHILD, L"../Video/Open.wmv");
	MoveWindow(m_hVideo,0,0,WINCX,WINCY,NULL);
	MCIWndPlay(m_hVideo);*/
	dwTime = GetTickCount();
}
void CLogo::Progress(void)
{
	CStageMgr::GetInst()->SetState(SI_STAGE1);
	/*if(GetAsyncKeyState(VK_ESCAPE) || GetTickCount() - 48000 >= dwTime)
	{
		CStageMgr::GetInst()->SetState(SI_STAGE1);
	}*/
}
void CLogo::Render(HDC hdc)
{
}
void CLogo::Release(void)
{
	MCIWndClose(m_hVideo);
}