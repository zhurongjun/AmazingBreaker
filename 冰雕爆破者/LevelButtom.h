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

	void Draw();	//重载的绘制函数
	void SetDesPos(D3DXVECTOR2 desPos);	//重载的位置设置函数
	void SetNum(int num) { m_iNum = num; }	//设置数字
	void SetLocked(bool locked);	//设置是否是锁定的
	void SetTieDraw(bool draw) { m_bDrawTie = draw; }	//设置带子的绘制
	void SetStar(int star) { m_iStar = star; if (m_iStar > 3)m_iStar = 3; if (m_iStar < 0)m_iStar = 0; }
	void SetBackfun(PFUN fun) { m_pBackFun = fun; }

	virtual void OnMouseDown(SEvent event);
	virtual void OnMouseUp(SEvent event);
	virtual void OnMouseMove(SEvent event);

protected:
private:
	int			m_iNum;			//当前的数字
	int			m_iStar;		//星星数
	CSprite *	m_pNumDrawer;	//绘制数字的精灵
	CSprite *	m_pTie;			//绘制带子的精灵
	CSprite *	m_pStar;		//绘制星星的精灵
	BOOL		m_bDrawTie;		//是否绘制带子
	BOOL		m_bLocked;		//是否被锁定
	BOOL		m_bPressed;		//是否被按下
	BOOL		m_bMouseIn;		//鼠标是否在区域内
	PFUN		m_pBackFun;		//回调函数
};

