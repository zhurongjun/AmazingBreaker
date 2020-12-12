#pragma once
#include "BaseBomb.h"

class CBombLine;

class CStaticBomb :
	public CBaseBomb
{
public:
	enum ECOLOR
	{
		RED,
		GREEN,
		VIOLET
	};
public:
	CStaticBomb();
	~CStaticBomb();

	virtual void Update();	//更新

	void SetColor(ECOLOR color);
	virtual void Draw(bool updateMatrix = true); //绘制
	void DrawLine();	//绘制链接线
	void Bomb();	//引爆静态炸弹
	void AddLine(D3DXVECTOR2 pos);
	void DeleteLine(D3DXVECTOR2 pos);

protected:
private:
	ECOLOR		m_color;		//静态炸弹的颜色
	list<CBombLine *>	m_listLines;	//维护的一个指向线列表
};

