#include "stdafx.h"
#include "FailScene.h"


CFailScene::CFailScene()
{
}


CFailScene::~CFailScene()
{
	delete m_pLevel;
	delete m_pLevelNums;
	delete m_pFailed;
	delete m_pPreNums;
	delete m_pReplay;
	delete m_pSelectLevel;
	delete m_pBackground;
	delete m_pMinPre;
}

void CFailScene::Init()
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
	m_pLevel->SetDesPos({ 200,120 });

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
	m_pLevelNums->SetDesPos({ 320,123 });

	m_pPreNums = new CSprite;
	names.clear();
	names.push_back(_T("Binner_YellowPre"));
	names.push_back(_T("Binner_Yellow0"));
	names.push_back(_T("Binner_Yellow1"));
	names.push_back(_T("Binner_Yellow2"));
	names.push_back(_T("Binner_Yellow3"));
	names.push_back(_T("Binner_Yellow4"));
	names.push_back(_T("Binner_Yellow5"));
	names.push_back(_T("Binner_Yellow6"));
	names.push_back(_T("Binner_Yellow7"));
	names.push_back(_T("Binner_Yellow8"));
	names.push_back(_T("Binner_Yellow9"));
	m_pPreNums->Init(names);
	m_pPreNums->SetCenter(CSprite::ECEN_CENTER | CSprite::ECEN_MIDDLE);
	m_pPreNums->SetXScale(0.7);
	m_pPreNums->SetYScale(0.7);
	m_pPreNums->SetDesPos({ 280,265 });

	m_pFailed = new CSprite;
	m_pFailed->Init(_T("levelfailed_text"));
	m_pFailed->SetCenter(CSprite::ECEN_CENTER | CSprite::ECEN_MIDDLE);
	m_pFailed->SetXScale(0.7);
	m_pFailed->SetYScale(0.7);
	m_pFailed->SetDesPos({ 240,160 });

	m_pReplay = new CRoundButton(_T("replayF_N"), _T("replayF_I"), _T("replayF_D"));
	m_pReplay->SetDesPos({ 370,545 });
	m_pReplay->SetRound(40);
	m_pReplay->Refresh();
	m_pReplay->SetDownSound(_T("buttomDown"));
	m_pReplay->SetCallBack(bind(&CFailScene::ReplayLevel, this, placeholders::_1));

	m_pSelectLevel= new CRoundButton(_T("selectF_N"), _T("selectF_I"), _T("selectF_D"));
	m_pSelectLevel->SetDesPos({ 110,545 });
	m_pSelectLevel->SetRound(40);
	m_pSelectLevel->Refresh();
	m_pSelectLevel->SetDownSound(_T("buttomDown"));
	m_pSelectLevel->SetCallBack(bind(&CFailScene::ToSelect, this, placeholders::_1));

	m_pMinPre = new CSprite;
	m_pMinPre->Init(_T("min_90%"));
	m_pMinPre->SetCenter(CSprite::ECEN_CENTER | CSprite::ECEN_MIDDLE);
	m_pMinPre->SetXScale(0.65);
	m_pMinPre->SetYScale(0.65);
	m_pMinPre->SetDesPos({ 240,320 });

	m_iCurLevel = 1;
	m_iPrecent = 50;
}

void CFailScene::Render()
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
	}
	else
	{
		m_pBackground->Draw();
		m_pFailed->Draw();
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

		//绘制百分比数字
		count = 0;
		pos = m_pPreNums->GetDesPos();
		num = m_iPrecent;
		while (num)
		{
			count++;
			num /= 10;
		}
		num = m_iPrecent;
		pos.x -= 45;
		m_pPreNums->SetCurAnimation(_T("Binner_YellowPre"));
		m_pPreNums->SetCenter(CSprite::ECEN_CENTER | CSprite::ECEN_MIDDLE);
		m_pPreNums->Draw();
		while (num)
		{
			_stprintf(numStr, _T("Binner_Yellow%d"), num % 10);
			m_pPreNums->SetCurAnimation(numStr);
			m_pPreNums->SetCenter(CSprite::ECEN_CENTER | CSprite::ECEN_MIDDLE);
			m_pPreNums->SetDesPos(pos);
			m_pPreNums->Draw();
			pos.x -= 35;
			num /= 10;
		}
		pos.x += count * 35;
		pos.x += 45;
		m_pPreNums->SetDesPos(pos);

		m_pMinPre->Draw();
		m_pLevel->Draw();
		m_pReplay->Render();
		m_pSelectLevel->Render();
	}
}

void CFailScene::Update()
{
	if (!m_nextScene.empty() && m_pSelectLevel->AnimIsEnd() && m_pReplay->AnimIsEnd())
	{
		CGameManager::GetInstance()->setNowScene(m_nextScene);
		m_nextScene.clear();
		return;
	}
}

void CFailScene::TurnIn()
{
	m_iTurnInCount = 20;
	CLevelManager::GetInstance()->UnloadeNowLevel();
	_stscanf(CGameManager::GetInstance()->getParam().c_str(), _T("%d,%d"), &m_iCurLevel, &m_iPrecent);
}

void CFailScene::TurnOut()
{
	CInputManager::GetInstance()->DeleteMouseListener(_T("选择按钮"));
	CInputManager::GetInstance()->DeleteMouseListener(_T("重玩按钮"));
}

void CFailScene::ReplayLevel(int num)
{
	m_nextScene = _T("等待界面");
	TCHAR paramWord[256];
	_stprintf(paramWord, _T("%d,%d,%s"), 60, m_iCurLevel, _T("游戏界面"));
	CGameManager::GetInstance()->setParam(paramWord);
}

void CFailScene::ToSelect(int num)
{
	TCHAR paramStr[128];
	_stprintf(paramStr, _T("%d,%d,%s"), 60, 0, _T("选关界面"));
	CGameManager::GetInstance()->setParam(paramStr);
	m_nextScene = _T("等待界面");
}
