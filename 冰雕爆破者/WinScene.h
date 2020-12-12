#pragma once

class CWinScene :
	public CGameScene
{
public:
	CWinScene();
	~CWinScene();

	void Init();	//初始化
	void Render();	//绘制
	void Update();	//更新
	void TurnIn();	//进入的准备工作
	void TurnOut();	//进入的准备工作

protected:
	void ToNextLevel(int num);
	void ReplayLevel(int num);
	void ToSelect(int num);
private:
	CSprite *		m_pBackground;	//背景图
	CSprite *		m_pLevel;		//Level界面组件
	CSprite *		m_pLevelNums;	//关卡数字
	CSprite	*		m_pComplete;	//Complete界面组件
	CSprite *		m_pStar;		//星星组件
	CSprite *		m_pStarBk;		//星星背景组件
	CRoundButton *	m_pReplay;		//重玩
	CRoundButton *	m_pSelectLevel;	//选择关卡
	CRoundButton *	m_pNextLevel;	//下一关按钮
	INT				m_iCurLevel;	//要进入的关卡数
	INT				m_iScore;		//得分
	INT				m_iTurnInCount;	//进入动画计数
};

