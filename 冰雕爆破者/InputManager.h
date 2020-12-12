#pragma once
class CInputManager
	:public CSingleton<CInputManager>
{
	friend class CSingleton<CInputManager>;
	CInputManager();
public:
	~CInputManager();
	void AddKeyListener(TSTRING, CKeyListener*);	//添加键盘监听事件
	void AddMouseListener(TSTRING, CMouseListener*);	//添加鼠标监听事件
	void DeleteKeyListener(TSTRING);		//删除键盘监听事件
	void DeleteMouseListener(TSTRING);	//删除鼠标监听事件
	void WinProc(UINT, WPARAM, LPARAM);	//消息处理
	CKeyListener*GetKeyListener(TSTRING);
	CMouseListener*GetMouseListener(TSTRING);
private:
	map<TSTRING, CKeyListener*>m_keyListener;
	map<TSTRING, CMouseListener*>m_mouseListener;
	map<TSTRING, CKeyListener*>::iterator m_keyIter;
	map<TSTRING, CMouseListener*>::iterator m_mouseIter;
};

