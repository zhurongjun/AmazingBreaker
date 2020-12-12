#pragma once

class CLevelButtom;

class CSelectScene :
	public CGameScene,
	public CMouseListener
{
	enum ESTATE
	{
		NORMAL,
		TURNIN,
		NEXT,
		LAST
	};
public:
	CSelectScene();
	~CSelectScene();

	void Init();	//��ʼ��
	void Render();	//����
	void Update();	//����
	void TurnIn();	//�����׼������
	void TurnOut();	//�˳��ĺ�������

	virtual void OnMouseDown(SEvent event);
	virtual void OnMouseUp(SEvent event);
	virtual void OnMouseMove(SEvent event);
	virtual void OnMouseWheel(SEvent event);

protected:
private:
	void backToMain(int num);
	void beginGame(int num);

	//ѡ�������Ϣ���
	int			m_iCurStage;	//��ǰ����
	ESTATE		m_eState;		//��ǰ��״̬
	int			m_iStateCount;	//״̬����

	//����UI���
	Button	*	m_pBackButtom;	//���ذ�ť
	CSprite *	m_pBackground;	//����ͼ
	CSprite *	m_pStage;		//StageUI
	CSprite *	m_pStageNum;	//�ؿ�������
	CSprite *	m_pLine;		//��
	
	CLevelButtom *	m_pButtoms[6][20];	//���еİ�ť(ɥ�Ĳ���)
	
};

