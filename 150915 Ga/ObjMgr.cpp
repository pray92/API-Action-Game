#include "StdAfx.h"
#include "ObjMgr.h"
#include "Obj.h"

CObjMgr::CObjMgr(void)
{
}


CObjMgr::~CObjMgr(void)
{
}

void CObjMgr::AddRender(CObj* _pObj)
{
	m_MapObj.insert(make_pair(_pObj->GetInfo()->fY,_pObj));
}
void CObjMgr::Render(HDC hdc)
{
	for(multimap<float,CObj*>::iterator iter = m_MapObj.begin();
		iter != m_MapObj.end(); ++iter)
	{
		iter->second->Render(hdc);
	}
}
void CObjMgr::Clear(void)
{
	m_MapObj.clear();
}
