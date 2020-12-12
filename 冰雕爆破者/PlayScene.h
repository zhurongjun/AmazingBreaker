#pragma once
#include "GameScene.h"

class CBombShooter;

class CPlayScene :
	public CGameScene
{
public:
	CPlayScene();
	~CPlayScene();

	void Init();	//��ʼ��
	void Render();	//����
	void Update();	//����
	void TurnIn();	//�����׼������
	void TurnOut();	//�˳��ĺ�������

protected:
private:
	void Exchange(int num);
	void ToMenu(int num);
private:
	CLevelManager * m_pLevelManager;//�ؿ�����
	CBombShooter *	m_pShooter;		//ը��������
	Button  *		m_pExchange;	//������ť
	Button  *		m_pMenu;		//�˵���ť
	int				m_iStateCount;	//״̬����
};

