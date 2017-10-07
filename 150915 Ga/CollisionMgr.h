#pragma once

class CObj;
class CCollisionMgr
{
	static bool RectCol(CObj* pTemp,CObj* pDest);
	static bool PlayerCol(CObj* pTemp,CObj* pDest);
public:
	static void ColRect(list<CObj*>* pTemp,list<CObj*>* pDest);
	static void ColPlayer(list<CObj*>* pTemp,list<CObj*>* pDest);
	CCollisionMgr(void);
	virtual ~CCollisionMgr(void);
};

