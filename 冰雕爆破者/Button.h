#pragma once
class Button :
	public IAnimable,
	public CMouseListener,
	public CCollisionObj
{
public:
	Button(TSTRING Normal,TSTRING MouseIn,TSTRING Pressed);
	~Button();

	//���Զ�Ч���ӿڵĺ���
	virtual void SetTransparent(int transpatent) {}//����͸����
	virtual int GetTransparent() { return 0; }//���͸����
	virtual void SetDesPos(D3DXVECTOR2 desPos);	//������Ļ����λ��
	virtual D3DXVECTOR2 GetDesPos() 
	{ 
		return m_vecDesPos; 
	}	//����Ŀ������
	virtual void Draw(bool updateMatrix = true) {}	//����
	virtual void SetXScale(float xScale) {}	//����X������
	virtual void SetYScale(float yScale) {}	//����Y������
	virtual float GetXScale() { return 0; }	//��ȡX�������
	virtual float GetYScale() { return 0; }	//��ȡY�������
	virtual void SetAngle(float angle) {} //������ת�Ƕ�
	virtual float GetAngle() { return 0; }	//��ȡ��ת�Ƕ�

	void Render();	//����
	void OnMouseDown(SEvent event);	//������갴�µĺ���
	void OnMouseUp(SEvent event);	//������굯��ĺ���
	void OnMouseMove(SEvent  event);//��������ƶ��ĺ���

	RECT GetRect(int width, int height);	//��ȡRC
	void Refresh();	//ˢ��
	void SetColorUse(BOOL useColor) { m_bUseColor = useColor; }	//�����Ƿ�ʹ����ɫ

	void  setCallBack(PFUN callBack) { this->m_pCallback = callBack; }	//���ûص�����
	void SetDownSound(TSTRING sound) { m_sDowmSound = sound; }	//���ð��µ�����

private:
	CAnimHander *	m_pNormal;		//ͨ��״̬
	CAnimHander *	m_pMouseIn;		//������
	CAnimHander *	m_pPressed;		//����
	CAnimHander *	m_curState;		//״̬
	D3DXVECTOR2		m_vecDesPos;	//��������
	TSTRING		m_sDowmSound;	//���µ�����
	PFUN		m_pCallback;	//�ص�����
	BOOL		m_bIsPressed;	//�Ƿ񱻰���
	BOOL		m_bMouseIn;	//�Ƿ񱻰���
	BOOL		m_bUseColor;//�Ƿ�ʹ����ɫ
};

