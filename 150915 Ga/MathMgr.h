#pragma once

#include "Obj.h"

class CMathMgr
{
public:
	static float GetDistance(CObj* pTemp,CObj* pDest)
	{
		float fWidth  = float(pTemp->GetInfo()->fX - pDest->GetInfo()->fX);
		float fHeight  = float(pTemp->GetInfo()->fY - pDest->GetInfo()->fY);
		return float( sqrt(fWidth*fWidth + fHeight*fHeight) ) ;
	}
	static float GetAngle(CObj* pTemp,CObj* pDest)
	{
		float fWidth  = float(pTemp->GetInfo()->fX - pDest->GetInfo()->fX);
		float fHeight  = float(pTemp->GetInfo()->fY - pDest->GetInfo()->fY);

		float fDistance = float( sqrt(fWidth*fWidth + fHeight*fHeight) ) ;

		float fAngle =  acosf(fWidth / fDistance) * 180 / PI;
		
		if(pTemp->GetInfo()->fY > pDest->GetInfo()->fY)
		{
			fAngle = 360 - fAngle;
		}

		return fAngle;
	}

	static float GetPlayerMovingAngle(CObj* pTemp,POINTFLOAT* _ptMouse)
	{
		/* 마우스 좌클릭으로 이동 좌표를 설정 */
		float fWidth  = float((*_ptMouse).x - pTemp->GetInfo()->fX - pTemp->GetScroll()->x);
		float fHeight  = float((*_ptMouse).y - pTemp->GetInfo()->fY - pTemp->GetScroll()->y);

		//fHeight = fHeight >= 0 ? fHeight += 10 : fHeight -= 10;

		float fDistance = float( sqrt(fWidth*fWidth + fHeight*fHeight)) ;
		float fAngle =  acosf(fWidth / fDistance) * 180 / PI;
		
		if((*_ptMouse).y > pTemp->GetInfo()->fY)
		{
			fAngle = 360 - fAngle;
		}

		return fAngle;
	}
public:
	CMathMgr(void);
	virtual ~CMathMgr(void);
};
