#pragma once

class CBaseBomb;

class CBombShooter :
	public CMouseListener
{
private:
	const static D3DXVECTOR2 s_BombStackPos[5];
	const static float s_fBigScalling;
	const static float s_fSmallScalling;
public:
	enum eSTATE {
		NORMAL,		//ͨ��״̬
		RESET = 1,	//����(��λ)
		SHOOTED = 2,//����(���)
		WAIT = 3,	//�ȴ�(�ȴ�ը����ը)
		FILL = 4,	//���(���ը��)
		ADD = 5,	//���(���ը��)
		EXCHANGE	//����(�õ����ڵ�ը���滻��ǰը��)
	};

public:
	CBombShooter();
	~CBombShooter();

	//��ʼ��
	void init();
	void reset();	//����

	//�¼�����
	void OnMouseDown(SEvent);
	void OnMouseUp(SEvent);
	void OnMouseMove(SEvent);

	//����
	void Draw();
	//����
	void Update();

	//��������
	void SetDestroyPre(float precent, bool anim);
	float GetDestoryPre() { return m_fDestoryPre; }	//��ȡ�ƻ�����
	int  GetNowBomb() { return m_listBomb.size() + m_curBomb ? 1 : 0; }
	eSTATE GetState() { return m_eState; }	//��ȡ״̬

	//���ը��
	void AddBomb(CBaseBomb * bomb);
	void AddBomb(vector<CBaseBomb *> * vec);
	//���ը��
	void ClearBomb();
	//����ը����
	void UpdateBombStack();
	//����ը��
	void ExchangeBomb();
	int GetBout() { return m_iCurBont; }	//��ȡ�غ���
	void SetBout(int bout) { m_iCurBont = bout; }	//���ûغ���
protected:
	void UpdateState();		//����״̬
private:
	int			m_preAnimCount;	//�ٷֱȵ�������������
	bool		m_bSelect;		//�Ƿ�ѡ��
	float		m_fDestoryPre;	//�ƻ�����(�ٷֱ�)
	float		m_fShootSpeed;	//�����ٶ�
	float		m_fMaxRound;	//�������������뾶
	float		m_fMinRound;	//������������С�İ뾶
	D3DXVECTOR2	m_vecNorPos;	//ͨ���µ�λ��
	D3DXVECTOR2 m_vecShootDir;	//���䷽��
	D3DXVECTOR2	m_vecCenterPos;	//�������ĵ�
	CBaseBomb *	m_curBomb;		//��������ǰװ�ص�ը��
	eSTATE		m_eState;		//��ǰ��״̬
	int			m_stateCount;	//״̬����
	int			m_stateParam;	//״̬����
	int			m_iCurBont;		//��ǰ�غ�
	CAnimHander *		m_animBlend;	//����Ч��
	list<CBaseBomb *>	m_listBomb;		//�������ڵ�ը��
	CAnimHander *		m_Special;		//���⶯��
	vector<CBaseBomb *> m_InsertBomb;	//����ӵ�ը��

	//����ͼƬ
	CSprite *	m_pArrow;		//ָ���ͷ
	CSprite *	m_pPrecentBar;	//�ƻ���������ʾ
	CSprite *	m_pMainUI;		//��UI
	CSprite *	m_pLine;		//�ָ���
	CSprite *	m_pSpringL;		//����
	CSprite *	m_pSpringR;		//����
	CSprite *	m_pNode;		//���ɵ����ӽڵ�
	CSprite *	m_pHolder;		//ץסը����holder
	CSprite *	m_pPrecentText;	//�ٷֱȵ��ַ�
	CSprite *	m_pSpecialSp;	//�������⶯���ľ���
};

