#include "stdafx.h"
#include "NormalBomb.h"



CNormalBomb::CNormalBomb()
{
	m_eType = CBaseBomb::NORMAL;
	vector<TSTRING> names;
	names.push_back(_T("CrystalBomb1"));
	names.push_back(_T("CrystalBomb2"));
	names.push_back(_T("Bomb_Anim"));
	CSprite::Init(names);	//初始化图片精灵
	CSprite::SetCurAnimation(_T("CrystalBomb1"));	//设置当前的图片
	CSprite::SetCenter(CSprite::ECEN_CENTER | CSprite::ECEN_MIDDLE);	//设置中心点
	m_fSpeed = 0;
	m_vecDir = { 0,0 };
	m_fDamping = 0.1;
	m_fDampFactor = 0.17;
	m_colRound = 15;
}


CNormalBomb::~CNormalBomb()
{
}

void CNormalBomb::Update()
{

	CBaseBomb::Update();
	switch (m_eState)
	{
	case CBaseBomb::PREPAREING:
		break;
	case CBaseBomb::FLYING:
	{
		m_angle += 0.04;
		CLevelManager * nowLevel = CLevelManager::GetInstance();
		D3DXVECTOR2 pos = m_sDesPos;
		D3DXVECTOR2 colPos;
		pos.x -= m_colRound;
		pos.y -= m_colRound;
		for (int i = 0; i < m_colRound * 2; i++)
		{
			for (int j = 0; j < m_colRound * 2; j++)
			{
				colPos = { pos.x + i,pos.y + j };
				if (D3DXVec2Length(&(colPos - m_sDesPos)) <= m_colRound)
				{
					if (nowLevel->IsCollision(colPos))
					{
						CAudio4Bass::GetInstance()->ChannelPlay(_T("intoIce"), true);
						m_eState = CBaseBomb::SECOND;
						m_iStateCount = m_iStateParam = 0;
						return;
					}
				}
			}
		}
		if (m_fSpeed <= 0)
		{
			CAudio4Bass::GetInstance()->ChannelPlay(_T("intoIce"), true);
			m_eState = CBaseBomb::SECOND;
			m_iStateCount = m_iStateParam = 0;
		}
	}
		break;
	case CBaseBomb::SECOND:
		if (m_iStateCount < 35)
		{
			m_iStateCount++;
			if (m_iStateCount % 6 == 0)
			{
				if (m_curAnimation == _T("CrystalBomb1"))
					m_curAnimation = _T("CrystalBomb2");
				else
					m_curAnimation = _T("CrystalBomb1");
			}
		}
		else
		{
			CAudio4Bass::GetInstance()->ChannelPlay(_T("bomb1"), true);
			m_iStateCount = m_iStateParam = 0;
			m_curAnimation = _T("Bomb_Anim");
			CSprite::SetCenter(ECEN_MIDDLE | ECEN_CENTER);
			m_eState = CBaseBomb::BOMB;
			CSprite::ResetMonoPlay();
		}
		break;
	case CBaseBomb::BOMB:
	{
		m_iStateCount++;
		if(m_iStateCount == 8)
			CLevelManager::GetInstance()->SendBombInfo(m_sDesPos);
		CSprite::Update();
		CSprite::m_xScale = CSprite::m_yScale = 1.6;
		if (CSprite::isEnd())
		{
			m_bIsVisible = false;
			m_eState = CBaseBomb::DESTORY;
		}
	}
		break;
	case CBaseBomb::DESTORY:
		break;
	}
}
