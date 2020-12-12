#pragma once
#include "GameScene.h"

class CBombShooter;

class CPlayScene :
	public CGameScene
{
public:
	CPlayScene();
	~CPlayScene();

	void Init();	//初始化
	void Render();	//绘制
	void Update();	//更新
	void TurnIn();	//进入的准备工作
	void TurnOut();	//退出的后续工作

protected:
private:
	void Exchange(int num);
	void ToMenu(int num);
private:
	CLevelManager * m_pLevelManager;//关卡管理
	CBombShooter *	m_pShooter;		//炸弹发射器
	Button  *		m_pExchange;	//交换按钮
	Button  *		m_pMenu;		//菜单按钮
	int				m_iStateCount;	//状态计数
};

