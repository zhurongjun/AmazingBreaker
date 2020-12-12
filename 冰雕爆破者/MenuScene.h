#pragma once
#include "GameScene.h"
class CMenuScene :
	public CGameScene
{
public:
	CMenuScene();
	~CMenuScene();

	void Init();	//初始化
	void Render();	//绘制
	void Update();	//更新
	void TurnIn();	//进入的准备工作
	void TurnOut();	//退出的后续工作

protected:

private:
	void ToSelect(int num);
	void ContinueGame(int num);
	void ReplayGame(int num);

private:
	CSprite *		m_pBackground;	//背景图
	CRoundButton *	m_pContinue;	//继续按钮
	CRoundButton *	m_pSelect;		//选关按钮
	CRoundButton *	m_pReplay;		//重玩按钮

};

