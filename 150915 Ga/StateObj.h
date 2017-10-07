#pragma once
class CBitMap;
class CStateObj
{
protected:
	map<string,CBitMap*> m_MapBmp;
public:
	virtual void Initialize(void) =0;
	virtual void Progress(void)=0;
	virtual void Render(HDC hdc)=0;
	virtual void Release(void)=0;
	
public:
	CStateObj(void);
	virtual ~CStateObj(void);
};

