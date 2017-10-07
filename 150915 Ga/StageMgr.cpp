#include "StdAfx.h"
#include "StageMgr.h"
#include "StateObj.h"
#include "Logo.h"
#include "Stage1.h"
#include "Stage2.h"
CStageMgr* CStageMgr::m_pInst = NULL;
CStageMgr::CStageMgr(void)
	:m_pState(0)
{
}


CStageMgr::~CStageMgr(void)
{
	Release();
}

void CStageMgr::SetState(STATEID _eStage)
{
	if(m_pState)
	{
		delete m_pState;
		m_pState = NULL;
	}

	switch(_eStage)
	{
	case SI_LOGO:
		m_pState = new CLogo;
		break;
	case SI_STAGE1:
		m_pState = new CStage1;
		break;
	case SI_STAGE2:
		m_pState = new CStage2;
		break;
	}
	m_pState->Initialize();
}

void CStageMgr::Release()
{
	if(m_pState)
	{
		delete m_pState;
		m_pState = NULL;
	}
}