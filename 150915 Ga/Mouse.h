#pragma once
#include "Obj.h"
class CMouse
{
public:

	static POINTFLOAT GetPosf()
	{
		POINT tPoint;
		GetCursorPos(&tPoint);
		ScreenToClient(g_hWnd,&tPoint);
		POINTFLOAT tfPont = {(float)tPoint.x,(float)tPoint.y};
		return tfPont;
	}
	static POINT GetPos()
	{
		POINT tPoint;
		GetCursorPos(&tPoint);
		ScreenToClient(g_hWnd,&tPoint);
		return tPoint;
	}
	CMouse(void);
	virtual ~CMouse(void);
};

