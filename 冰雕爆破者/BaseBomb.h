#pragma once
#include "Sprite.h"

//����ը���Ļ���
class CBaseBomb :
	public CSprite ,
	public CMouseListener
{
public:
	enum eSTATE
	{
		PREPAREING,	//׼����(����״̬��׼������)
		FLYING,		//������(����״̬)
		SECOND,		//����״̬
		BOMB,		//��ը��
		DESTORY		//�Ѿ����ƻ�
	};
	enum eTYPE {
		NORMAL,		//��ͨը��
		TRIPLE,		//����ɢ��
		ROTATE,		//��ת����
		GHOST,		//�����ӵ�
		ROCKET,		//���
		STATIC		//��̬Ƕ��ը��
	};
public:
	CBaseBomb();
	virtual ~CBaseBomb();

	void SetDamping(float damping) { m_fDamping = damping; }	//��������
	float GetDamping() { return m_fDamping; }	//��ȡ����
	void SetSpeed(float speed) { m_fSpeed = speed; }	//�����ٶ�
	float GetSpeed() { return m_fSpeed; }	//��ȡ����
	void SetDir(const D3DXVECTOR2 & dir);	//���÷���
	D3DXVECTOR2 GetDir() { return m_vecDir; }	//��ȡ����
	eSTATE GetState() { return m_eState; }	//��ȡ״̬
	eTYPE  GetBombType() { return m_eType; }	//��ȡ����

	virtual void OnMouseDown(SEvent){}
	virtual void OnMouseUp(SEvent){}
	virtual void OnMouseMove(SEvent){}
	virtual void OnMouseWheel(SEvent){}

	virtual void FreeBomb(const D3DXVECTOR2 &dir, float speed);	//�ͷ�ը��������һ�����ٶȺͷ���
	virtual void Update();	//����

protected:
	float		m_fDamping;		//ը���ķ�������
	float		m_fDampFactor;	//ը��������ϵ��
	float		m_fSpeed;		//ը�����е��ٶ�
 	float		m_colRound;		//��ײ�뾶
	D3DXVECTOR2	m_vecDir;		//ը�����еķ���
	eSTATE		m_eState;		//ը����״̬
	INT			m_iStateCount;	//״̬����
	INT			m_iStateParam;	//״̬����
	eTYPE		m_eType;		//�ӵ�����
private:
};

