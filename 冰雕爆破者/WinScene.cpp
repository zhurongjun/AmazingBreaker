#include "stdafx.h"
#include "WinScene.h"


CWinScene::CWinScene()
{
	m_iTurnInCount = 0;
}


CWinScene::~CWinScene()
{
	delete m_pBackground;
	delete m_pLevel;
	delete m_pLevelNums;
	delete m_pComplete;
	delete m_pStar;
	delete m_pStarBk;
	delete m_pReplay;
	delete m_pSelectLevel;
	delete m_pNextLevel;
}

void CWinScene::Init()
{
	SIZE * winSize = (SIZE *)CPlatform::GetInstance()->GetPlatData((_T("winsize")));

	//初始化精灵
	m_pBackground = new CSprite;
	m_pBackground->Init(_T("bg_light"));
	m_pBackground->SetDesPos({ 0,0 });
	m_pBackground->AjustScale(*winSize);

	vector<TSTRING> names;
	m_pLevel = new CSprite;
	m_pLevel->Init(_T("level"));
	m_pLevel->SetCenter(CSprite::ECEN_CENTER | CSprite::ECEN_MIDDLE);
	m_pLevel->SetXScale(0.8);
	m_pLevel->SetYScale(0.8);
	m_pLevel->SetDesPos({ 200,90 });

	m_pLevelNums = new CSprite;
	names.clear();
	names.push_back(_T("Wait_0"));
	names.push_back(_T("Wait_1"));
	names.push_back(_T("Wait_2"));
	names.push_back(_T("Wait_3"));
	names.push_back(_T("Wait_4"));
	names.push_back(_T("Wait_5"));
	names.push_back(_T("Wait_6"));
	names.push_back(_T("Wait_7"));
	names.push_back(_T("Wait_8"));
	names.push_back(_T("Wait_9"));
	m_pLevelNums->Init(names);
	m_pLevelNums->SetCenter(CSprite::ECEN_CENTER | CSprite::ECEN_MIDDLE);
	m_pLevelNums->SetXScale(0.7);
	m_pLevelNums->SetYScale(0.7);
	m_pLevelNums->SetDesPos({ 320,93 });

	m_pReplay = new CRoundButton(_T("replayF_N"), _T("replayF_I"), _T("replayF_D"));
	m_pReplay->SetDesPos({ 370,545 });
	m_pReplay->SetRound(40);
	m_pReplay->Refresh();
	m_pReplay->SetDownSound(_T("buttomDown"));
	m_pReplay->SetCallBack(bind(&CWinScene::ReplayLevel, this, placeholders::_1));

	m_pSelectLevel = new CRoundButton(_T("selectF_N"), _T("selectF_I"), _T("selectF_D"));
	m_pSelectLevel->SetDesPos({ 110,545 });
	m_pSelectLevel->SetRound(40);
	m_pSelectLevel->Refresh();
	m_pSelectLevel->SetDownSound(_T("buttomDown"));
	m_pSelectLevel->SetCallBack(bind(&CWinScene::ToSelect, this, placeholders::_1));

	m_pNextLevel = new CRoundButton(_T("next_N"), _T("next_I"), _T("next_D"));
	m_pNextLevel->SetDesPos({ 240,400 });
	m_pNextLevel->SetRound(60);
	m_pNextLevel->Refresh();
	m_pNextLevel->SetDownSound(_T("buttomDown"));
	m_pNextLevel->SetCallBack(bind(&CWinScene::ToNextLevel, this, placeholders::_1));

	m_pComplete = new CSprite;
	m_pComplete->Init(_T("levelcomplete_text"));
	m_pComplete->SetCenter(CSprite::ECEN_CENTER | CSprite::ECEN_MIDDLE);
	m_pComplete->SetXScale(0.7);
	m_pComplete->SetYScale(0.7);
	m_pComplete->SetDesPos({ 240,130 });

	m_pStar = new CSprite;
	m_pStar->Init(_T("slstar"));
	m_pStar->SetCenter(CSprite::ECEN_CENTER | CSprite::ECEN_MIDDLE);
	m_pStar->SetXScale(0.6);
	m_pStar->SetYScale(0.6);
	m_pStar->SetDesPos({ 240,230 });

	m_pStarBk = new CSprite;
	m_pStarBk->Init(_T("star_empty"));
	m_pStarBk->SetCenter(CSprite::ECEN_CENTER | CSprite::ECEN_MIDDLE);
	m_pStarBk->SetXScale(0.6);
	m_pStarBk->SetYScale(0.6);
	m_pStarBk->SetDesPos({ 240,230 });

	m_iCurLevel = 50;
	m_iScore = 3;
}

void CWinScene::Render()
{
	if (m_iTurnInCount > 0)
	{
		m_iTurnInCount--;
		CGameScene * scene = CGameManager::GetInstance()->getScene(_T("游戏界面"));
		scene->Render();
		D3DXVECTOR2 pos = { 0,-(float)m_iTurnInCount * 20 };

		RECT rc = { 0,0,m_pBackground->GetShowWidth(),m_pBackground->GetShowHeight() / 2 };
		m_pBackground->SetSrcRect(rc);
		m_pBackground->SetDesPos(pos);
		m_pBackground->Draw();


		rc = { 0,rc.bottom,rc.right,rc.bottom * 2 };
		pos = { 0,360 - pos.y };
		m_pBackground->SetSrcRect(rc);
		m_pBackground->SetDesPos(pos);
		m_pBackground->Draw();
	}
	else if (m_iTurnInCount == 0)
	{
		CGameScene * scene = CGameManager::GetInstance()->getScene(_T("游戏界面"));
		scene->Render();
		D3DXVECTOR2 pos = { 0,-(float)m_iTurnInCount * 20 };

		RECT rc = { 0,0,m_pBackground->GetShowWidth(),m_pBackground->GetShowHeight() / 2 };
		m_pBackground->SetSrcRect(rc);
		m_pBackground->SetDesPos(pos);
		m_pBackground->Draw();


		rc = { 0,rc.bottom,rc.right,rc.bottom * 2 };
		pos = { 0,360 - pos.y };
		m_pBackground->SetSrcRect(rc);
		m_pBackground->SetDesPos(pos);
		m_pBackground->Draw();

		m_pBackground->ResetSrcRect();
		m_pBackground->SetDesPos({ 0, 0 });
		m_iTurnInCount--;
		CInputManager::GetInstance()->AddMouseListener(_T("选择按钮"), m_pSelectLevel);
		CInputManager::GetInstance()->AddMouseListener(_T("重玩按钮"), m_pReplay);
		CInputManager::GetInstance()->AddMouseListener(_T("下一关按钮"), m_pNextLevel);
	}
	else
	{
		m_pBackground->Draw();
		m_pComplete->Draw();
		m_pLevel->Draw();
		//绘制关卡数字
		D3DXVECTOR2 pos = m_pLevelNums->GetDesPos();
		int count = 0;	//计数
		int num = m_iCurLevel;
		TCHAR numStr[128];
		while (num)
		{
			count++;
			num /= 10;
		}
		num = m_iCurLevel;
		pos.x += (count - 1) * 15;
		while (num)
		{
			_stprintf(numStr, _T("Wait_%d"), num % 10);
			m_pLevelNums->SetCurAnimation(numStr);
			m_pLevelNums->SetDesPos(pos);
			m_pLevelNums->Draw();
			pos.x -= 30;
			num /= 10;
		}
		pos.x += (count - 1) * 15 + 30;
		m_pLevelNums->SetDesPos(pos);

		//绘制星星
		pos = m_pStar->GetDesPos();
		pos.x -= 100;
		for (int i = 0; i < 3; i++)
		{
			m_pStarBk->SetDesPos(pos);
			m_pStarBk->Draw();
			m_pStar->SetDesPos(pos);
			if (i < m_iScore)
				m_pStar->Draw();
			pos.x += 100;
		}
		pos.x -= 100 * 2;
		m_pStarBk->SetDesPos(pos);
		m_pStar->SetDesPos(pos);

		m_pNextLevel->Render();
		m_pReplay->Render();
		m_pSelectLevel->Render();
	}
}

void CWinScene::TurnIn()
{
	CLevelManager::GetInstance()->SaveLevelsInfo();
	m_iTurnInCount = 20;
	CLevelManager::GetInstance()->UnloadeNowLevel();
	_stscanf(CGameManager::GetInstance()->getParam().c_str(), _T("%d,%d"), &m_iCurLevel, &m_iScore);
}

void CWinScene::TurnOut()
{
	CInputManager::GetInstance()->DeleteMouseListener(_T("选择按钮"));
	CInputManager::GetInstance()->DeleteMouseListener(_T("重玩按钮"));
	CInputManager::GetInstance()->DeleteMouseListener(_T("下一关按钮"));
}

void CWinScene::Update()
{
	if (m_iTurnInCount >= 0) return;
	if (!m_nextScene.empty() && m_pSelectLevel->AnimIsEnd() && m_pReplay->AnimIsEnd() && m_pNextLevel->AnimIsEnd())
	{
		CGameManager::GetInstance()->setNowScene(m_nextScene);
		m_nextScene.clear();
		return;
	}
}

void CWinScene::ToNextLevel(int num)
{
	m_nextScene = _T("等待界面");
	TCHAR paramWord[256];
	_stprintf(paramWord, _T("%d,%d,%s"), 60, m_iCurLevel + 1, _T("游戏界面"));
	CGameManager::GetInstance()->setParam(paramWord);
}

void CWinScene::ReplayLevel(int num)
{
	m_nextScene = _T("等待界面");
	TCHAR paramWord[256];
	_stprintf(paramWord, _T("%d,%d,%s"), 60, m_iCurLevel, _T("游戏界面"));
	CGameManager::GetInstance()->setParam(paramWord);
}

void CWinScene::ToSelect(int num)
{
	TCHAR paramStr[128];
	_stprintf(paramStr, _T("%d,%d,%s"), 60, 0, _T("选关界面"));
	CGameManager::GetInstance()->setParam(paramStr);
	m_nextScene = _T("等待界面");
}
