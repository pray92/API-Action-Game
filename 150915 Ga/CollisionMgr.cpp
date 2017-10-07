#include "StdAfx.h"

#include "CollisionMgr.h"
#include "MathMgr.h"
#include "ObjFactory.h"
#include "Device.h"

#include "Obj.h"
#include "Bullet.h"
#include "Player.h"
#include "Damaged.h"

/* 몬스터 피격 충돌 처리 */
bool CCollisionMgr::RectCol(CObj* pTemp,CObj* pDest)
{
	if((pTemp->GetInfo()->fX - pTemp->GetInfo()->fCX / 2 < pDest->GetInfo()->fX + pDest->GetInfo()->fCX / 2 && pTemp->GetInfo()->fX + pTemp->GetInfo()->fCX / 2 > pDest->GetInfo()->fX - pDest->GetInfo()->fCX / 2) &&
		(pTemp->GetInfo()->fY - pTemp->GetInfo()->fCY / 2 < pDest->GetInfo()->fY + pDest->GetInfo()->fCY / 2 && pTemp->GetInfo()->fY + pTemp->GetInfo()->fCY / 2 > pDest->GetInfo()->fY - pDest->GetInfo()->fCY / 2))
		return true;
	return false;
}

void CCollisionMgr::ColRect(list<CObj*>* pTemp,list<CObj*>* pDest)
{
	for(list<CObj*>::iterator iter1 = pTemp->begin();
		iter1 != pTemp->end(); ++iter1)
	{
		for(list<CObj*>::iterator iter2 = pDest->begin();
			iter2 != pDest->end();)
		{
			/* 피격 */
			if(RectCol(*iter1,*iter2))
			{
				if(!(*iter2)->GetInfo()->bDamaged)
				{
					(*iter2)->GetDamageList()->push_back(CObjFactory<CDamaged>::CreateObj((*iter2)->GetInfo()->fX, (*iter2)->GetInfo()->fY));
					if((*iter2)->GetDrawID() == "Witch_LD" ||(*iter2)->GetDrawID() == "Witch_LU" ||
						(*iter2)->GetDrawID() == "Witch_RD" ||(*iter2)->GetDrawID() == "Witch_RU")
					{
						if((*iter2)->GetMonsterList()->size() == 0 && (*iter2)->GetObstacleList()->size() == 0)
						{
							CDevice::GetInstance()->SoundPlay(6,0);
							(*iter2)->GetInfo()->bDamaged = true;
							(*iter2)->GetInfo()->dwDamaged = GetTickCount() + 200;
							(*iter2)->GetInfo()->iCurHP -= (*iter1)->GetInfo()->iDamage;
						}
						if((*iter2)->GetFourthPhase())
						{
							CDevice::GetInstance()->SoundPlay(6,0);
							(*iter2)->GetInfo()->bDamaged = true;
							(*iter2)->GetInfo()->dwDamaged = GetTickCount() + 200;
							(*iter2)->GetInfo()->iCurHP -= (*iter1)->GetInfo()->iDamage;
						}
					}
					else if((*iter2)->GetDrawID() == "Obstacle")
					{
						CDevice::GetInstance()->SoundPlay(5,0);
						(*iter2)->GetInfo()->bDamaged = true;
						(*iter2)->GetInfo()->dwDamaged = GetTickCount() + 200;
						(*iter2)->GetInfo()->iCurHP -= (*iter1)->GetInfo()->iDamage;
					}
					else
					{
						CDevice::GetInstance()->SoundPlay(5,0);
						(*iter2)->GetInfo()->bDamaged = true;
						(*iter2)->GetInfo()->dwDamaged = GetTickCount() + 200;
						(*iter2)->GetInfo()->iCurHP -= (*iter1)->GetInfo()->iDamage;
						(*iter2)->GetInfo()->fX -= cosf((*iter2)->GetAngle() * PI / 180) * 3;
						(*iter2)->GetInfo()->fY -= -sinf((*iter2)->GetAngle() * PI / 180) * 3;
					}
				}
			}
			(*iter2)->BoundaryCheck();
			++iter2;

			if(iter1 == pTemp->end())
				break;
		}

		if(iter1 == pTemp->end())
			break;
	}
}
/* 플레이어 피격 충돌 처리 */
bool CCollisionMgr::PlayerCol(CObj* pTemp,CObj* pDest)
{
	if((pTemp->GetInfo()->fX - pTemp->GetInfo()->fCX / 2 < pDest->GetInfo()->fX + 36 / 2 && pTemp->GetInfo()->fX + pTemp->GetInfo()->fCX / 2 > pDest->GetInfo()->fX - 36 / 2) &&
		(pTemp->GetInfo()->fY - pTemp->GetInfo()->fCY / 2 < pDest->GetInfo()->fY + 90 / 2 + 36 && pTemp->GetInfo()->fY + pTemp->GetInfo()->fCY / 2 > pDest->GetInfo()->fY - 90 / 2 + 36))
		return true;
	return false;
}

void CCollisionMgr::ColPlayer(list<CObj*>* pTemp,list<CObj*>* pDest)
{
	for(list<CObj*>::iterator iter1 = pTemp->begin();
		iter1 != pTemp->end(); ++iter1)
	{
		for(list<CObj*>::iterator iter2 = pDest->begin();
			iter2 != pDest->end();)
		{
			if((*iter2)->GetDrawID() == "Monster_LD" ||(*iter2)->GetDrawID() == "Monster_LU" ||
				(*iter2)->GetDrawID() == "Monster_RD" ||(*iter2)->GetDrawID() == "Monster_RU")
			{
				if(PlayerCol(*iter1,*iter2))
				{
					if(!(*iter2)->GetInfo()->bDamaged)
					{
						CDevice::GetInstance()->SoundPlay(5,0);
						(*iter2)->GetDamageList()->push_back(CObjFactory<CDamaged>::CreateObj((*iter2)->GetInfo()->fX, (*iter2)->GetInfo()->fY));
						(*iter2)->GetInfo()->bDamaged = true;
						(*iter2)->GetInfo()->dwDamaged = GetTickCount() + 200;
						(*iter2)->GetInfo()->iCurHP -= (*iter1)->GetInfo()->iDamage;
						(*iter2)->GetInfo()->fX -= cosf((*iter2)->GetAngle() * PI / 180) * 3;
						(*iter2)->GetInfo()->fY -= -sinf((*iter2)->GetAngle() * PI / 180) * 3;
					}
				}
			}
			else
			{
				/* 피격 */
				if(PlayerCol(*iter1,*iter2))
				{
					CDevice::GetInstance()->SoundPlay(3,0);
					(*iter2)->GetDamageList()->push_back(CObjFactory<CDamaged>::CreateObj((*iter2)->GetInfo()->fX, (*iter2)->GetInfo()->fY));
					(*iter2)->GetInfo()->iCurHP -= (*iter1)->GetInfo()->iDamage;
				}
			}
			++iter2;

			if(iter1 == pTemp->end())
				break;
		}

		if(iter1 == pTemp->end())
			break;
	}
}

CCollisionMgr::CCollisionMgr(void)
{
}


CCollisionMgr::~CCollisionMgr(void)
{
}
