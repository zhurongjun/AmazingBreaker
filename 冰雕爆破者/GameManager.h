#pragma once

class CGameScene;

class CGameManager
	:public CSingleton<CGameManager>
{
	friend class CSingleton<CGameManager>;
	CGameManager();
public:
	~CGameManager();
	void Init(HWND&);	//��ʼ��
	void Render();		//����
	void Update();		//����
	void WinProc(UINT msg, WPARAM wParam, LPARAM lParam);	//��Ϣ����

	CGameScene * getScene(TSTRING name);	//��ȡĳ������
	void setNowScene(TSTRING name);	//���õ�ǰ�Ľ���
	void addScene(TSTRING name, CGameScene * scene);	//��ӽ���
	void setParam(TSTRING param) { m_paramWord = param; }	//���ò���
	TSTRING getParam() { return m_paramWord; }	//��ȡ����

protected:
	map<TSTRING, CGameScene *> m_mapScene;	//�洢���������
	TSTRING		m_curSecne;	//��ǰ����
	TSTRING		m_paramWord;	//�л�����Ĳ����ַ���
private:	
};

