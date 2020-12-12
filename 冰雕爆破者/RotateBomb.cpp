#include "stdafx.h"
#include "StaticBomb.h"
#include "RotateBomb.h"


CRotateBomb::CRotateBomb()
{
	m_eType = CBaseBomb::ROTATE;
	vector<TSTRING> names;
	names.push_back(_T("Bombomet1"));
	names.push_back(_T("Bomb_Anim"));
	CSprite::Init(names);	//初始化图片精灵
	CSprite::SetCurAnimation(_T("Bombomet1"));	//设置当前的图片
	CSprite::SetCenter(CSprite::ECEN_CENTER | CSprite::ECEN_MIDDLE);	//设置中心点
	for (int i = 0; i < 3; i++)
	{
		m_FillBombs[i] = new CStaticBomb;
		m_FillBombs[i]->SetColor(CStaticBomb::VIOLET);
		m_FillBombs[i]->SetXScale(0.3);
		m_FillBombs[i]->SetYScale(0.3);
	}
	m_fSpeed = 0;
	m_vecDir = { 0,0 };
	m_fDamping = 0.1;
	m_fDampFactor = 0.17;
	m_colRound = 15;
}


CRotateBomb::~CRotateBomb()
{
	for(int i = 0;i<3;i++)
	{
		if (m_FillBombs[i])
			delete m_FillBombs[i];
	}
}

void CRotateBomb::Update()
{
	CBaseBomb::Update();
	switch (m_eState)
	{
	case CBaseBomb::PREPAREING:
	{
		//计算三个炸弹的位置
		D3DXVECTOR2 dir = { 0,-1 };
		D3DXMATRIX mat;
		D3DXMatrixRotationZ(&mat, m_angle);
		D3DXVec2TransformNormal(&dir, &dir, &mat);	//计算初始旋转
		D3DXMatrixRotationZ(&mat, D3DX_PI * 2 / 3);	//计算每次的旋转矩阵
		for (int i = 0; i < 3; i++)
		{
			if (m_FillBombs[i])
			{
				m_FillBombs[i]->SetXScale(m_xScale * 0.75);
				m_FillBombs[i]->SetYScale(m_xScale * 0.75);
				m_FillBombs[i]->SetDesPos(m_sDesPos + dir * 20 * m_xScale);
				D3DXVec2TransformNormal(&dir, &dir, &mat);	//计算每次的增量旋转
			}
		}
	}
		break;
	case CBaseBomb::FLYING:
	{
		m_angle += m_fSpeed / 10 + 0.2;
		while (m_angle >= D3DX_PI * 2) m_angle -= D3DX_PI * 2;
		//计算三个炸弹的位置
		D3DXVECTOR2 dir = { 0,-1 };
		D3DXMATRIX mat;
		D3DXMatrixRotationZ(&mat, m_angle);
		D3DXVec2TransformNormal(&dir, &dir, &mat);	//计算初始旋转
		D3DXMatrixRotationZ(&mat, D3DX_PI * 2 / 3);	//计算每次的旋转矩阵
		for (int i = 0; i < 3; i++)
		{
			if(m_FillBombs[i])
			{
				m_FillBombs[i]->SetDesPos(m_sDesPos + dir * 8);
				D3DXVec2TransformNormal(&dir, &dir, &mat);	//计算每次的增量旋转
			}
		}
		if (m_fSpeed <= 0)
		{
			for (int i = 0; i < 3; i++)
			{
				if (m_FillBombs[i])
				{
					delete m_FillBombs[i];
					m_FillBombs[i] = nullptr;
				}
			}
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

void CRotateBomb::Draw(bool updateMatrix)
{
	for (int i = 0; i < 3; i++)
	{
		if (m_FillBombs[i])
		{
			m_FillBombs[i]->Draw();
		}
	}
	CSprite::Draw();
}

void CRotateBomb::OnMouseDown(SEvent event)
{
	for(int i = 0;i<3;i++)
	{
		if(m_FillBombs[i])
		{
			CAudio4Bass::GetInstance()->ChannelPlay(_T("intoIce"), true);
			m_FillBombs[i]->SetDesPos(m_sDesPos);
			m_FillBombs[i]->SetXScale(0.5);
			m_FillBombs[i]->SetYScale(0.5);
			CLevelManager::GetInstance()->AddStaticBomb(m_FillBombs[i]);
			m_FillBombs[i] = nullptr;
			return;
		}
	}
}

void CRotateBomb::FreeBomb(const D3DXVECTOR2 & dir, float speed)
{
	CBaseBomb::FreeBomb(dir, speed);
	CAudio4Bass::GetInstance()->ChannelPlay(_T("rotateFly1"), true);
}

void CRotateBomb::SetDesPos(D3DXVECTOR2 desPos)
{
	CSprite::SetDesPos(desPos);
	//计算三个炸弹的位置
	D3DXVECTOR2 dir = { 0,-1 };
	D3DXMATRIX mat;
	D3DXMatrixRotationZ(&mat, m_angle);
	D3DXVec2TransformNormal(&dir, &dir, &mat);	//计算初始旋转
	D3DXMatrixRotationZ(&mat, D3DX_PI * 2 / 3);	//计算每次的旋转矩阵
	for (int i = 0; i < 3; i++)
	{
		if (m_FillBombs[i])
		{
			m_FillBombs[i]->SetXScale(m_xScale * 0.75);
			m_FillBombs[i]->SetYScale(m_xScale * 0.75);
			m_FillBombs[i]->SetDesPos(m_sDesPos + dir * 20 * m_xScale);
			D3DXVec2TransformNormal(&dir, &dir, &mat);	//计算每次的增量旋转
		}
	}
}
