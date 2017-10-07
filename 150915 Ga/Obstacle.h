#pragma once
#include "obj.h"
class CObstacle :
	public CObj
{
public:
	void Initialize(void);
	int Progress(void);
	void Render(HDC hdc);
	void Release(void);
public:
	CObstacle(void);
	virtual ~CObstacle(void);
};

