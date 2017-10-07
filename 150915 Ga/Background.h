#pragma once
#include "obj.h"
class CBackground :
	public CObj
{
public:
	virtual void Initialize(void);
	virtual int Progress(void);
	virtual void Render(HDC hdc);
	virtual void Release(void);
public:
	CBackground(void);
	virtual ~CBackground(void);
};

