#pragma once
#include "BaseBomb.h"
class CRotateBomb :
	public CBaseBomb
{
public:
	CRotateBomb();
	~CRotateBomb();

	virtual void Update();	//¸üÐÂ
	void Draw(bool updateMatrix = true);
	virtual void OnMouseDown(SEvent event);
	void FreeBomb(const D3DXVECTOR2 &dir, float speed);
	void SetDesPos(D3DXVECTOR2 desPos);

protected:
private:
	CStaticBomb *		m_FillBombs[3];		//ÔØºÉÕ¨µ¯



};

