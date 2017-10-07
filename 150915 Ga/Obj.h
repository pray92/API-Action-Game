#pragma once

class CBitMap;

class CObj
{
protected:
	INFO							m_tInfo;
	static POINTFLOAT				m_tScroll;
	static POINTFLOAT				m_tOffset;
	static map<string,CBitMap*>*	m_pMapBmp;
	float							m_fAngle;
	string							m_strDrawID;
	DWORD							m_dwState;
	
	bool							m_bFourthPhase;
	
	list<CObj*>*					m_pBulletList;
	list<CObj*>*					m_pMonsterList;
	list<CObj*>*					m_pPlayerList;
	list<CObj*>*					m_pObstacleList;
	list<CObj*>*					m_pDamagedList;
public:
	/* 탄환 */
	void SetBulletList(list<CObj*>* _pBulletList)
	{
		m_pBulletList = _pBulletList;
	}
	/* 플레이어*/
	void SetPlayerList(list<CObj*>* _pPlayerList)
	{
		m_pPlayerList = _pPlayerList;
	}
	/* 몬스터 소환 */
	void SetMonsterList(list<CObj*>* _pMonsterList)
	{
		m_pMonsterList = _pMonsterList;
	}
	/* 장애물 소환 */
	void SetObstacleList(list<CObj*>* _pObstacleList)
	{
		m_pObstacleList = _pObstacleList;
	}
	/* 피격 표시 */
	void SetDamageList(list<CObj*>* _pDamagedList)
	{
		m_pDamagedList = _pDamagedList;
	}
	/* 탄환 정보 */
	list<CObj*>* GetBulletList() const
	{
		return m_pBulletList;
	}
	/* 플레이어 정보 */
	list<CObj*>* GetPlayerList() const
	{
		return m_pPlayerList;
	}
	/* 몬스터 정보 */
	list<CObj*>* GetMonsterList() const
	{
		return m_pMonsterList;
	}
	/* 장애물 정보 */
	list<CObj*>* GetObstacleList() const
	{
		return m_pObstacleList;
	}
	/* 피격 정보 */
	list<CObj*>*  GetDamageList()
	{
		return m_pDamagedList;
	}
	
	/* 4페이즈 확인 */
	bool GetFourthPhase() const
	{
		return m_bFourthPhase;
	}
	void SetDrawID(string _strDrawID)
	{
		m_strDrawID = _strDrawID;
	}
public:
	string GetDrawID() const
	{
		return m_strDrawID;
	}
	DWORD GetState() const
	{
		return m_dwState;
	}
	void BoundaryCheck()
	{
		if(m_tInfo.fX < 410)
			m_tInfo.fX = 410;
		if(m_tInfo.fX > 1750)
			m_tInfo.fX = 1750;
		if(m_tInfo.fY < 200)
			m_tInfo.fY = 200;
		if(m_tInfo.fY > 900)
			m_tInfo.fY = 900;
	}
	float GetAngle()
	{
		return m_fAngle;
	}
	static void SetBmp(map<string,CBitMap*>* _pMapBmp)
	{
		 m_pMapBmp = _pMapBmp;
	}

	const POINTFLOAT* GetScroll()
	{
		return  &m_tScroll;
	}
	INFO* GetInfo()
	{
		return &m_tInfo;
	}

	RECT GetRect(void)
	{
		RECT rt = 
		{
			int(m_tInfo.fX - m_tInfo.fCX /2.f),
			int(m_tInfo.fY - m_tInfo.fCY /2.f),
			int(m_tInfo.fX + m_tInfo.fCX /2.f),
			int(m_tInfo.fY + m_tInfo.fCY /2.f),
		};
		return rt;
	}

	void SetPos(float _x,float _y)
	{
		m_tInfo.fX = _x;
		m_tInfo.fY = _y;
	}


public:
	virtual void Initialize(void) = 0;
	virtual int Progress(void) = 0;
	virtual void Render(HDC hdc) = 0;
	virtual void Release(void) = 0;
public:
	CObj(void);
	virtual ~CObj(void);
};

