#include "stdafx.h"
#include "BombShooter.h"
#include "NormalBomb.h"
#include "LevelManager.h"
#include "PlayScene.h"


CPlayScene::CPlayScene()
{
}


CPlayScene::~CPlayScene()
{
	delete m_pShooter;
	delete m_pExchange;
	delete m_pMenu;
}

void CPlayScene::Init()
{
	m_pShooter = new CBombShooter;
	m_pShooter->init();


	m_pExchange = new Button(_T("exchange_N"), _T("exchange_D"), _T("exchange_D"));
	m_pExchange->SetDesPos({ 25,699 });
	m_pExchange->setCollisionArr(m_pExchange->GetRect(40, 40));
	m_pExchange->SetDownSound(_T("simpleClick"));
	m_pExchange->setCallBack(bind(&CPlayScene::Exchange, this, placeholders::_1));

	m_pMenu = new Button(_T("menu_N"), _T("menu_D"), _T("menu_D"));
	m_pMenu->SetDesPos({ 419,701 });
	m_pMenu->setCollisionArr(m_pMenu->GetRect(110, 30));
	m_pMenu->SetDownSound(_T("simpleClick"));
	m_pMenu->setCallBack(bind(&CPlayScene::ToMenu, this, placeholders::_1));
	m_pLevelManager = CLevelManager::GetInstance();
}

void CPlayScene::Render()
{
	m_pLevelManager->Render();
	m_pShooter->Draw();
	m_pExchange->Render();
	m_pMenu->Render();
}

void CPlayScene::Update()
{
	if (!m_nextScene.empty())
	{
		CGameManager::GetInstance()->setNowScene(m_nextScene);
		m_nextScene.clear();
		return;
	}
	m_pLevelManager->Update();
	m_pShooter->Update();
	if (!m_pShooter->GetNowBomb() && m_pShooter->GetDestoryPre() < 90.0 && m_pShooter->GetState() == CBombShooter::NORMAL)
	{
		m_iStateCount++;
		if (m_iStateCount >= 60)
		{
			m_nextScene = _T("失败界面");
			TCHAR paramStr[128];
			_stprintf(paramStr, _T("%d,%d"), m_pLevelManager->GetNowLevel(), (int)m_pShooter->GetDestoryPre());
			CGameManager::GetInstance()->setParam(paramStr);
			CAudio4Bass::GetInstance()->ChannelPlay(_T("failed"),true);
		}
	}
	else if (!m_pShooter->GetNowBomb() && m_pShooter->GetDestoryPre() >= 90.0 && m_pShooter->GetState() == CBombShooter::NORMAL)
	{
		int score;
		float pre = m_pShooter->GetDestoryPre();
		if (pre >= 100)
			score = 3;
		else if (pre >= 95)
			score = 2;
		else
			score = 1;
		m_iStateCount++;
		if (m_iStateCount >= 80)
		{
			m_nextScene = _T("胜利界面");
			TCHAR paramStr[128];
			_stprintf(paramStr, _T("%d,%d"), m_pLevelManager->GetNowLevel(), score);
			CLevelManager::GetInstance()->UnlockNextLevel();
			CLevelManager::GetInstance()->SetNowLevelScore(score);
			CGameManager::GetInstance()->setParam(paramStr);
			CAudio4Bass::GetInstance()->ChannelPlay(_T("win"), true);
		}
	}
	else if (m_pShooter->GetDestoryPre() >= 100.0)
	{
		m_iStateCount++;
		if (m_iStateCount >= 80)
		{
			m_nextScene = _T("胜利界面");
			TCHAR paramStr[128];
			_stprintf(paramStr, _T("%d,%d"), m_pLevelManager->GetNowLevel(), 3);
			CLevelManager::GetInstance()->UnlockNextLevel();
			CLevelManager::GetInstance()->SetNowLevelScore(3);
			CGameManager::GetInstance()->setParam(paramStr);
			CAudio4Bass::GetInstance()->ChannelPlay(_T("win"), true);
		}
	}
	else
		m_iStateCount = 0;

}

void CPlayScene::TurnIn()
{
	m_pShooter->SetBout(1);
	CInputManager::GetInstance()->AddMouseListener(_T("发射者"), m_pShooter);
	CInputManager::GetInstance()->AddMouseListener(_T("交换按钮"), m_pExchange);
	CInputManager::GetInstance()->AddMouseListener(_T("菜单按钮"), m_pMenu);
}

void CPlayScene::TurnOut()
{
	CInputManager::GetInstance()->DeleteMouseListener(_T("发射者"));
	CInputManager::GetInstance()->DeleteMouseListener(_T("交换按钮"));
	CInputManager::GetInstance()->DeleteMouseListener(_T("菜单按钮"));
}

void CPlayScene::Exchange(int num)
{
	m_pShooter->ExchangeBomb();
}

void CPlayScene::ToMenu(int num)
{
	m_nextScene = _T("菜单界面");
}
