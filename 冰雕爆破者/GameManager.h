#pragma once

class CGameScene;

class CGameManager
	:public CSingleton<CGameManager>
{
	friend class CSingleton<CGameManager>;
	CGameManager();
public:
	~CGameManager();
	void Init(HWND&);	//初始化
	void Render();		//绘制
	void Update();		//更新
	void WinProc(UINT msg, WPARAM wParam, LPARAM lParam);	//消息处理

	CGameScene * getScene(TSTRING name);	//获取某个界面
	void setNowScene(TSTRING name);	//设置当前的界面
	void addScene(TSTRING name, CGameScene * scene);	//添加界面
	void setParam(TSTRING param) { m_paramWord = param; }	//设置参数
	TSTRING getParam() { return m_paramWord; }	//获取参数

protected:
	map<TSTRING, CGameScene *> m_mapScene;	//存储界面的容器
	TSTRING		m_curSecne;	//当前界面
	TSTRING		m_paramWord;	//切换界面的参数字符串
private:	
};

