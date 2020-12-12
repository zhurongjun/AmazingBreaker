#pragma once

class CLevelButtom :
	public CSprite,
	public CCollisionObj,
	public CMouseListener
{
public:
	CLevelButtom();
	~CLevelButtom();

	void Init();

	void Draw();	//���صĻ��ƺ���
	void SetDesPos(D3DXVECTOR2 desPos);	//���ص�λ�����ú���
	void SetNum(int num) { m_iNum = num; }	//��������
	void SetLocked(bool locked);	//�����Ƿ���������
	void SetTieDraw(bool draw) { m_bDrawTie = draw; }	//���ô��ӵĻ���
	void SetStar(int star) { m_iStar = star; if (m_iStar > 3)m_iStar = 3; if (m_iStar < 0)m_iStar = 0; }
	void SetBackfun(PFUN fun) { m_pBackFun = fun; }

	virtual void OnMouseDown(SEvent event);
	virtual void OnMouseUp(SEvent event);
	virtual void OnMouseMove(SEvent event);

protected:
private:
	int			m_iNum;			//��ǰ������
	int			m_iStar;		//������
	CSprite *	m_pNumDrawer;	//�������ֵľ���
	CSprite *	m_pTie;			//���ƴ��ӵľ���
	CSprite *	m_pStar;		//�������ǵľ���
	BOOL		m_bDrawTie;		//�Ƿ���ƴ���
	BOOL		m_bLocked;		//�Ƿ�����
	BOOL		m_bPressed;		//�Ƿ񱻰���
	BOOL		m_bMouseIn;		//����Ƿ���������
	PFUN		m_pBackFun;		//�ص�����
};

