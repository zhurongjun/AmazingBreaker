#include "stdafx.h"
#include "InputManager.h"

CInputManager::CInputManager()
{
}

CInputManager::~CInputManager()
{
}

void CInputManager::AddKeyListener(TSTRING name, CKeyListener*key)
{
	m_keyListener[name] = key;
	m_keyIter = m_keyListener.begin();
}

void CInputManager::AddMouseListener(TSTRING name, CMouseListener*mouse)
{
	m_mouseListener[name] = mouse;
	m_mouseIter = m_mouseListener.begin();
}

void CInputManager::DeleteKeyListener(TSTRING name)
{
	map<TSTRING, CKeyListener*>::iterator it = m_keyListener.find(name);
	if (it != m_keyListener.end())
		m_keyIter = m_keyListener.erase(it);
}

void CInputManager::DeleteMouseListener(TSTRING name)
{
	map<TSTRING, CMouseListener*>::iterator it = m_mouseListener.find(name);
	if (it != m_mouseListener.end())
		m_mouseIter = m_mouseListener.erase(it);
}

void CInputManager::WinProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	SEvent event;
	ZeroMemory(&event, sizeof(SEvent));
	switch (msg)
	{
	
	case WM_KEYDOWN:
	case WM_KEYUP:                                   //�����¼�
		event.eventType = eEVENT_KEY;
		event.keyInput.key = wParam;	//���¼���ֵ
		event.keyInput.pressedDown = (msg == WM_KEYDOWN);	//�Ƿ���
		m_keyIter = m_keyListener.begin();
		for (; m_keyIter != m_keyListener.end(); m_keyIter++)
		{
			if (event.keyInput.pressedDown)                  //������̰��£���ִ�а����¼�������ִ��̧���¼�
				m_keyIter->second->OnKeyDown(event);
			else
				m_keyIter->second->OnKeyUp(event);
		}
		break;
	case WM_LBUTTONDOWN:                                 //��갴���¼�
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
		event.eventType = eEVENT_MOUSE;
		event.mouseInput.x = LOWORD(lParam);
		event.mouseInput.y = HIWORD(lParam);
		switch (wParam)
		{
		case MK_LBUTTON:                                   //�������
			event.mouseInput.mouseEvent = eMOUSE_PRESS_LDOWN;
			break;
		case MK_RBUTTON:                                 //�Ҽ�����
			event.mouseInput.mouseEvent = eMOUSE_PRESS_RDOWN;
			break;
		case MK_MBUTTON:                                  //���ְ���
			event.mouseInput.mouseEvent = eMOUSE_PRESS_MDOWN;
			break;
		}
		m_mouseIter = m_mouseListener.begin();
		for (; m_mouseIter != m_mouseListener.end(); m_mouseIter++)
			m_mouseIter->second->OnMouseDown(event);
		break;
	case WM_LBUTTONUP:                   //���̧���¼�
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
		event.eventType = eEVENT_MOUSE;
		event.mouseInput.x = LOWORD(lParam);
		event.mouseInput.y = HIWORD(lParam);
		switch (wParam)
		{

		case MK_RBUTTON:
			event.mouseInput.mouseEvent = eMOUSE_PRESS_RUP;     //��̧��
			break;
		case MK_MBUTTON:
			event.mouseInput.mouseEvent = eMOUSE_PRESS_MUP;          //�м�̧��
			break;
		default:
			//case MK_LBUTTON:!!!!!!!!!!!!!!!!
			event.mouseInput.mouseEvent = eMOUSE_PRESS_LUP;          //��̧��
			break;
		}
		m_mouseIter = m_mouseListener.begin();
		for (; m_mouseIter != m_mouseListener.end(); m_mouseIter++)
			m_mouseIter->second->OnMouseUp(event);
		break;
	case WM_MOUSEMOVE:                                                             //����ƶ��¼�
		event.eventType = eEVENT_MOUSE;
		event.mouseInput.mouseEvent = eMOUSE_MOVE;
		event.mouseInput.x = LOWORD(lParam);
		event.mouseInput.y = HIWORD(lParam);
		m_mouseIter = m_mouseListener.begin();
		for (; m_mouseIter != m_mouseListener.end(); m_mouseIter++)
			m_mouseIter->second->OnMouseMove(event);
		break;
	case WM_MOUSEWHEEL:
		event.eventType = eEVENT_MOUSE;
		event.mouseInput.y = event.mouseInput.x = (char)HIWORD(wParam);
		event.mouseInput.mouseEvent = eMOUSE_MOVE;
		m_mouseIter = m_mouseListener.begin();
		for (; m_mouseIter != m_mouseListener.end(); m_mouseIter++)
			m_mouseIter->second->OnMouseWheel(event);
		break;
	}
}

CKeyListener*CInputManager::GetKeyListener(TSTRING name)
{
	map<TSTRING, CKeyListener*>::iterator it = m_keyListener.find(name);
	if (it != m_keyListener.end())
		return it->second;
	else
		return nullptr;
}

CMouseListener*CInputManager::GetMouseListener(TSTRING name)
{
	map<TSTRING, CMouseListener*>::iterator it = m_mouseListener.find(name);
	if (it != m_mouseListener.end())
		it->second;
	else
		return nullptr;
}