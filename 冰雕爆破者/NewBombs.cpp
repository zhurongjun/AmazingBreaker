#include "stdafx.h"
#include "NewBombs.h"


CNewBombs::CNewBombs()
{
	vector<TSTRING> names;

	names.push_back(_T("new_flare"));
	names.push_back(_T("new_bomb_1"));
	names.push_back(_T("new_bomb_2"));
	names.push_back(_T("new_bomb_3"));
	names.push_back(_T("new_bomb_4"));
	names.push_back(_T("new_bomb_5"));
	CSprite::Init(names);
	m_xScale = m_yScale = 0;
	m_fAngle1 = m_fAngle2 = 0;
}


CNewBombs::~CNewBombs()
{
}

void CNewBombs::Draw(bool updateMatrix)
{
	m_curAnimation = _T("new_flare");
	CSprite::SetCenter(ECEN_CENTER | ECEN_MIDDLE);
	m_angle = m_fAngle1;
	CSprite::Draw(updateMatrix);
	m_angle = m_fAngle2;
	CSprite::Draw(updateMatrix);

	switch (m_iType)
	{
	case 1:
		m_curAnimation = _T("new_bomb_1");
		break;
	case 2:
		m_curAnimation = _T("new_bomb_2");
		break;
	case 3:
		m_curAnimation = _T("new_bomb_3");
		break;
	case 4:
		m_curAnimation = _T("new_bomb_4");
		break;
	case 5:
		m_curAnimation = _T("new_bomb_5");
		break;
	}
	CSprite::SetCenter(ECEN_CENTER | ECEN_MIDDLE);
	m_angle = 0;
	CSprite::Draw(updateMatrix);
}

void CNewBombs::Update()
{
	if (m_curBout == m_disappearBout)
	{
		if (m_xScale > 0)
		{
			m_xScale -= 0.02;
			m_yScale -= 0.02;
		}
		else
			m_xScale = m_yScale = 0;
		m_nTransparent = 255 << 24;
		return;
	}
	if (m_xScale < 0.4)
	{
		m_xScale += 0.02;
		m_yScale += 0.02;
	}
	if (this->GetTransparent() > (m_disappearBout - m_curBout) * 255.0 / (m_disappearBout - m_showBout))
	{
		this->SetTransparent(this->GetTransparent() - 3);
	}
	m_fAngle1 += 0.028;
	m_fAngle2 -= 0.028;
}