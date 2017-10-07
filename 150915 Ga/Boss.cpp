#include "StdAfx.h"

#include "Boss.h"
#include "Monster.h"
#include "Bullet.h"
#include "Obstacle.h"

#include "BitMap.h"
#include "MathMgr.h"
#include "ObjFactory.h"
#include "Device.h"

void CBoss::Initialize(void)
{
	m_tInfo.fX =  1175;
	m_tInfo.fY = 480;

	m_strDrawID = "Witch_LU";
	m_fAngle = 135;

	m_tInfo.fCX = 300;
	m_tInfo.fCY = 300;

	m_dwAtk = GetTickCount() + 2000;
	m_dwTime = 0;
	m_iCombo = 0;
	

	/* 기본 스테이터스 */
	m_tInfo.iCurHP = 7000;
	m_tInfo.iMaxHP = m_tInfo.iCurHP;
	m_tInfo.iCurMP = 0;
	m_tInfo.iMaxMP = 100;
	m_tInfo.iDamage = 30;

	m_tInfo.dwDamaged = GetTickCount();
	m_tInfo.bDamaged = false;

	m_bFirstPhase = true;
	m_bSecondPhase = false;
	m_bThirdPhase = false;
	m_bFourthPhase = false;

	m_dwState = STATE_STAND;
}

int CBoss::Progress(void)
{
	if(m_tInfo.iCurHP <= 0 && m_dwState != STATE_DEAD)
	{
		CDevice::GetInstance()->SoundStop(7);
		CDevice::GetInstance()->SoundPlay(9,0);
		m_dwState = STATE_DEAD;
		m_tInfo.dwDeadRemain = GetTickCount() + 1200;
	}
	if(m_dwState != STATE_DEAD)
	{
		m_fAngle = CMathMgr::GetAngle(*m_pPlayerList->begin(),this);
		Attack();
		Frame();
		Motion();
		Direction();
		BoundaryCheck();
		if(m_tInfo.dwDamaged < GetTickCount())
			m_tInfo.bDamaged = false;
		return 0;
	}
	if(m_dwState == STATE_DEAD)
	{
		if(m_tInfo.dwDeadRemain < GetTickCount())
		{
			return -1;
		}
		else
		{
			Motion();
			Frame();
		}
	}
}

void CBoss::Render(HDC hdc)
{
	TCHAR str[50] = L"";

	TransparentBlt(hdc,
		(int)(m_tInfo.fX - m_tInfo.fCX / 2)  + m_tScroll.x,
		(int)(m_tInfo.fY - m_tInfo.fCY / 2) + m_tScroll.y,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		(*m_pMapBmp)[m_strDrawID]->GetMemDC(),
		(int)(m_tInfo.fCX * m_tFrame.iStart),
		(int)(m_tInfo.fCY * m_tFrame.iScene),
		(int)m_tInfo.fCX,(int)m_tInfo.fCY,
		RGB(0,0,0));
	
	float fPerHP = (float)m_tInfo.iCurHP / m_tInfo.iMaxHP;
	float fPerMP = (float)m_tInfo.iCurMP / m_tInfo.iMaxMP;

	/* 상태 바 */
	TransparentBlt(hdc,
		(int)(m_tInfo.fX - 50) + m_tScroll.x,
		(int)(m_tInfo.fY - 10) + 100 + m_tScroll.y,
		100,
		15,
		(*m_pMapBmp)["Bar_BG"]->GetMemDC(),
		0,0,
		100,15,
		RGB(255,255,255));	
	TransparentBlt(hdc,
		(int)(m_tInfo.fX - 50) + m_tScroll.x,
		(int)(m_tInfo.fY - 10) + 100 + m_tScroll.y,
		100 * fPerHP,
		10,
		(*m_pMapBmp)["Bar_HP"]->GetMemDC(),
		0,0,
		100, 10,
		RGB(255,255,255));	
	TransparentBlt(hdc,
		(int)(m_tInfo.fX - 50) + m_tScroll.x,
		(int)(m_tInfo.fY - 10) + 110 + m_tScroll.y,
		100 * fPerMP,
		5,
		(*m_pMapBmp)["Bar_MP"]->GetMemDC(),
		0,0,
		100,5,
		RGB(255,255,255));	

	if(GetAsyncKeyState(VK_SHIFT) & 0x8000)
	{
		
		wsprintf(str, L"HP : %d / %d MP : %d / %d", (int)m_tInfo.iCurHP, (int)m_tInfo.iMaxHP,(int)m_tInfo.iCurMP, (int)m_tInfo.iMaxMP);
		//wsprintf(str, L"HP : %d Angle : %d", (int)m_tInfo.iCurHP, (int)m_fAngle);
		TextOut(hdc, (int)m_tInfo.fX - 22 + m_tScroll.x, (int)m_tInfo.fY - 40 + m_tScroll.y, str, lstrlen(str));
	}
}

void CBoss::Release(void)
{
}

void CBoss::Attack()
{
	list<CObj*>::iterator PlayerInfo = m_pPlayerList->begin();
	if((*PlayerInfo)->GetState() != STATE_DEAD)
	{
		/* 보스 체력이 80퍼 일 경우 2페이즈로 넘어감 */
		if(m_bFirstPhase && m_tInfo.iCurHP <= m_tInfo.iMaxHP * 0.8f)
		{
			m_dwState = STATE_CAST;
			m_iSummonNumber = 2;
			for (int i =0; i < m_iSummonNumber; ++i)
			{
				CMonster* TempMonster = new CMonster;
				TempMonster->Initialize();
				TempMonster->SetPlayerList(m_pPlayerList);
				TempMonster->SetBulletList(m_pBulletList);
				TempMonster->SetDamageList(m_pDamagedList);
				m_pMonsterList->push_back(TempMonster);
			}
			m_bFirstPhase = false;
			m_bSecondPhase = true;
		}
		/* 보스 체력이 60퍼 일 경우 3페이즈로 넘어감 */
		if(m_bSecondPhase && m_tInfo.iCurHP <= m_tInfo.iMaxHP * 0.6f)
		{
			m_dwState = STATE_CAST;
			for (int i = 0; i < m_iSummonNumber; ++i)
			{
				CObstacle* TempObstacle = new CObstacle;
				TempObstacle->Initialize();
				TempObstacle->SetDamageList(m_pDamagedList);
				m_pObstacleList->push_back(TempObstacle); //CObjFactory<CObstacle>::CreateObj()
			}
			m_dwAtk = GetTickCount() + 3000;
			m_bSecondPhase = false;
			m_bThirdPhase = true;
			++m_iSummonNumber;
			m_dwSummon = GetTickCount() + 20000;
		}
		/* 3페이즈 동안 일정 시간이 지나면 몬스터랑 장애물 재소환 */
		if(m_bThirdPhase && m_dwState != STATE_CAST)
		{
			if(m_dwSummon < GetTickCount())
			{
				int RandSummon = rand() % 2;
				if(RandSummon == 0)
				{
					m_dwState = STATE_CAST;
					for (int i = 0; i < m_iSummonNumber; ++i)
					{
						CMonster* TempMonster = new CMonster;
						TempMonster->Initialize();
						TempMonster->SetPlayerList(m_pPlayerList);
						TempMonster->SetBulletList(m_pBulletList);
						TempMonster->SetDamageList(m_pDamagedList);
						m_pMonsterList->push_back(TempMonster);
					}
					++m_iSummonNumber;
				}
				else
				{
					m_dwState = STATE_CAST;
					for (int i = 0; i < m_iSummonNumber; ++i)
					{
						CObstacle* TempObstacle = new CObstacle;
						TempObstacle->Initialize();
						TempObstacle->SetDamageList(m_pDamagedList);
						m_pObstacleList->push_back(TempObstacle);
					}
					++m_iSummonNumber;
				}
			}
		}
		/* 보스 체력이 10퍼 일 경우 4페이즈로 넘어감 */
		if(m_bThirdPhase && m_tInfo.iCurHP <= m_tInfo.iMaxHP * 0.1f)
		{
			m_dwState = STATE_CAST;
			m_dwAtk = GetTickCount() + 100;
			m_bThirdPhase = false;
			m_bFourthPhase = true;
		}

		/* 공격 */
		if(m_dwAtk < GetTickCount())
		{
			/* 2페이즈 공격 */
			if(m_dwState == STATE_CAST && m_bSecondPhase)
			{
				m_dwAtk = GetTickCount() + 1000;	
				CBullet* TempObj = new CBullet;
				TempObj->SetBullet("Bullet", m_tInfo.fX, m_tInfo.fY, 50, 50, m_fAngle, 10.f, 5, 9999);
				m_pBulletList->push_back(TempObj);
			}
			/* 3페이즈 공격 */
			else if(m_dwState == STATE_CAST && m_bThirdPhase)
			{
				if(m_pObstacleList->size() != 0)
				{
					m_dwAtk = GetTickCount() + 2000;
					m_tInfo.iCurMP += 10;
					if(m_tInfo.iCurMP >= m_tInfo.iMaxMP)
					{
						//CDevice::GetInstance()->SoundPlay(6,0);
						CBullet* TempObj = new CBullet;
						TempObj->SetBullet("Onekill", m_tInfo.fX, m_tInfo.fY, 1386, 1386, m_fAngle, 0, 200, 500);
						m_pBulletList->push_back(TempObj);
						m_tInfo.iCurMP = 0;
					}
				}
				else if(m_pMonsterList->size() != 0)
				{
					m_dwAtk = GetTickCount() + 1000;
					if(m_iCombo == 3)
					{
						CDevice::GetInstance()->SoundPlay(4,0);
						++m_iCombo;
						m_dwState = STATE_SKILL;
						/* 매 3회마다 연발탄 */
						for(int i = 0 ; i < 10 ; ++i)
						{
							float fRandSpeed = rand() % 10 + 5;
							float fRandAngle = rand() % 2 == 1 ? rand() % 20 : rand() % 20 * -1;
							CBullet* TempObj = new CBullet;
							TempObj->SetBullet("Bullet", m_tInfo.fX, m_tInfo.fY, 50, 50, m_fAngle + fRandAngle, fRandSpeed, 5, 9999);
							m_pBulletList->push_back(TempObj);
						}
						m_iCombo = 0;
					}
					else
					{
						/* 장거리 공격 */
						if(CMathMgr::GetDistance(this, *m_pPlayerList->begin()) > 200)
						{
							CDevice::GetInstance()->SoundPlay(4,0);
							m_dwState = STATE_SKILL;
							CBullet* TempObj = new CBullet;
							TempObj->SetBullet("Bullet", m_tInfo.fX, m_tInfo.fY, 50, 50, m_fAngle, 10.f, 5, 9999);
							m_pBulletList->push_back(TempObj);
						}
						/*근거리 공격*/
						else
						{	
							CDevice::GetInstance()->SoundPlay(4,0);
							m_dwState = STATE_ATTACK;
							CBullet* TempObj = new CBullet;
							TempObj->SetBullet("", m_tInfo.fX, m_tInfo.fY, 500, 500, m_fAngle, 0, m_tInfo.iDamage, 0);
							m_pBulletList->push_back(TempObj);
						}
					}
				}
			}
			else
			{
				++m_iCombo;
				if(m_bThirdPhase)
				{
					m_dwAtk = GetTickCount() + 1500;	
				}
				else
					m_dwAtk = GetTickCount() + 2000;		

				if(m_bFourthPhase && m_dwState == STATE_CAST)
				{
					/* 4페이즈 전방위 공격 */
					for(int i = 0 ; i < 36 ; ++i)
					{
						CBullet* TempObj = new CBullet;
						TempObj->SetBullet("Bullet", m_tInfo.fX, m_tInfo.fY, 50, 50, i * 10, 10.f, 1, 9999);
						m_pBulletList->push_back(TempObj);
					}
				}
				else
				{
					if(m_iCombo == 3)
					{
						m_dwState = STATE_SKILL;
						/* 매 3회마다 연발탄 */
						for(int i = 0 ; i < 10 ; ++i)
						{
							CDevice::GetInstance()->SoundPlay(4,0);
							float fRandSpeed = rand() % 10 + 5;
							float fRandAngle = rand() % 2 == 1 ? rand() % 20 : rand() % 20 * -1;
							CBullet* TempObj = new CBullet;
							TempObj->SetBullet("Bullet", m_tInfo.fX, m_tInfo.fY, 50, 50, m_fAngle + fRandAngle, fRandSpeed, 5, 9999);
							m_pBulletList->push_back(TempObj);
						}
						m_iCombo = 0;
					}
					else
					{
						/* 장거리 공격 */
						if(CMathMgr::GetDistance(this, *m_pPlayerList->begin()) > 200)
						{
							CDevice::GetInstance()->SoundPlay(4,0);
							m_dwState = STATE_SKILL;
							CBullet* TempObj = new CBullet;
							TempObj->SetBullet("Bullet", m_tInfo.fX, m_tInfo.fY, 50, 50, m_fAngle, 10.f, 5, 9999);
							m_pBulletList->push_back(TempObj);
						}
						/*근거리 공격*/
						else
						{	
							CDevice::GetInstance()->SoundPlay(4,0);
							m_dwState = STATE_ATTACK;
							CBullet* TempObj = new CBullet;
							TempObj->SetBullet("", m_tInfo.fX, m_tInfo.fY, 500, 500, m_fAngle, 0, m_tInfo.iDamage, 0);
							m_pBulletList->push_back(TempObj);
						}
					}
				}
			}
		}
		if(m_bThirdPhase && m_dwState == STATE_CAST && m_pMonsterList->size() == 0 && m_pObstacleList->size() == 0)
			m_dwSummon = GetTickCount() + 5000;
	}
}


void CBoss::Motion(void)
{
	if(m_pMonsterList->size() != 0 || m_pObstacleList->size() != 0 || m_bFourthPhase)
		m_dwState = STATE_CAST;

	/* 대기 모션 */
	if(m_dwState  & STATE_STAND)
	{
		m_tInfo.iCurMP = 0;
		SetScene(5,0,100);
	}

	/* 공격 모션 */
	else if(m_dwState  & STATE_ATTACK)
	{
		SetScene(9,1,50);
	}
	/* 캐스팅 모션 */
	else if(m_dwState  & STATE_CAST)
	{
		SetScene(9,2,100);
	}
	/* 특수스킬 모션 */
	else if(m_dwState  & STATE_SKILL)
	{
		SetScene(4,3,100);
	}
	else if(m_dwState & STATE_DEAD)
	{
		m_strDrawID = "Witch_DEAD";
		SetScene(5,0,200);
	}
}

void CBoss::Frame(void)
{
	if(m_dwTime + m_tFrame.dwTime < GetTickCount())
	{
		m_dwTime = GetTickCount();
		++m_tFrame.iStart;

		if(m_dwState != STATE_DEAD)
		{
			if(m_tFrame.iStart >= m_tFrame.iLast )
			{
				m_tFrame.iStart = 0;
				if(m_dwState != STATE_STAND)
					m_dwState = STATE_STAND;
			}
		}
	}
}

void CBoss::Direction(void)
{
	if(0 <= m_fAngle && m_fAngle < 90)
	{
		m_strDrawID =  "Witch_RU";
	}
	else if(90 <= m_fAngle && m_fAngle < 180)
	{
		m_strDrawID =  "Witch_LU";
	}
	else if(180 <= m_fAngle && m_fAngle < 270)
	{
		m_strDrawID =  "Witch_LD";
	}
	else if(270 <= m_fAngle && m_fAngle < 360)
	{
		m_strDrawID =  "Witch_RD";
	}
}

void CBoss::SetScene(int _iLast,int _iScene,DWORD _dwTime)
{
	if(m_tFrame.iScene != _iScene)
		m_tFrame.iStart = 0;

	m_tFrame.iLast =  _iLast;
	m_tFrame.iScene = _iScene;
	m_tFrame.dwTime = _dwTime;
}

CBoss::CBoss(void)
{
}


CBoss::~CBoss(void)
{
	Release();
}
