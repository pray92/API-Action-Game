#pragma once
#include "obj.h"
class CPointer :
	public CObj
{
public:
	virtual void Initialize(void);
	virtual int Progress(void);
	virtual void Render(HDC hdc);
	virtual void Release(void);
public:
	CPointer(void);
	virtual ~CPointer(void);
};

