#pragma once
#include "BaseBomb.h"
class CRocketBomb :
	public CBaseBomb
{
public:
	CRocketBomb();
	~CRocketBomb();

	void Update();	//更新
	void OnMouseDown(SEvent event);
	void OnMouseMove(SEvent event);
	void OnMouseUp(SEvent event);
	void FreeBomb(const D3DXVECTOR2 &dir, float speed);

protected:
private:
	D3DXVECTOR2		m_vecBeginPos;		//鼠标移动的开始位置
	D3DXVECTOR2		m_vecEndPos;		//鼠标移动的结束位置
	BOOL			m_bPressed;			//鼠标是否按下
};

