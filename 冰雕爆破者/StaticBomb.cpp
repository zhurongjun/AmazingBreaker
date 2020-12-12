#include "stdafx.h"
#include "BombLine.h"
#include "StaticBomb.h"


CStaticBomb::CStaticBomb()
{
	m_eType = CBaseBomb::STATIC;
	vector<TSTRING> names;
	names.push_back(_T("cristal_red_1"));
	names.push_back(_T("Cristal_green_1"));
	names.push_back(_T("Cristal_violet_1"));
	names.push_back(_T("Cristal_crack1"));
	names.push_back(_T("Bomb_Anim"));

	CSprite::Init(names);	//初始化图片精灵
	CSprite::SetCurAnimation(_T("cristal_red_1"));	//设置当前的图片
	CSprite::SetCenter(CSprite::ECEN_CENTER | CSprite::ECEN_MIDDLE);	//设置中心点
	m_xScale = 0.5;
	m_yScale = 0.5;
	m_color = RED;
	m_fSpeed = 0;
	m_vecDir = { 0,0 };
	m_fDamping = 0;
	m_fDampFactor = 0;
	m_eState = PREPAREING;
	m_colRound = 12;
}


CStaticBomb::~CStaticBomb()
{
	for (auto it : m_listLines)
	{
		delete it;
	}
}


void CStaticBomb::Update()
{
	switch (m_eState)
	{
	case CBaseBomb::PREPAREING:
		for (auto it : m_listLines)
			it->Update();
		break;
	case CBaseBomb::FLYING:
	{
		BOOL  Collised = FALSE;
		SIZE * winSize = (SIZE *)CPlatform::GetInstance()->GetPlatData(_T("winsize"));
		m_sDesPos += m_vecDir * m_fSpeed;
		if (m_fSpeed <= 0)
		{
			m_eState = SECOND;
		}
		if (m_sDesPos.x <= 12)
		{
			m_sDesPos.x = 12;
			m_vecDir.x = -m_vecDir.x;
			Collised = TRUE;
		}
		if (m_sDesPos.y <= 12)
		{
			m_sDesPos.y = 12;
			m_vecDir.y = -m_vecDir.y;
			Collised = TRUE;
		}
		if (m_sDesPos.x >= winSize->cx - 12)
		{
			m_sDesPos.x = winSize->cx - 12;
			m_vecDir.x = -m_vecDir.x;
			Collised = TRUE;
		}
		if (m_sDesPos.y >= winSize->cy + 12)
		{
			m_eState = DESTORY;
		}
		if (Collised)
		{
			CAudio4Bass::GetInstance()->ChannelPlay(_T("collision"), true);
		}
		m_fSpeed -= m_fDamping * (m_fSpeed <= 1 ? 1 : m_fSpeed) * m_fDampFactor;
		D3DXVECTOR2 pos = m_sDesPos;
		pos.x -= m_colRound;
		pos.y -= m_colRound;
		D3DXVECTOR2 colPos;
		CLevelManager * nowLevel = CLevelManager::GetInstance();
		for (int i = 0; i < m_colRound * 2; i++)
		{
			for (int j = 0; j < m_colRound * 2; j++)
			{
				colPos = { pos.x + i,pos.y + j };
				if (D3DXVec2Length(&(colPos - m_sDesPos)) <= m_colRound)
				{
					if (nowLevel->IsCollision(colPos))
					{
						CAudio4Bass::GetInstance()->ChannelPlay(_T("greenCol"), true);
						CLevelManager::GetInstance()->AddStaticBomb(this);
						m_eState = CBaseBomb::PREPAREING;
						m_fSpeed = 0;
						m_iStateCount = m_iStateParam = 0;
						return;
					}
				}
			}
		}
	}
		break;
	case CBaseBomb::SECOND:
		m_iStateCount++;
		if(m_iStateCount >= 1)
		{
			m_iStateCount = m_iStateParam = 0;
			m_eState = BOMB;
			CSprite::ResetMonoPlay();
			CAudio4Bass::GetInstance()->ChannelPlay(_T("bomb1"), true);
			m_curAnimation = _T("Bomb_Anim");
			CSprite::SetCenter(ECEN_MIDDLE | ECEN_CENTER);
			for (auto it : m_listLines)
			{
				delete it;
			}
			m_listLines.clear();
		}
		break;
	case CBaseBomb::BOMB:
		m_iStateCount++;
		if (m_iStateCount == 8)
			CLevelManager::GetInstance()->SendBombInfo(m_sDesPos);
		CSprite::Update();
		CSprite::m_xScale = CSprite::m_yScale = 1.6;
		if (CSprite::isEnd())
		{
			m_bIsVisible = false;
			m_eState = CBaseBomb::DESTORY;
			m_sDesPos = { -1000,-1000 };
		}
		break;
	case CBaseBomb::DESTORY:
		break;
	}


}

void CStaticBomb::SetColor(ECOLOR color)
{
	m_color = color;
	switch (m_color)
	{
	case CStaticBomb::RED:
		m_curAnimation = _T("cristal_red_1");
		break;
	case CStaticBomb::GREEN:
		m_curAnimation = _T("Cristal_green_1");
		break;
	case CStaticBomb::VIOLET:
		m_curAnimation = _T("Cristal_violet_1");
		break;
	}
}

void CStaticBomb::Draw(bool updateMatrix)
{
	CSprite::Draw(updateMatrix);

	
}

void CStaticBomb::DrawLine()
{
	if (m_eState == DESTORY) return;
	//绘制线
	for (auto it : m_listLines)
	{
		it->Draw();
	}
}

void CStaticBomb::Bomb()
{
	if (m_eState != PREPAREING) return;
	m_eState = SECOND;
	m_iStateParam = m_iStateCount = 0;
}

void CStaticBomb::AddLine(D3DXVECTOR2 pos)
{
	CBombLine * temp = new CBombLine;
	temp->Init(m_sDesPos, pos);
	m_listLines.push_back(temp);
}

void CStaticBomb::DeleteLine(D3DXVECTOR2 pos)
{
	for (auto it = m_listLines.begin(); it != m_listLines.end(); it++)
	{
		if ((*it)->GetEndPos() == pos)
		{
			it = m_listLines.erase(it);
		}
	}
}
