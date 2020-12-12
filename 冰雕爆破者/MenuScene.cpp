#include "stdafx.h"
#include "MenuScene.h"


CMenuScene::CMenuScene()
{
}


CMenuScene::~CMenuScene()
{
	delete m_pBackground;
	delete m_pContinue;
	delete m_pSelect;
	delete m_pReplay;
}

void CMenuScene::Init()
{
	SIZE * winSize = (SIZE *)CPlatform::GetInstance()->GetPlatData((_T("winsize")));

	//��ʼ������
	m_pBackground = new CSprite;
	m_pBackground->Init(_T("bg_light"));
	m_pBackground->SetDesPos({ 0,0 });
	m_pBackground->AjustScale(*winSize);

	m_pContinue = new CRoundButton(_T("continue_N"), _T("continue_I"), _T("continue_D"));
	m_pContinue->SetDesPos({ 240,250 });
	m_pContinue->SetRound(60);
	m_pContinue->Refresh();
	m_pContinue->SetDownSound(_T("buttomDown"));
	m_pContinue->SetCallBack(bind(&CMenuScene::ContinueGame, this, placeholders::_1));

	m_pSelect = new CRoundButton(_T("select_N"), _T("select_I"), _T("select_D"));
	m_pSelect->SetDesPos({ 110,475 });
	m_pSelect->SetRound(40);
	m_pSelect->Refresh();
	m_pSelect->SetDownSound(_T("buttomDown"));
	m_pSelect->SetCallBack(bind(&CMenuScene::ToSelect, this, placeholders::_1));

	m_pReplay = new CRoundButton(_T("replay_N"), _T("replay_I"), _T("replay_D"));
	m_pReplay->SetDesPos({ 370,475 });
	m_pReplay->SetRound(40);
	m_pReplay->Refresh();
	m_pReplay->SetDownSound(_T("buttomDown"));
	m_pReplay->SetCallBack(bind(&CMenuScene::ReplayGame, this, placeholders::_1));


}

void CMenuScene::Render()
{
	m_pBackground->Draw();
	m_pContinue->Render();
	m_pSelect->Render();
	m_pReplay->Render();
}

void CMenuScene::Update()
{
	if (!m_nextScene.empty() && m_pContinue->AnimIsEnd() && m_pSelect->AnimIsEnd() && m_pReplay->AnimIsEnd())
	{
		CGameManager::GetInstance()->setNowScene(m_nextScene);
		m_nextScene.clear();
		return;
	}
}

void CMenuScene::TurnIn()
{
	CInputManager::GetInstance()->AddMouseListener(_T("������ť"), m_pContinue);
	CInputManager::GetInstance()->AddMouseListener(_T("ѡ��ť"), m_pSelect);
	CInputManager::GetInstance()->AddMouseListener(_T("���水ť"), m_pReplay);
}

void CMenuScene::TurnOut()
{
	CInputManager::GetInstance()->DeleteMouseListener(_T("������ť"));
	CInputManager::GetInstance()->DeleteMouseListener(_T("ѡ��ť"));
	CInputManager::GetInstance()->DeleteMouseListener(_T("���水ť"));
}

void CMenuScene::ToSelect(int num)
{
	TCHAR paramStr[128];
	_stprintf(paramStr, _T("%d,%d,%s"), 60, 0, _T("ѡ�ؽ���"));
	CGameManager::GetInstance()->setParam(paramStr);
	CLevelManager::GetInstance()->UnloadeNowLevel();	//ж�عؿ�
	CInputManager::GetInstance()->DeleteMouseListener(_T("��ǰը��"));
	m_nextScene = _T("�ȴ�����");
}

void CMenuScene::ContinueGame(int num)
{
	m_nextScene = _T("��Ϸ����");
}

void CMenuScene::ReplayGame(int num)
{
	CLevelManager::GetInstance()->UnloadeNowLevel();
	m_nextScene = _T("�ȴ�����");
	CInputManager::GetInstance()->DeleteMouseListener(_T("��ǰը��"));
}
