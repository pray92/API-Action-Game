#include "StdAfx.h"
#include "MainGame.h"
#include "Player.h"
#include "StateObj.h"
#include "StageMgr.h"
#include "KeyManager.h"
CMainGame::CMainGame(void)
{
}


CMainGame::~CMainGame(void)
{
	Release();
}
void CMainGame::Initialize(void)
{
	m_hdc =  GetDC(g_hWnd);


	CStageMgr::GetInst()->SetState(SI_LOGO);

}
void CMainGame::Progress(void)
{
	GET_SINGLE(CKeyManager)->CheckKey();
	CStageMgr::GetInst()->GetState()->Progress();
}
void CMainGame::Render(void)
{
	CStageMgr::GetInst()->GetState()->Render(m_hdc);
}
void CMainGame::Release(void)
{
	ReleaseDC(g_hWnd,m_hdc);
	CStageMgr::GetInst()->DestroyInst();
	GET_SINGLE(CKeyManager)->DestroyInstance();
}