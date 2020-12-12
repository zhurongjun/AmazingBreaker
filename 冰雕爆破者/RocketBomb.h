#pragma once
#include "BaseBomb.h"
class CRocketBomb :
	public CBaseBomb
{
public:
	CRocketBomb();
	~CRocketBomb();

	void Update();	//����
	void OnMouseDown(SEvent event);
	void OnMouseMove(SEvent event);
	void OnMouseUp(SEvent event);
	void FreeBomb(const D3DXVECTOR2 &dir, float speed);

protected:
private:
	D3DXVECTOR2		m_vecBeginPos;		//����ƶ��Ŀ�ʼλ��
	D3DXVECTOR2		m_vecEndPos;		//����ƶ��Ľ���λ��
	BOOL			m_bPressed;			//����Ƿ���
};

