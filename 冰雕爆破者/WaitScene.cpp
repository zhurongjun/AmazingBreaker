#include "stdafx.h"
#include "WaitScene.h"


CWaitScene::CWaitScene()
{
	m_curTick = 0;
}


CWaitScene::~CWaitScene()
{
	delete m_pBackground;
	delete m_pLoading;
	delete m_pLevel;
	delete m_pLevelNum;
}

void CWaitScene::Init()
{
	SIZE * winSize = (SIZE *)CPlatform::GetInstance()->GetPlatData((_T("winsize")));

	//初始化精灵
	m_pBackground = new CSprite;
	m_pBackground->Init(_T("bg_light"));
	m_pBackground->SetDesPos({ 0,0 });
	m_pBackground->AjustScale(*winSize);

	m_pLoading = new CSprite;
	m_pLoading->Init(_T("loading"));
	m_pLoading->SetDesPos({ 250,330});
	m_pLoading->SetCenter(CSprite::ECEN_MIDDLE | CSprite::ECEN_CENTER);
	m_pLoading->SetXScale(0.6);
	m_pLoading->SetYScale(0.6);

	vector<TSTRING> names;
	m_pLevelNum = new CSprite;
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
	m_pLevelNum->Init(names);
	m_pLevelNum->SetXScale(0.7);
	m_pLevelNum->SetYScale(0.7);
	m_pLevelNum->SetCenter(CSprite::ECEN_CENTER | CSprite::ECEN_MIDDLE);
	m_pLevelNum->SetDesPos({ (float)winSize->cx / 2 + 55,(float)winSize->cy / 2 - 58 });

	m_pLevel = new CSprite;
	m_pLevel->Init(_T("level"));
	m_pLevel->SetCenter(CSprite::ECEN_CENTER | CSprite::ECEN_MIDDLE);
	m_pLevel->SetXScale(0.8);
	m_pLevel->SetYScale(0.8);
	m_pLevel->SetDesPos({ (float)winSize->cx / 2 - 40,(float)winSize->cy / 2 - 60 });
}

void CWaitScene::Render()
{
	if (m_curTick >= m_maxTick)
	{
		if (m_curTick < m_maxTick + 20 && m_nextLevel)
		{
			CGameScene * scene = CGameManager::GetInstance()->getScene(_T("游戏界面"));
			scene->Render();
			scene->Update();
			D3DXVECTOR2 pos = { 0,-(float)(m_curTick - m_maxTick) * 24 };

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
	}
	else
	{
		m_pBackground->Draw();
		m_pLoading->Draw();
		if (m_nextLevel)
		{
			m_pLevel->Draw();
			int num = m_nextLevel;
			int count = 0;
			TCHAR numStr[64];
			D3DXVECTOR2 pos = m_pLevelNum->GetDesPos();
			while (num)
			{
				count++;
				num /= 10;

			}
			pos.x += (count - 1) * 35;
			num = m_nextLevel;
			for (int i = 0; i < count; i++)
			{
				_stprintf(numStr, _T("Wait_%d"), num % 10);
				m_pLevelNum->SetCurAnimation(numStr);
				m_pLevelNum->SetDesPos(pos);
				m_pLevelNum->Draw();
				num /= 10;
				pos.x -= 35;
			}

		}
	}
}

void CWaitScene::Update()
{
	m_curTick++;
	if (m_curTick >= m_maxTick)
	{
		if (m_nextLevel)
		{
			if (m_curTick >= m_maxTick + 20)
			{
				CGameManager::GetInstance()->setNowScene(m_nextScene);
				m_nextScene.clear();
				m_pBackground->ResetSrcRect();
				m_pBackground->SetDesPos({ 0, 0 });
				CAudio4Bass::GetInstance()->ChannelPlay(_T("turnIn"), true);
				return;
			}
		}
		else
		{
			CGameManager::GetInstance()->setNowScene(m_nextScene);
			m_nextScene.clear();
			return;
		}
	}
}

void CWaitScene::TurnIn()
{
	TCHAR	nextScene[256];
	int level;
	TSTRING paramWord = CGameManager::GetInstance()->getParam();
	_stscanf(paramWord.c_str(), _T("%d,%d,%s"), &m_maxTick, &level, nextScene);
	if (level)
	{
		m_pLoading->SetDesPos({ 240,370 });
		CLevelManager::GetInstance()->LoadLevel(level);
	}
	else
	{
		m_pLoading->SetDesPos({ 240,330 });
	}
	m_nextLevel = level;
	m_nextScene = nextScene;
}

void CWaitScene::TurnOut()
{
	m_curTick = 0;
}
