#include "StdAfx.h"
#include "Village.h"

#include "StdAfx.h"
#include "Stage2.h"

#include "Obj.h"
#include "MyButton.h"
#include "Background.h"
#include "Player.h"
#include "Monster.h"
#include "Tile.h"

#include "BitMap.h"

#include "ObjMgr.h"
#include "StageMgr.h"
#include "ObjFactory.h"
#include "CollisionMgr.h"
#include "MathMgr.h"


CVillage::CVillage(void)
{
}


CVillage::~CVillage(void)
{
	Release();
}

void CVillage::Initialize(void)
{

#pragma region 기본 텍스쳐
	m_MapBmp["BackBuffer"] = (new CBitMap)->LoadBmp(L"../Texture/BackBuffer.bmp");
	m_MapBmp["Background"] = (new CBitMap)->LoadBmp(L"../Texture/Background.bmp");
	m_MapBmp["TILE"] = (new CBitMap)->LoadBmp(L"../Texture/TILE.bmp");
#pragma endregion 기본 텍스쳐
	
#pragma region 이펙트
	m_MapBmp["Damaged"] = (new CBitMap)->LoadBmp(L"../Texture/Effect/Damaged.bmp");
	m_MapBmp["Stk1"] = (new CBitMap)->LoadBmp(L"../Texture/Effect/Stk1.bmp");
	m_MapBmp["Stk2"] = (new CBitMap)->LoadBmp(L"../Texture/Effect/Stk2.bmp");
	m_MapBmp["Stk3"] = (new CBitMap)->LoadBmp(L"../Texture/Effect/Stk3.bmp");
	m_MapBmp["Stk4_DOWN"] = (new CBitMap)->LoadBmp(L"../Texture/Effect/Stk4_DOWN.bmp");
	m_MapBmp["Stk4_LD"] = (new CBitMap)->LoadBmp(L"../Texture/Effect/Stk4_LD.bmp");
	m_MapBmp["Stk4_LU"] = (new CBitMap)->LoadBmp(L"../Texture/Effect/Stk4_LU.bmp");
	m_MapBmp["Stk4_LEFT"] = (new CBitMap)->LoadBmp(L"../Texture/Effect/Stk4_LEFT.bmp");
	m_MapBmp["Stk4_RIGHT"] = (new CBitMap)->LoadBmp(L"../Texture/Effect/Stk4_RIGHT.bmp");
	m_MapBmp["Stk4_RD"] = (new CBitMap)->LoadBmp(L"../Texture/Effect/Stk4_RD.bmp");
	m_MapBmp["Stk4_RU"] = (new CBitMap)->LoadBmp(L"../Texture/Effect/Stk4_RU.bmp");
	m_MapBmp["Stk4_UP"] = (new CBitMap)->LoadBmp(L"../Texture/Effect/Stk4_UP.bmp");
	m_MapBmp["Stk5"] = (new CBitMap)->LoadBmp(L"../Texture/Effect/Stk5.bmp");
#pragma endregion 이펙트
	
#pragma region 플레이어
	m_MapBmp["Player_DOWN"] = (new CBitMap)->LoadBmp(L"../Texture/Player/Player_DOWN.bmp");
	m_MapBmp["Player_LD"] = (new CBitMap)->LoadBmp(L"../Texture/Player/Player_LD.bmp");
	m_MapBmp["Player_LEFT"] = (new CBitMap)->LoadBmp(L"../Texture/Player/Player_LEFT.bmp");
	m_MapBmp["Player_LU"] = (new CBitMap)->LoadBmp(L"../Texture/Player/Player_LU.bmp");
	m_MapBmp["Player_RD"] = (new CBitMap)->LoadBmp(L"../Texture/Player/Player_RD.bmp");
	m_MapBmp["Player_RIGHT"] = (new CBitMap)->LoadBmp(L"../Texture/Player/Player_RIGHT.bmp");
	m_MapBmp["Player_RU"] = (new CBitMap)->LoadBmp(L"../Texture/Player/Player_RU.bmp");
	m_MapBmp["Player_UP"] = (new CBitMap)->LoadBmp(L"../Texture/Player/Player_UP.bmp");
#pragma endregion 플레이어
	
#pragma region 몬스터
	m_MapBmp["Monster"] = (new CBitMap)->LoadBmp(L"../Texture/Monster.bmp");
#pragma endregion 몬스터
	
	/* 플레이어 생성 */		
	CPlayer* TempPlayer = new CPlayer;
	TempPlayer->Initialize();
	TempPlayer->SetBulletList(&m_ObjList[OBJ_BULLET]);
	m_ObjList[OBJ_PLAYER].push_back(TempPlayer);
	/* 맵 생성 */
	m_ObjList[OBJ_BACKGROUD].push_back(CObjFactory<CBackground>::CreateObj(0, 0));

	/* 타일 생성 */
	/*for(int i= 0; i < TILEY; ++i)
	{
		for(int j= 0; j < TILEX; ++j)	
		{
			float fx = (j * TILECX / 2) + TILECX / 2;

			float fy = 0;
			if(j % 2 == 0)
				fy = (i * TILECY) + TILECY / 2;
			else
				fy = (i * TILECY) + TILECY;
			m_ObjList[OBJ_TILE].push_back(CObjFactory<CTile>::CreateObj(fx, fy));
		}
	}*/

	/* 몬스터 생성 */
	CMonster* TempMonster = new CMonster;
	TempMonster->Initialize();
	TempMonster->SetPlayerList(&m_ObjList[OBJ_PLAYER]);
	m_ObjList[OBJ_MONSTER].push_back(TempMonster);
	for (int i =0; i < 20; ++i)
	{
		CMonster* TempMonster = new CMonster;
		TempMonster->Initialize();
		TempMonster->SetPlayerList(&m_ObjList[OBJ_PLAYER]);
		m_ObjList[OBJ_MONSTER].push_back(TempMonster);
	}

	CObj::SetBmp(&m_MapBmp);

}
void CVillage::Progress(void)
{
	for(int i=0; i < OBJ_END; ++i)
	{
		for(list<CObj*>::iterator iter = m_ObjList[i].begin();
			iter != m_ObjList[i].end();)
		{
			if((*iter)->Progress() == -1)
			{
				SAFE_DELETE(*iter);
				iter = m_ObjList[i].erase(iter);
			}
			else
			{
				if(i == OBJ_TILE || i == OBJ_BACKGROUD || i == OBJ_BULLET)
					(*iter)->Render(m_MapBmp["BackBuffer"]->GetMemDC());
				else
					GET_SINGLE(CObjMgr)->AddRender(*iter);
				++iter;
			}
		}
	}
	/* 공격 */
	CCollisionMgr::ColRect(&m_ObjList[OBJ_BULLET], &m_ObjList[OBJ_MONSTER]);
	/*if(m_ObjList[OBJ_MONSTER].size() < 20)
	{
		CMonster* TempMonster = new CMonster;
		TempMonster->Initialize();
		TempMonster->SetPlayerList(&m_ObjList[OBJ_PLAYER]);
		m_ObjList[OBJ_MONSTER].push_back(TempMonster);
	}*/
}

void CVillage::Render(HDC hdc)
{	
	/*for(int i=0; i < OBJ_END; ++i)
	{
		for(list<CObj*>::iterator iter = m_ObjList[i].begin();
			iter != m_ObjList[i].end(); ++iter)
		{
			(*iter)->Render(m_MapBmp["BackBuffer"]->GetMemDC());
		}
	}*/

	GET_SINGLE(CObjMgr)->Render(m_MapBmp["BackBuffer"]->GetMemDC());
	GET_SINGLE(CObjMgr)->Clear();

	BitBlt(hdc,0,0,WINCX,WINCY,
		m_MapBmp["BackBuffer"]->GetMemDC(),0,0,SRCCOPY);

}

void CVillage::Release(void)
{
	for(int i=0; i < OBJ_END; ++i)
	{
		for(list<CObj*>::iterator iter = m_ObjList[i].begin();
			iter != m_ObjList[i].end(); ++iter)
		{
			SAFE_DELETE(*iter);
		}
		m_ObjList[i].clear();
	}
}
