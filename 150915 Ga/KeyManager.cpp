#include "StdAfx.h"
#include "KeyManager.h"


CKeyManager::CKeyManager(void)
{
}


CKeyManager::~CKeyManager(void)
{
}

void CKeyManager::CheckKey(void)
{
	m_dwKey = 0;

	if(GetAsyncKeyState(VK_UP) & 0x8000){
		m_dwKey |= KEY_UP;
	}
	if(GetAsyncKeyState(VK_DOWN) & 0x8000){
		m_dwKey |= KEY_DOWN;
	}
	if(GetAsyncKeyState(VK_LEFT) & 0x8000){
		m_dwKey |= KEY_LEFT;
	}
	if(GetAsyncKeyState(VK_RIGHT) & 0x8000){
		m_dwKey |= KEY_RIGHT;
	}
	if(GetAsyncKeyState(VK_SPACE) & 0x8000){
		m_dwKey |= KEY_SPACE;
	}

	if(GetAsyncKeyState(VK_LBUTTON) & 0x8000){
		m_dwKey |= KEY_LBUTTON;
	}

	if(GetAsyncKeyState('A') & 0x8000){
		m_dwKey |= KEY_A;
	}

	if(GetAsyncKeyState('S') & 0x8000){
		m_dwKey |= KEY_S;
	}
	
	if(GetAsyncKeyState('D') & 0x8000){
		m_dwKey |= KEY_D;
	}

	if(GetAsyncKeyState(VK_ESCAPE) & 0x8000){
		m_dwKey |= KEY_ESC;
	}
}
