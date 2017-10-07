#pragma once
#include "obj.h"
class CMyButton :
	public CObj
{
	int	   m_iScene;
public:
	virtual void Initialize(void);
	virtual int Progress(void);
	virtual void Render(HDC hdc);
	virtual void Release(void);

	CMyButton(void);
	virtual ~CMyButton(void);
};

