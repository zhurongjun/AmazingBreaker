#pragma once
#include "GameScene.h"
class CMenuScene :
	public CGameScene
{
public:
	CMenuScene();
	~CMenuScene();

	void Init();	//��ʼ��
	void Render();	//����
	void Update();	//����
	void TurnIn();	//�����׼������
	void TurnOut();	//�˳��ĺ�������

protected:

private:
	void ToSelect(int num);
	void ContinueGame(int num);
	void ReplayGame(int num);

private:
	CSprite *		m_pBackground;	//����ͼ
	CRoundButton *	m_pContinue;	//������ť
	CRoundButton *	m_pSelect;		//ѡ�ذ�ť
	CRoundButton *	m_pReplay;		//���水ť

};

