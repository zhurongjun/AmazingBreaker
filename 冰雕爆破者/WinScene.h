#pragma once

class CWinScene :
	public CGameScene
{
public:
	CWinScene();
	~CWinScene();

	void Init();	//��ʼ��
	void Render();	//����
	void Update();	//����
	void TurnIn();	//�����׼������
	void TurnOut();	//�����׼������

protected:
	void ToNextLevel(int num);
	void ReplayLevel(int num);
	void ToSelect(int num);
private:
	CSprite *		m_pBackground;	//����ͼ
	CSprite *		m_pLevel;		//Level�������
	CSprite *		m_pLevelNums;	//�ؿ�����
	CSprite	*		m_pComplete;	//Complete�������
	CSprite *		m_pStar;		//�������
	CSprite *		m_pStarBk;		//���Ǳ������
	CRoundButton *	m_pReplay;		//����
	CRoundButton *	m_pSelectLevel;	//ѡ��ؿ�
	CRoundButton *	m_pNextLevel;	//��һ�ذ�ť
	INT				m_iCurLevel;	//Ҫ����Ĺؿ���
	INT				m_iScore;		//�÷�
	INT				m_iTurnInCount;	//���붯������
};

