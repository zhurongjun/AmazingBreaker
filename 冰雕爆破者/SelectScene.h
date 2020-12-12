#pragma once

class CLevelButtom;

class CSelectScene :
	public CGameScene,
	public CMouseListener
{
	enum ESTATE
	{
		NORMAL,
		TURNIN,
		NEXT,
		LAST
	};
public:
	CSelectScene();
	~CSelectScene();

	void Init();	//初始化
	void Render();	//绘制
	void Update();	//更新
	void TurnIn();	//进入的准备工作
	void TurnOut();	//退出的后续工作

	virtual void OnMouseDown(SEvent event);
	virtual void OnMouseUp(SEvent event);
	virtual void OnMouseMove(SEvent event);
	virtual void OnMouseWheel(SEvent event);

protected:
private:
	void backToMain(int num);
	void beginGame(int num);

	//选择界面信息组件
	int			m_iCurStage;	//当前界面
	ESTATE		m_eState;		//当前的状态
	int			m_iStateCount;	//状态计数

	//界面UI组件
	Button	*	m_pBackButtom;	//返回按钮
	CSprite *	m_pBackground;	//背景图
	CSprite *	m_pStage;		//StageUI
	CSprite *	m_pStageNum;	//关卡的数字
	CSprite *	m_pLine;		//线
	
	CLevelButtom *	m_pButtoms[6][20];	//所有的按钮(丧心病狂)
	
};

