#pragma once

class CPlayer;
class CMainGame
{
private:
	HDC m_hdc;
	CPlayer* m_pPlayer;
public:
	void Initialize(void);
	void Progress(void);
	void Render(void);
	void Release(void);

public:
	CMainGame(void);
	virtual ~CMainGame(void);
};

