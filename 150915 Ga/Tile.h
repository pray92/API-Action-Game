#pragma once
#include "obj.h"
class CTile :
	public CObj
{
private:
	void Scroll();
public:
	virtual void Initialize(void);
	virtual int Progress(void);
	virtual void Render(HDC hdc);
	virtual void Release(void);
public:
	CTile(void);
	virtual ~CTile(void);
};

