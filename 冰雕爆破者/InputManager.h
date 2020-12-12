#pragma once
class CInputManager
	:public CSingleton<CInputManager>
{
	friend class CSingleton<CInputManager>;
	CInputManager();
public:
	~CInputManager();
	void AddKeyListener(TSTRING, CKeyListener*);	//��Ӽ��̼����¼�
	void AddMouseListener(TSTRING, CMouseListener*);	//����������¼�
	void DeleteKeyListener(TSTRING);		//ɾ�����̼����¼�
	void DeleteMouseListener(TSTRING);	//ɾ���������¼�
	void WinProc(UINT, WPARAM, LPARAM);	//��Ϣ����
	CKeyListener*GetKeyListener(TSTRING);
	CMouseListener*GetMouseListener(TSTRING);
private:
	map<TSTRING, CKeyListener*>m_keyListener;
	map<TSTRING, CMouseListener*>m_mouseListener;
	map<TSTRING, CKeyListener*>::iterator m_keyIter;
	map<TSTRING, CMouseListener*>::iterator m_mouseIter;
};

