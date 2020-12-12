#pragma once
#include "GameScene.h"
class CWaitScene :
	public CGameScene
{
public:
	CWaitScene();
	~CWaitScene();

	void Init();	//��ʼ��
	void Render();	//����
	void Update();	//����
	void TurnIn();	//�����׼������
	void TurnOut();	//�˳��ĺ�������


protected:

private:
	CSprite *	m_pBackground;	//����
	CSprite *	m_pLoading;		//������ͼ��
	CSprite *	m_pLevel;		//LEVELͼ��
	CSprite *	m_pLevelNum;	//level����ͼ�� 
	UINT		m_curTick;		//�ȴ�����
	UINT		m_maxTick;		//������
	UINT		m_nextLevel;	//���عؿ�
};

