#include "stdafx.h"
#include "LoginScene.h"


CLoginScene::CLoginScene()
{
}


CLoginScene::~CLoginScene()
{
	delete m_pBackground;
	delete m_pLogo;
	delete m_pStar;
	delete m_pPlayButtom;
}

void CLoginScene::Init()
{
	SIZE * winSize = (SIZE *)CPlatform::GetInstance()->GetPlatData((_T("winsize")));
	
	//初始化精灵
	m_pBackground = new CSprite;
	m_pBackground->Init(_T("bg_light"));
	m_pBackground->SetDesPos({ 0,0 });
	m_pBackground->AjustScale(*winSize);

	m_pLogo = new CSprite;
	m_pLogo->Init(_T("ab_free"));
	m_pLogo->SetCenter(CSprite::ECEN_MIDDLE | CSprite::ECEN_CENTER);
	m_pLogo->SetDesPos({ (float)winSize->cx / 2,140 });
	m_pLogo->SetXScale(0.74);
	m_pLogo->SetYScale(0.74);

	vector<TSTRING> names;
	m_pPlayButtom = new CRoundButton(_T("play_N"), _T("play_I"), _T("play_D"));
	m_pPlayButtom->SetDesPos({ (float)winSize->cx / 2,400 });
	m_pPlayButtom->SetRound(75);
	m_pPlayButtom->SetCallBack(bind(&CLoginScene::toSelectMenu, this, std::placeholders::_1));
	m_pPlayButtom->Refresh();
	m_pPlayButtom->SetDownSound(_T("buttomDown"));

	m_pStar = new CSprite;
	m_pStar->Init(_T("small_Star"));
	m_pStar->SetCenter(CSprite::ECEN_MIDDLE | CSprite::ECEN_CENTER);

}

void CLoginScene::Render()
{
	m_pBackground->Draw();
	for (int i = 0; i < 200; i++)
	{
		m_pStar->SetXScale(m_stars[i].size);
		m_pStar->SetYScale(m_stars[i].size);
		m_pStar->SetDesPos(m_stars[i].pos);
		m_pStar->Draw();
		m_pStar->SetTransparent(int(m_stars[i].life * 0.6));
	}

	m_pLogo->Draw();
	m_pPlayButtom->Render();
}

void CLoginScene::Update()
{
	SIZE * winSize = (SIZE *)CPlatform::GetInstance()->GetPlatData((_T("winsize")));
	if (!m_nextScene.empty() && m_pPlayButtom->AnimIsEnd())
	{
		CGameManager::GetInstance()->setNowScene(m_nextScene);
		m_nextScene.clear();
		return;
	}
	for (int i = 0; i < 200; i++)
	{
		m_stars[i].upDate();
		if (m_stars[i].life <= 0)
		{
			m_stars[i].life = rand() % 500 + 200;
			m_stars[i].pos.x = rand() % winSize->cx;
			m_stars[i].pos.y = rand() % winSize->cy;
			m_stars[i].size = (rand() % 40 + 10) / 100.0;
			m_stars[i].dir.x = rand() % 200 - 100;
			m_stars[i].dir.y = rand() % 200 - 100;
			D3DXVec2Normalize(&m_stars[i].dir, &m_stars[i].dir);
		}
	}
}

void CLoginScene::TurnIn()
{
	SIZE * winSize = (SIZE *)CPlatform::GetInstance()->GetPlatData((_T("winsize")));

	//加入监听
	CInputManager::GetInstance()->AddMouseListener(_T("进入按钮"), m_pPlayButtom);

	//初始化所有的星点
	for (int i = 0; i < 200; i++)
	{
		m_stars[i].life = rand() % 500 + 200;
		m_stars[i].pos.x = rand() % winSize->cx;
		m_stars[i].pos.y = rand() % winSize->cy;
		m_stars[i].size = (rand() % 40 + 10) / 100.0;
		m_stars[i].dir.x = rand() % 200 - 100;
		m_stars[i].dir.y = rand() % 200 - 100;
		D3DXVec2Normalize(&m_stars[i].dir, &m_stars[i].dir);
	}
}

void CLoginScene::TurnOut()
{
	CInputManager::GetInstance()->DeleteMouseListener(_T("进入按钮"));
}

void CLoginScene::toSelectMenu(int num)
{
	m_nextScene = _T("选关界面");
}

void CLoginScene::star::upDate()
{
	this->pos += this->dir * (this->size * 0.5);
	this->life--;
}
