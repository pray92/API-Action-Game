#include "StdAfx.h"
#include "StateObj.h"


CStateObj::CStateObj(void)
{
}


CStateObj::~CStateObj(void)
{
	for (map<string,CBitMap*>::iterator iter = m_MapBmp.begin();
		iter != m_MapBmp.end(); ++iter)
	{
		SAFE_DELETE(iter->second);
	}
	m_MapBmp.clear();
}
