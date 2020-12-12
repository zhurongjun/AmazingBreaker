#pragma once
class CGameScene
{
public:
	CGameScene();
	virtual ~CGameScene();

	virtual void Init() = 0;	//初始化
	virtual void Render() = 0;	//绘制
	virtual void Update() = 0;	//更新
	virtual void TurnIn() = 0;	//进入的准备工作
	virtual void TurnOut() = 0;	//进入的准备工作

protected:
	TSTRING m_nextScene;	//下一个界面
};

