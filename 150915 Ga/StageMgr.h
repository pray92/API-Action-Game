#pragma once
class CStateObj;
class CStageMgr
{
private:
	CStateObj*	m_pState;
private:
	static CStageMgr* m_pInst;
public:
	CStateObj* GetState()
	{
		return m_pState;
	}
	static CStageMgr* GetInst(void)
	{
		if(m_pInst == NULL)
			m_pInst = new CStageMgr;

		return m_pInst;
	}

	void DestroyInst()
	{
		delete m_pInst;
		m_pInst = NULL;
	}
	
	void SetState(STATEID _eStage);

	void Release();

	virtual ~CStageMgr(void);
private:
	CStageMgr(void);
};

