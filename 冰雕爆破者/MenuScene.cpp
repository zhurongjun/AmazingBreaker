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

	//初始化精灵
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
	CInputManager::GetInstance()->AddMouseListener(_T("继续按钮"), m_pContinue);
	CInputManager::GetInstance()->AddMouseListener(_T("选择按钮"), m_pSelect);
	CInputManager::GetInstance()->AddMouseListener(_T("重玩按钮"), m_pReplay);
}

void CMenuScene::TurnOut()
{
	CInputManager::GetInstance()->DeleteMouseListener(_T("继续按钮"));
	CInputManager::GetInstance()->DeleteMouseListener(_T("选择按钮"));
	CInputManager::GetInstance()->DeleteMouseListener(_T("重玩按钮"));
}

void CMenuScene::ToSelect(int num)
{
	TCHAR paramStr[128];
	_stprintf(paramStr, _T("%d,%d,%s"), 60, 0, _T("选关界面"));
	CGameManager::GetInstance()->setParam(paramStr);
	CLevelManager::GetInstance()->UnloadeNowLevel();	//卸载关卡
	CInputManager::GetInstance()->DeleteMouseListener(_T("当前炸弹"));
	m_nextScene = _T("等待界面");
}

void CMenuScene::ContinueGame(int num)
{
	m_nextScene = _T("游戏界面");
}

void CMenuScene::ReplayGame(int num)
{
	CLevelManager::GetInstance()->UnloadeNowLevel();
	m_nextScene = _T("等待界面");
	CInputManager::GetInstance()->DeleteMouseListener(_T("当前炸弹"));
}
