#pragma once
#include "MySingleton.h"
class CKeyManager //: public MySingleton<CKeyManager>
{
private:
	DWORD	m_dwKey;
public:
	DWORD GetKey(void) const {return m_dwKey;};
	bool PressKey(DWORD _Key) const {
		return m_dwKey & KEY_UP;
	};
public:
	void CheckKey(void);

	DECLARE_SINGLETON(CKeyManager);
public:
	CKeyManager(void);
	virtual ~CKeyManager(void);
};

