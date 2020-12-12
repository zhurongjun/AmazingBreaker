#include "stdafx.h"
#include "LevelButtom.h"
#include "SelectScene.h"


CSelectScene::CSelectScene()
{
}


CSelectScene::~CSelectScene()
{
	delete m_pBackground;
	delete m_pBackButtom;
	delete m_pStage;
	delete m_pStageNum;
	delete m_pLine;
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			delete m_pButtoms[i][j];
		}
	}
}

void CSelectScene::Init()
{
	SIZE * winSize = (SIZE *)CPlatform::GetInstance()->GetPlatData((_T("winsize")));

	//初始化精灵
	m_pBackground = new CSprite;
	m_pBackground->Init(_T("bg_light"));
	m_pBackground->SetDesPos({ 0,0 });
	m_pBackground->AjustScale(*winSize);

	vector<TSTRING> names;
	m_pBackButtom = new Button(_T("back_N"), _T("back_I"), _T("back_D"));
	m_pBackButtom->SetDesPos({ 100,680 });
	m_pBackButtom->setCollisionArr(m_pBackButtom->GetRect(170,68));
	m_pBackButtom->setCallBack(bind(&CSelectScene::backToMain, this, std::placeholders::_1));
	m_pBackButtom->Refresh();
	m_pBackButtom->SetDownSound(_T("simpleClick"));

	names.clear();
	m_pStage = new CSprite;
	m_pStage->Init(_T("stage_label"));
	m_pStage->SetCenter(CSprite::ECEN_CENTER | CSprite::ECEN_MIDDLE);
	m_pStage->SetXScale(0.6);
	m_pStage->SetYScale(0.6);
	m_pStage->SetDesPos({ 370,668 });

	m_pStageNum = new CSprite;
	names.push_back(_T("stageNum1"));
	names.push_back(_T("stageNum2"));
	names.push_back(_T("stageNum3"));
	names.push_back(_T("stageNum4"));
	names.push_back(_T("stageNum5"));
	names.push_back(_T("stageNum6"));
	m_pStageNum->Init(names);
	m_pStageNum->SetCenter(CSprite::ECEN_CENTER | CSprite::ECEN_MIDDLE);
	m_pStageNum->SetDesPos({ 435,668 });
	m_pStageNum->SetCurAnimation(_T("stageNum1"));

	m_pLine = new CSprite;
	m_pLine->Init(_T("mmflare1"));
	m_pLine->SetCenter(CSprite::ECEN_CENTER|CSprite::ECEN_MIDDLE);
	m_pLine->SetXScale(0.06);
	m_pLine->SetYScale(0.5);

	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				m_pButtoms[i][j * 4 + k] = new CLevelButtom;
				m_pButtoms[i][j * 4 + k]->Init();
				m_pButtoms[i][j * 4 + k]->SetDesPos({ (float)k * 110 + 75 + i * 480,(float)j * 125 + 80 });
				m_pButtoms[i][j * 4 + k]->SetNum(i * 20 + j * 4 + k + 1);
				m_pButtoms[i][j * 4 + k]->SetLocked(!CLevelManager::GetInstance()->LevelIsOpen(i * 20 + j * 4 + k + 1));
				m_pButtoms[i][j * 4 + k]->SetStar(CLevelManager::GetInstance()->LevelScore(i * 20 + j * 4 + k + 1));
				m_pButtoms[i][j * 4 + k]->SetBackfun(bind(&CSelectScene::beginGame, this, placeholders::_1));
				if (j == 4)
					m_pButtoms[i][j * 4 + k]->SetTieDraw(true);
				else
					m_pButtoms[i][j * 4 + k]->SetTieDraw(false);
			}
		}
	}

	m_iCurStage = 0;
}

void CSelectScene::Render()
{
	m_pBackground->Draw();
	m_pBackButtom->Render();
	m_pStage->Draw();
	m_pStageNum->Draw();
	D3DXVECTOR2 vec;
	//绘制当前页的所有组件
	switch (m_eState)
	{
	case CSelectScene::NORMAL:
	case CSelectScene::TURNIN:
		for (int i = 0; i < 20; i++)
		{
			vec = m_pButtoms[m_iCurStage][i]->GetDesPos();
			vec.y -= 80;
			m_pLine->SetDesPos(vec);
			m_pLine->Draw();
		}
		for (int i = 0; i < 20; i++)
		{
			m_pButtoms[m_iCurStage][i]->Draw();
		}
		break;
	case CSelectScene::NEXT:
		for (int i = 0; i < 20; i++)
		{
			vec = m_pButtoms[m_iCurStage][i]->GetDesPos();
			vec.y -= 80;
			m_pLine->SetDesPos(vec);
			m_pLine->Draw();
			vec = m_pButtoms[m_iCurStage + 1][i]->GetDesPos();
			vec.y -= 80;
			m_pLine->SetDesPos(vec);
			m_pLine->Draw();
		}
		for (int i = 0; i < 20; i++)
		{
			m_pButtoms[m_iCurStage][i]->Draw();
			m_pButtoms[m_iCurStage + 1][i]->Draw();
		}
		break;
	case CSelectScene::LAST:
		for (int i = 0; i < 20; i++)
		{
			vec = m_pButtoms[m_iCurStage][i]->GetDesPos();
			vec.y -= 80;
			m_pLine->SetDesPos(vec);
			m_pLine->Draw();
			vec = m_pButtoms[m_iCurStage - 1][i]->GetDesPos();
			vec.y -= 80;
			m_pLine->SetDesPos(vec);
			m_pLine->Draw();
		}
		for (int i = 0; i < 20; i++)
		{
			m_pButtoms[m_iCurStage][i]->Draw();
			m_pButtoms[m_iCurStage - 1][i]->Draw();
		}
		break;
	}
}

void CSelectScene::Update()
{
	if (!m_nextScene.empty())
	{
		CGameManager::GetInstance()->setNowScene(m_nextScene);
		m_nextScene.clear();
		return;
	}
	//更新目前的数字
	TCHAR  numName[128];
	_stprintf(numName, _T("stageNum%d"), m_iCurStage + 1);
	m_pStageNum->SetCurAnimation(numName);

	//根据状态更新
	switch (m_eState)
	{
	case CSelectScene::NORMAL:
		break;
	case CSelectScene::TURNIN:
	{
		m_iStateCount++;
		D3DXVECTOR2 pos;
		if (m_iStateCount <= 15)
		{
			for (int i = 0; i < 20; i++)
			{
				pos = m_pButtoms[m_iCurStage][i]->GetDesPos();
				pos.y += 41;
				m_pButtoms[m_iCurStage][i]->SetDesPos(pos);
			}
		}
		else
		{
			for (int i = 0; i < 20; i++)
			{
				pos = m_pButtoms[m_iCurStage][i]->GetDesPos();
				pos.y -= 3;
				m_pButtoms[m_iCurStage][i]->SetDesPos(pos);
			}
		}
		if (m_iStateCount >= 20)
		{
			CInputManager::GetInstance()->AddMouseListener(_T("选择界面"), this);
			CInputManager::GetInstance()->AddMouseListener(_T("返回按钮"), m_pBackButtom);
			m_eState = NORMAL;
		}
	}
		break;
	case CSelectScene::NEXT:
	{
		m_iStateCount++;
		D3DXVECTOR2 pos;
		for (int i = 0; i < 6; i++)
		{
			for (int j = 0; j < 20; j++)
			{
				pos = m_pButtoms[i][j]->GetDesPos();
				pos.x -= 24;
				m_pButtoms[i][j]->SetDesPos(pos);
			}
		}
		if (m_iStateCount <= 10)
		{
			m_pStage->SetTransparent(255 - 25 * m_iStateCount);
			m_pStageNum->SetTransparent(255 - 25 * m_iStateCount);
		}
		else
		{
			//更新目前的数字
			TCHAR  numName[128];
			_stprintf(numName, _T("stageNum%d"), m_iCurStage + 2);
			m_pStageNum->SetCurAnimation(numName);
			m_pStageNum->SetTransparent(25 * (m_iStateCount - 10) + 5);
			m_pStage->SetTransparent(25 * (m_iStateCount - 10) + 5);
		}
		if (m_iStateCount >= 20)
		{
			m_iCurStage++;
			m_eState = NORMAL;
		}
	}
		break;
	case CSelectScene::LAST:
	{
		m_iStateCount++;
		D3DXVECTOR2 pos;
		for (int i = 0; i < 6; i++)
		{
			for (int j = 0; j < 20; j++)
			{
				pos = m_pButtoms[i][j]->GetDesPos();
				pos.x += 24;
				m_pButtoms[i][j]->SetDesPos(pos);
			}
		}
		if (m_iStateCount <= 10)
		{
			m_pStage->SetTransparent(255 - 25 * m_iStateCount);
			m_pStageNum->SetTransparent(255 - 25 * m_iStateCount);
		}
		else
		{
			//更新目前的数字
			TCHAR  numName[128];
			_stprintf(numName, _T("stageNum%d"), m_iCurStage);
			m_pStageNum->SetCurAnimation(numName);
			m_pStageNum->SetTransparent(25 * (m_iStateCount - 10) + 5);
			m_pStage->SetTransparent(25 * (m_iStateCount - 10) + 5);
		}
		if (m_iStateCount >= 20)
		{
			m_iCurStage--;
			m_eState = NORMAL;
		}
	}
		break;
	}
}

void CSelectScene::TurnIn()
{
	//更新按钮数据
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			m_pButtoms[i][j]->SetLocked(!CLevelManager::GetInstance()->LevelIsOpen(i * 20 + j + 1));
			m_pButtoms[i][j]->SetStar(CLevelManager::GetInstance()->LevelScore(i * 20 + j + 1));
		}
	}

	m_eState = TURNIN;
	D3DXVECTOR2 pos;
	for (int i = 0; i < 20; i++)
	{
		pos = m_pButtoms[m_iCurStage][i]->GetDesPos();
		pos.y -= 600;
		m_pButtoms[m_iCurStage][i]->SetDesPos(pos);
	}
	m_iStateCount = 0;
}

void CSelectScene::TurnOut()
{
	CInputManager::GetInstance()->DeleteMouseListener(_T("选择界面"));
	CInputManager::GetInstance()->DeleteMouseListener(_T("返回按钮"));
}

void CSelectScene::OnMouseDown(SEvent event)
{
	if (m_eState != NORMAL) return;
	for (int i = 0; i < 20; i++)
	{
		m_pButtoms[m_iCurStage][i]->OnMouseDown(event);
	}
}

void CSelectScene::OnMouseUp(SEvent event)
{
	if (m_eState != NORMAL) return;
	for (int i = 0; i < 20; i++)
	{
		m_pButtoms[m_iCurStage][i]->OnMouseUp(event);
	}
}

void CSelectScene::OnMouseMove(SEvent event)
{
	if (m_eState != NORMAL) return;
	for (int i = 0; i < 20; i++)
	{
		m_pButtoms[m_iCurStage][i]->OnMouseMove(event);
	}
}

void CSelectScene::OnMouseWheel(SEvent event)
{
	if (m_eState != NORMAL) return;
	if (event.mouseInput.x > 0 && m_iCurStage > 0)
	{
		CAudio4Bass::GetInstance()->ChannelPlay(_T("change"), false);
		m_eState = LAST;
		m_iStateCount = 0;
	}
	else if (event.mouseInput.x < 0 && m_iCurStage < 5)
	{
		CAudio4Bass::GetInstance()->ChannelPlay(_T("change"), false);
		m_eState = NEXT;
		m_iStateCount = 0;
	}
}

void CSelectScene::backToMain(int num)
{
	m_nextScene = _T("进入界面");
}

void CSelectScene::beginGame(int num)
{
	m_nextScene = _T("等待界面");
	TCHAR paramWord[256];
	_stprintf(paramWord, _T("%d,%d,%s"), 60, num,_T("游戏界面"));
	CGameManager::GetInstance()->setParam(paramWord);
	CAudio4Bass::GetInstance()->ChannelPlay(_T("simpleClick"), true);
}
