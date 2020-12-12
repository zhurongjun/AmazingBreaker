#pragma once
#include "GameScene.h"
class CFailScene :
	public CGameScene
{
public:
	CFailScene();
	~CFailScene();

	void Init();	//��ʼ��
	void Render();	//����
	void Update();	//����
	void TurnIn();	//�����׼������
	void TurnOut();	//�����׼������

protected:
	void ReplayLevel(int num);
	void ToSelect(int num);
private:
	CSprite *		m_pBackground;	//����ͼ
	CSprite *		m_pLevel;		//Level�������
	CSprite *		m_pLevelNums;	//�ؿ�����
	CSprite	*		m_pFailed;		//Failed�������
	CSprite *		m_pPreNums;		//�ƻ��ٷֱ�����
	CSprite *		m_pMinPre;		//��С�ٷֱ����
	CRoundButton *	m_pReplay;		//����
	CRoundButton *	m_pSelectLevel;	//ѡ��ؿ�
	INT				m_iCurLevel;	//Ҫ����Ĺؿ���
	INT				m_iPrecent;		//�ƻ�����
	INT				m_iTurnInCount;	//���붯������
};

