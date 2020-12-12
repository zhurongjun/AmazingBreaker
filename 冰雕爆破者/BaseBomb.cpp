#include "stdafx.h"
#include "BaseBomb.h"


CBaseBomb::CBaseBomb()
{
	m_eState = CBaseBomb::PREPAREING;
}


CBaseBomb::~CBaseBomb()
{
}

void CBaseBomb::SetDir(const D3DXVECTOR2 & dir)
{
	D3DXVec2Normalize(&m_vecDir, &dir);
}

void CBaseBomb::FreeBomb(const D3DXVECTOR2 & dir, float speed)
{
	D3DXVec2Normalize(&m_vecDir, &dir);
	m_fSpeed = speed;
	m_eState = FLYING;
}

void CBaseBomb::Update()
{
	switch (m_eState)
	{
	case CBaseBomb::PREPAREING:
		break;
	case CBaseBomb::FLYING:
	{
		BOOL  Collised = FALSE;
		SIZE * winSize = (SIZE *)CPlatform::GetInstance()->GetPlatData(_T("winsize"));
		m_sDesPos += m_vecDir * m_fSpeed;
		if(m_fSpeed <= 0)
		{
			m_eState = SECOND;
		}
		if(m_sDesPos.x <= 20)
		{
			m_sDesPos.x = 20;
			m_vecDir.x = -m_vecDir.x;
			Collised = TRUE;
		}
		if (m_sDesPos.y <= 20)
		{
			m_sDesPos.y = 20;
			m_vecDir.y = -m_vecDir.y;
			Collised = TRUE;
		}
		if (m_sDesPos.x >= winSize->cx - 20)
		{
			m_sDesPos.x = winSize->cx - 20;
			m_vecDir.x = -m_vecDir.x;
			Collised = TRUE;
		}
		if (m_sDesPos.y >= winSize->cy + 20)
		{
			m_eState = DESTORY;
		}
		if(Collised)
		{
			CAudio4Bass::GetInstance()->ChannelPlay(_T("collision"), true);
		}
		m_fSpeed -= m_fDamping * (m_fSpeed <= 1 ? 1 : m_fSpeed) * m_fDampFactor;
	}
		break;
	case CBaseBomb::SECOND:
		break;
	case CBaseBomb::BOMB:
		break;
	case CBaseBomb::DESTORY:
		break;
	}
}
