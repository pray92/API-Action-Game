#pragma once
#include "obj.h"

class CPlayer :
	public CObj
{
private:
	int			m_temp;							//ĳ���� ��ǥ �缳��
	float		m_fDistance;					//ĳ���� �̵� �Ÿ� ����
	POINTFLOAT	m_ptMouse;						//�̵� ��ǥ
	float		m_fSpeed;

	DWORD		m_dwKey;						//Ű �Է�
	DWORD		m_dwTime;
	
	DWORD		m_dwRecoveryMP;					//��� ȸ�� ������
	DWORD		m_dwRecoveryHP;					//ü�� ȸ�� ������
	DWORD		m_dwDash;						//���
	DWORD		m_dwAtk;						//����
	DWORD		m_dwStk1;						//��Ÿ1
	DWORD		m_dwStk2;						//��Ÿ2
	DWORD		m_dwStk3;						//��Ÿ3
	DWORD		m_dwStk4;						//��Ÿ4
	DWORD		m_dwStk5;						//��Ÿ5
	int			m_iCombo;						//�޺�
	DWORD		m_dwLastPressed;				//�޺� ��ȿȭ

	FRAME	m_tFrame;

	list<CObj*>* m_pBulletList;					//���� ����
private:
	void Frame(void);
	void Direction(void);
	void Motion(void);
	void SetScene(int _iLast,int _iScene,DWORD _dwTime);
	void CheckKey();
private:
	void RecoverMP();
	void RecoverHP();
	void SkillMove(int _iTemp);								//Ư�� ��ų �Է� �� �̵�
	void Scroll();
	void Attack();											//����
	void Strike1();											//��Ÿ1
	void Strike2();											//��Ÿ2
	void Strike3();											//��Ÿ3
	void Strike4();											//��Ÿ4
	void Strike5();											//��Ÿ5
public:
	void SetBulletList(list<CObj*>* _pBulletList)
	{
		m_pBulletList = _pBulletList;
	}
public:
	void Initialize(void);
	int Progress(void);
	void Render(HDC hdc);
	void Release(void);
public:
	CPlayer(void);
	virtual ~CPlayer(void);
};


