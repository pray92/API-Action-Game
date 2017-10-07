#pragma once

class CObj;
class CObjMgr
{
private:
	multimap<float,CObj*> m_MapObj;
public:
	void AddRender(CObj* _pObj);
	void Render(HDC hdc);
	void Clear(void);

public:
	CObjMgr(void);
	virtual ~CObjMgr(void);
	DECLARE_SINGLETON(CObjMgr);
};

