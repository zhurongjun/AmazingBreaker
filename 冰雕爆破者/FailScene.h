#pragma once
#include "GameScene.h"
class CFailScene :
	public CGameScene
{
public:
	CFailScene();
	~CFailScene();

	void Init();	//初始化
	void Render();	//绘制
	void Update();	//更新
	void TurnIn();	//进入的准备工作
	void TurnOut();	//进入的准备工作

protected:
	void ReplayLevel(int num);
	void ToSelect(int num);
private:
	CSprite *		m_pBackground;	//背景图
	CSprite *		m_pLevel;		//Level界面组件
	CSprite *		m_pLevelNums;	//关卡数字
	CSprite	*		m_pFailed;		//Failed界面组件
	CSprite *		m_pPreNums;		//破坏百分比数字
	CSprite *		m_pMinPre;		//最小百分比组件
	CRoundButton *	m_pReplay;		//重玩
	CRoundButton *	m_pSelectLevel;	//选择关卡
	INT				m_iCurLevel;	//要进入的关卡数
	INT				m_iPrecent;		//破坏进度
	INT				m_iTurnInCount;	//进入动画计数
};

