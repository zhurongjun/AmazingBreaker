#include "stdafx.h"
#include "StaticBomb.h"
#include "TripleBomb.h"


CTripleBomb::CTripleBomb()
{
	m_eType = CBaseBomb::TRIPLE;
	vector<TSTRING> names;
	names.push_back(_T("triple_green"));
	names.push_back(_T("Bomb_Anim"));
	CSprite::Init(names);	//初始化图片精灵
	CSprite::SetCurAnimation(_T("triple_green"));	//设置当前的图片
	CSprite::SetCenter(CSprite::ECEN_CENTER | CSprite::ECEN_MIDDLE);	//设置中心点
	for(int i = 0;i<3;i++)
	{
		m_StaticBombs[i] = new CStaticBomb;
		m_StaticBombs[i]->SetColor(CStaticBomb::GREEN);
	}
	m_fSpeed = 0;
	m_vecDir = { 0,0 };
	m_fDamping = 0.1;
	m_fDampFactor = 0.17;
	m_colRound = 15;
}


CTripleBomb::~CTripleBomb()
{
	for(int i = 0;i<3;i++)
	{
		if (m_StaticBombs[i])
			delete m_StaticBombs[i];
	}
}

void CTripleBomb::Update()
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
						CAudio4Bass::GetInstance()->ChannelPlay(_T("bomb1"), true);
						m_eState = CBaseBomb::BOMB;
						m_curAnimation = _T("Bomb_Anim");
						CSprite::SetCenter(ECEN_MIDDLE | ECEN_CENTER);
						CSprite::ResetMonoPlay();
						m_iStateCount = m_iStateParam = 0;
						return;
					}
				}
			}
		}
		if (m_fSpeed <= 0)
		{
			CAudio4Bass::GetInstance()->ChannelPlay(_T("bomb1"), true);
			m_eState = CBaseBomb::BOMB;
			m_curAnimation = _T("Bomb_Anim");
			CSprite::SetCenter(ECEN_MIDDLE | ECEN_CENTER);
			CSprite::ResetMonoPlay();
			m_iStateCount = m_iStateParam = 0;
		}
	}
	break;
	case CBaseBomb::SECOND:
	{
		BOOL Quit = true;
		for (int i = 0; i < 3; i++)
		{
			if (!m_StaticBombs[i]) continue;
			m_StaticBombs[i]->Update();
			if (m_StaticBombs[i]->GetState() == CBaseBomb::FLYING)
				Quit = FALSE;
			else
				m_StaticBombs[i] = nullptr;
		}
		if(Quit)
		{
			for (int i = 0; i < 3; i++)
				m_StaticBombs[i] = nullptr;
			m_eState = DESTORY;
		}
	}
		break;
	case CBaseBomb::BOMB:
	{
		m_iStateCount++;
		if (m_iStateCount == 8)
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

void CTripleBomb::Draw(bool updateMatrix)
{
	if (m_eState == FLYING || m_eState == CBaseBomb::BOMB || m_eState == PREPAREING)
		CSprite::Draw();
	if(m_eState == SECOND)
	{
		for (int i = 0; i < 3; i++)
			if (m_StaticBombs[i])
				m_StaticBombs[i]->Draw();
	}
}

void CTripleBomb::OnMouseDown(SEvent event)
{
	if(m_eState == FLYING)
	{
		m_eState = SECOND;
		float angle = atan2f(m_vecDir.y, m_vecDir.x) + D3DX_PI / 7;
		D3DXVECTOR2 dir = { 1,0 };
		D3DXMATRIX mat;
		D3DXMatrixRotationZ(&mat, angle);
		D3DXVec2TransformNormal(&dir, &dir, &mat);	//计算初始向量
		angle = -D3DX_PI / 7;
		D3DXMatrixRotationZ(&mat, angle);

		for (int i = 0; i < 3; i++)
		{
			m_StaticBombs[i]->SetDesPos(m_sDesPos);
			m_StaticBombs[i]->FreeBomb(dir, m_fSpeed * 0.5 + 3);
			D3DXVec2TransformNormal(&dir, &dir, &mat);	//持续旋转
		}
		CAudio4Bass::GetInstance()->ChannelPlay(_T("greenDepart"),true);
	}
}
