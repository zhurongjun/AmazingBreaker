#include "stdafx.h"
#include "BombLine.h"


CBombLine::CBombLine()
{
}


CBombLine::~CBombLine()
{
	delete m_BombLine;
	delete m_BombLight;
}

void CBombLine::Init(D3DXVECTOR2 begin, D3DXVECTOR2 end)
{
	m_iStateCount = 0;
	m_vecBegin = begin;
	m_vecEnd = end;

	m_BombLight = new CSprite;
	m_BombLight->Init(_T("Bomb_Light"));
	m_BombLight->SetXScale(0.3);
	m_BombLight->SetYScale(0.3);
	m_BombLight->SetCenter(CSprite::ECEN_CENTER | CSprite::ECEN_MIDDLE);

	m_BombLine = new CSprite;
	m_BombLine->Init(_T("BOMB_Line"));
	m_vecDir = end - begin;
	m_BombLine->SetXScale(D3DXVec2Length(&m_vecDir) / 20);
	m_BombLine->SetCenter({ 0,1,0 });
	m_BombLine->SetDesPos(begin);
	m_BombLine->SetAngle(atan2f(m_vecDir.y, m_vecDir.x));
	m_BombLight->SetAngle(atan2f(m_vecDir.y, m_vecDir.x));
	m_BombLight->SetDesPos(m_vecBegin);
	D3DXVec2Normalize(&m_vecDir, &m_vecDir);

	m_fSpeed = 5;
}

void CBombLine::Draw()
{
	m_BombLine->Draw();
	if (m_DrawLight)
		m_BombLight->Draw();
}

void CBombLine::Update()
{
	m_iStateCount++;
	if (m_iStateCount >= 30 && !m_DrawLight)
	{
		m_DrawLight = TRUE;
		m_iStateCount = 0;
	}
	if (m_DrawLight)
	{
		m_BombLight->SetDesPos(m_BombLight->GetDesPos() + m_fSpeed * m_vecDir);
		if (D3DXVec2Length(&(m_BombLight->GetDesPos() - m_vecEnd)) <= m_fSpeed)
		{
			m_BombLight->SetDesPos(m_vecBegin);
			m_DrawLight = false;
			m_iStateCount = 0;
		}
	}
}
