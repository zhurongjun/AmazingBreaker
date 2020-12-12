#pragma once
#include "GameScene.h"
class CLoginScene :
	public CGameScene
{
public:
	CLoginScene();
	~CLoginScene();

	void Init();	//初始化
	void Render();	//绘制
	void Update();	//更新
	void TurnIn();	//进入的准备工作
	void TurnOut();	//退出的后续工作

protected:
private:
	CSprite *	m_pBackground;	//背景图
	CSprite *	m_pLogo;		//Logo
	CSprite *	m_pStar;		//浮游星星
	CRoundButton	*	m_pPlayButtom;	//进入游戏按钮
	struct star{
		int		life;	//生命周期
		float	size;	//大小(缩放值)
		D3DXVECTOR2	dir;	//方向
		D3DXVECTOR2 pos;	//位置
		void upDate();
	}m_stars[200];

	void toSelectMenu(int num);	//转向设置界面
};

