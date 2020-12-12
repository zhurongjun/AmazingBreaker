#pragma once
#include "GameScene.h"
class CWaitScene :
	public CGameScene
{
public:
	CWaitScene();
	~CWaitScene();

	void Init();	//初始化
	void Render();	//绘制
	void Update();	//更新
	void TurnIn();	//进入的准备工作
	void TurnOut();	//退出的后续工作


protected:

private:
	CSprite *	m_pBackground;	//背景
	CSprite *	m_pLoading;		//加载中图标
	CSprite *	m_pLevel;		//LEVEL图标
	CSprite *	m_pLevelNum;	//level数字图标 
	UINT		m_curTick;		//等待计数
	UINT		m_maxTick;		//最大计数
	UINT		m_nextLevel;	//加载关卡
};

