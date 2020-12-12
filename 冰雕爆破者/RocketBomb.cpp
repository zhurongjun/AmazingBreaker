#include "stdafx.h"
#include "RocketBomb.h"


CRocketBomb::CRocketBomb()
{
	m_eType = CBaseBomb::ROCKET;
	vector<TSTRING> names;
	names.push_back(_T("rocket"));
	names.push_back(_T("Bomb_Anim"));
	CSprite::Init(names);	//初始化图片精灵
	CSprite::SetCenter(CSprite::ECEN_CENTER | CSprite::ECEN_MIDDLE);	//设置中心点
	m_fSpeed = 0;
	m_vecDir = { 0,0 };
	m_fDamping = 0;
	m_fDampFactor = 0;
	m_colRound = 8;
	m_vecBeginPos = m_vecEndPos = { 0,0 };
}


CRocketBomb::~CRocketBomb()
{
}

void CRocketBomb::Update()
{
	switch (m_eState)
	{
	case CBaseBomb::PREPAREING:
		break;
	case CBaseBomb::FLYING:
	{
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
						m_eState = BOMB;
						return;
					}
				}
			}
		}
		while (m_angle <= -D3DX_PI) m_angle += 2 * D3DX_PI;
		while (m_angle >= D3DX_PI)	m_angle -= 2 * D3DX_PI;
		BOOL  Collised = FALSE;
		SIZE * winSize = (SIZE *)CPlatform::GetInstance()->GetPlatData(_T("winsize"));
		m_sDesPos += m_vecDir * m_fSpeed;
		if (m_fSpeed <= 0)
		{
			m_eState = SECOND;
		}
		if (m_sDesPos.y <= 5)
		{
			if (m_angle >= -D3DX_PI / 3 && m_angle <= D3DX_PI / 3)
			{
				CAudio4Bass::GetInstance()->ChannelPlay(_T("bomb1"), true);
				m_eState = CBaseBomb::BOMB;
				m_curAnimation = _T("Bomb_Anim");
				CSprite::SetCenter(ECEN_MIDDLE | ECEN_CENTER);
				CSprite::ResetMonoPlay();
				m_iStateCount = m_iStateParam = 0;
				m_eState = BOMB;
				return;
			}
			m_sDesPos.y = 6;
			m_angle = D3DX_PI / 2 * (float)(m_vecDir.x >0 ? 1.0 : -1.0);
			m_vecDir = { (float)(m_vecDir.x >0 ? 1.0 : -1.0),0 };
			Collised = TRUE;
		}
		if (m_sDesPos.x <= 5)
		{
			if ((m_angle >= D3DX_PI / 6 && m_angle <= D3DX_PI * 5 / 6) ||
				(m_angle >= -D3DX_PI * 5 / 6 && m_angle <= -D3DX_PI / 6))
			{
				CAudio4Bass::GetInstance()->ChannelPlay(_T("bomb1"), true);
				m_eState = CBaseBomb::BOMB;
				m_curAnimation = _T("Bomb_Anim");
				CSprite::SetCenter(ECEN_MIDDLE | ECEN_CENTER);
				CSprite::ResetMonoPlay();
				m_iStateCount = m_iStateParam = 0;
				m_eState = BOMB;
				return;
			}
			m_sDesPos.x = 6;
			m_vecDir = { 0,(float)(m_vecDir.y >0 ? 1.0 : -1.0) };
			m_angle = 0 + D3DX_PI * (float)(m_vecDir.y >0 ? 1 : 0);
			Collised = TRUE;
		}
		if (m_sDesPos.x >= winSize->cx - 5)
		{
			if ((m_angle >= D3DX_PI / 6 && m_angle <= D3DX_PI * 5 / 6) ||
				(m_angle >= -D3DX_PI * 5 / 6 && m_angle <= -D3DX_PI / 6))
			{
				CAudio4Bass::GetInstance()->ChannelPlay(_T("bomb1"), true);
				m_eState = CBaseBomb::BOMB;
				m_curAnimation = _T("Bomb_Anim");
				CSprite::SetCenter(ECEN_MIDDLE | ECEN_CENTER);
				CSprite::ResetMonoPlay();
				m_iStateCount = m_iStateParam = 0;
				m_eState = BOMB;
				return;
			}
			m_sDesPos.x = winSize->cx - 6;
			m_angle = 0 + D3DX_PI * (float)(m_vecDir.y > 0 ? 1 : 0);
			m_vecDir = { 0,(float)(m_vecDir.y >0 ? 1.0 : -1.0) };
			Collised = TRUE;
		}
		if (m_sDesPos.y >= winSize->cy + 20)
		{
			m_eState = DESTORY;
		}
		if (Collised)
		{
			CAudio4Bass::GetInstance()->ChannelPlay(_T("collision"), true);
		}
		m_fSpeed -= m_fDamping * (m_fSpeed <= 1 ? 1 : m_fSpeed) * m_fDampFactor;
	}
	break;
	case CBaseBomb::SECOND:
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

void CRocketBomb::OnMouseDown(SEvent event)
{
	if (m_eState != CBaseBomb::FLYING) return;
	m_bPressed = TRUE;
	m_vecBeginPos = m_vecEndPos = { (float)event.mouseInput.x,(float)event.mouseInput.y };
}

void CRocketBomb::OnMouseMove(SEvent event)
{
	if (m_eState != CBaseBomb::FLYING || !m_bPressed) return;
	m_vecEndPos = { (float)event.mouseInput.x,(float)event.mouseInput.y };
	float length = m_vecEndPos.x - m_vecBeginPos.x;

	m_angle += length / 80;
	m_vecDir = { 0,-1 };
	D3DXMATRIX mat;
	D3DXMatrixRotationZ(&mat, m_angle);
	D3DXVec2TransformNormal(&m_vecDir, &m_vecDir, &mat);

	m_vecBeginPos = m_vecEndPos;
}

void CRocketBomb::OnMouseUp(SEvent event)
{
	if (m_eState != CBaseBomb::FLYING) return;
	m_bPressed = FALSE;
}

void CRocketBomb::FreeBomb(const D3DXVECTOR2 & dir, float speed)
{
	CAudio4Bass::GetInstance()->ChannelPlay(_T("rocketFly"), true);
	CBaseBomb::FreeBomb(dir, speed);
	m_fSpeed = 4;
}
