#include "StdAfx.h"
#include "Stage2.h"

#include "Obj.h"
#include "MyButton.h"
#include "Background.h"
#include "Player.h"
#include "Monster.h"
#include "Boss.h"
#include "Tile.h"
#include "Obstacle.h"

#include "BitMap.h"

#include "ObjMgr.h"
#include "StageMgr.h"
#include "ObjFactory.h"
#include "CollisionMgr.h"
#include "MathMgr.h"
#include "Device.h"


CStage2::CStage2(void)
{
}


CStage2::~CStage2(void)
{
	Release();
}

void CStage2::Initialize(void)
{
	
	CDevice::GetInstance()->Init();
	CDevice::GetInstance()->LoadWave(L"../Sound/Player_Attack.wav");			//0번
	CDevice::GetInstance()->LoadWave(L"../Sound/Player_Strike.wav");			//1번
	CDevice::GetInstance()->LoadWave(L"../Sound/Player_Damaged.wav");		//2번
	CDevice::GetInstance()->LoadWave(L"../Sound/Player_Shocked.wav");		//3번

	CDevice::GetInstance()->LoadWave(L"../Sound/Monster_Attack.wav");		//4번
	CDevice::GetInstance()->LoadWave(L"../Sound/Monster_Damaged.wav");		//5번
	CDevice::GetInstance()->LoadWave(L"../Sound/Boss_Damaged.wav");			//6번

	CDevice::GetInstance()->LoadWave(L"../Sound/Bgm.wav");					//7번
	CDevice::GetInstance()->LoadWave(L"../Sound/Dead.wav");					//8번
	CDevice::GetInstance()->LoadWave(L"../Sound/Stage_Clear.wav");			//9번
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
	m_MapBmp["Onekill"] = (new CBitMap)->LoadBmp(L"../Texture/Effect/Onekill.bmp");
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
	m_MapBmp["Player_DEAD"] = (new CBitMap)->LoadBmp(L"../Texture/Player/Player_DEAD.bmp");
#pragma endregion 플레이어
	
#pragma region 보스
	m_MapBmp["Witch_RD"] = (new CBitMap)->LoadBmp(L"../Texture/Boss/Witch_RD.bmp");
	m_MapBmp["Witch_RU"] = (new CBitMap)->LoadBmp(L"../Texture/Boss/Witch_RU.bmp");
	m_MapBmp["Witch_LU"] = (new CBitMap)->LoadBmp(L"../Texture/Boss/Witch_LU.bmp");
	m_MapBmp["Witch_LD"] = (new CBitMap)->LoadBmp(L"../Texture/Boss/Witch_LD.bmp");
	m_MapBmp["Witch_DEAD"] = (new CBitMap)->LoadBmp(L"../Texture/Boss/Witch_DEAD.bmp");
	m_MapBmp["Bullet"] = (new CBitMap)->LoadBmp(L"../Texture/Boss/Bullet.bmp");
#pragma endregion 보스
	
#pragma region 몬스터
	m_MapBmp["Monster_LD"] = (new CBitMap)->LoadBmp(L"../Texture/Monster/Monster_LD.bmp");
	m_MapBmp["Monster_LU"] = (new CBitMap)->LoadBmp(L"../Texture/Monster/Monster_LU.bmp");
	m_MapBmp["Monster_RD"] = (new CBitMap)->LoadBmp(L"../Texture/Monster/Monster_RD.bmp");
	m_MapBmp["Monster_RU"] = (new CBitMap)->LoadBmp(L"../Texture/Monster/Monster_RU.bmp");
#pragma endregion 몬스터
	
#pragma region 장애물
	m_MapBmp["Obstacle"] = (new CBitMap)->LoadBmp(L"../Texture/Obstacle/Obstacle.bmp");
#pragma endregion 장애물

#pragma region 상태바
	m_MapBmp["Bar_BG"] = (new CBitMap)->LoadBmp(L"../Texture/Bar/Bar_BG.bmp");
	m_MapBmp["Bar_HP"] = (new CBitMap)->LoadBmp(L"../Texture/Bar/Bar_HP.bmp");
	m_MapBmp["Bar_MP"] = (new CBitMap)->LoadBmp(L"../Texture/Bar/Bar_MP.bmp");
#pragma endregion 상태바
	CDevice::GetInstance()->SoundPlay(7,1);
	/* 플레이어 생성 */		
	CPlayer* TempPlayer = new CPlayer;
	TempPlayer->Initialize();
	TempPlayer->SetBulletList(&m_ObjList[OBJ_BULLET]);
	TempPlayer->SetDamageList(&m_ObjList[OBJ_DAMAGED]);
	m_ObjList[OBJ_PLAYER].push_back(TempPlayer);
	/* 맵 생성 */
	m_ObjList[OBJ_BACKGROUD].push_back(CObjFactory<CBackground>::CreateObj());

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
	/* 맵 생성 */

	CBoss* TempBoss = new CBoss;
	TempBoss->Initialize();
	TempBoss->SetBulletList(&m_ObjList[OBJ_MONBULLET]);
	TempBoss->SetPlayerList(&m_ObjList[OBJ_PLAYER]);
	TempBoss->SetMonsterList(&m_ObjList[OBJ_MONSTER]);
	TempBoss->SetObstacleList(&m_ObjList[OBJ_OBSTACLE]);
	TempBoss->SetDamageList(&m_ObjList[OBJ_DAMAGED]);
	m_ObjList[OBJ_BOSS].push_back(TempBoss);
	//for (int i =0; i < 20; ++i)
	//{
	//	CMonster* TempMonster = new CMonster;
	//	TempMonster->Initialize();
	//	TempMonster->SetPlayerList(&m_ObjList[OBJ_PLAYER]);
	//	m_ObjList[OBJ_MONSTER].push_back(TempMonster);
	//}

	CObj::SetBmp(&m_MapBmp);

}
void CStage2::Progress(void)
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
				{
					if((*iter)->GetDrawID() == "Stk3")
						GET_SINGLE(CObjMgr)->AddRender(*iter);
					else
						(*iter)->Render(m_MapBmp["BackBuffer"]->GetMemDC());
				}
				else
					GET_SINGLE(CObjMgr)->AddRender(*iter);
				++iter;
			}
		}
	}
	/* 공격 */
	CCollisionMgr::ColRect(&m_ObjList[OBJ_BULLET], &m_ObjList[OBJ_BOSS]);
	CCollisionMgr::ColPlayer(&m_ObjList[OBJ_BULLET], &m_ObjList[OBJ_MONSTER]);
	CCollisionMgr::ColRect(&m_ObjList[OBJ_BULLET], &m_ObjList[OBJ_OBSTACLE]);
	CCollisionMgr::ColPlayer(&m_ObjList[OBJ_MONBULLET], &m_ObjList[OBJ_PLAYER]);
	/*if(m_ObjList[OBJ_MONSTER].size() < 20)
	{
		CMonster* TempMonster = new CMonster;
		TempMonster->Initialize();
		TempMonster->SetPlayerList(&m_ObjList[OBJ_PLAYER]);
		m_ObjList[OBJ_MONSTER].push_back(TempMonster);
	}*/
}
void CStage2::Render(HDC hdc)
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
void CStage2::Release(void)
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