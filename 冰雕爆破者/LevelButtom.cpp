#include "stdafx.h"
#include "LevelButtom.h"


CLevelButtom::CLevelButtom()
{
}


CLevelButtom::~CLevelButtom()
{
	delete m_pNumDrawer;
	delete m_pTie;
	delete m_pStar;
}

void CLevelButtom::Init()
{
	m_bLocked = TRUE;
	m_iStar = 0;
	vector<TSTRING> names;
	names.push_back(_T("level_locked"));
	names.push_back(_T("level_open"));
	CSprite::Init(names);
	CSprite::m_xScale = 0.6;
	CSprite::m_yScale = 0.6;
	CSprite::SetCenter(ECEN_CENTER | ECEN_MIDDLE);

	names.clear();
	m_pNumDrawer = new CSprite;
	names.push_back(_T("0"));
	names.push_back(_T("1"));
	names.push_back(_T("2"));
	names.push_back(_T("3"));
	names.push_back(_T("4"));
	names.push_back(_T("5"));
	names.push_back(_T("6"));
	names.push_back(_T("7"));
	names.push_back(_T("8"));
	names.push_back(_T("9"));
	m_pNumDrawer->Init(names);
	m_pNumDrawer->SetCenter(ECEN_CENTER | ECEN_MIDDLE);
	m_pNumDrawer->SetXScale(0.6);
	m_pNumDrawer->SetYScale(0.6);
	m_pNumDrawer->SetTransparent(220);

	m_pTie = new CSprite;
	m_pTie->Init(_T("slbow"));
	m_pTie->SetCenter(ECEN_CENTER | ECEN_MIDDLE);
	m_pTie->SetXScale(0.7);
	m_pTie->SetYScale(0.7);

	m_pStar = new CSprite;
	m_pStar->Init(_T("slstar"));
	m_pStar->SetXScale(0.3);
	m_pStar->SetYScale(0.3);
	m_pStar->SetCenter(ECEN_CENTER | ECEN_MIDDLE);
}

void CLevelButtom::Draw()
{
	if (m_bDrawTie)
		m_pTie->Draw();
	CSprite::Draw();
	int num = m_iNum;
	int count = 0;
	TCHAR numStr[6];
	D3DXVECTOR2 pos = m_sDesPos;
	pos.y -= 20;
	//确定数字位数
	while (num)
	{
		count++;
		num /= 10;
	}
	num = m_iNum;
	//计算宽度缩放
	pos.x += (count - 1) / 2.0 * 17;
	//绘制数字
	for (int i = 0; i < count; i++)
	{
		_stprintf(numStr, _T("%d"), num % 10);
		m_pNumDrawer->SetCurAnimation(numStr);
		m_pNumDrawer->SetDesPos(pos);
		m_pNumDrawer->Draw();
		pos.x -= 17;
		num /= 10;
	}
	if (m_bLocked) return;
	//绘制星星
	pos = m_sDesPos;
	switch (m_iStar)
	{
	case 1:
		pos.x -= 13;
		pos.y += 9;
		m_pStar->SetDesPos(pos);
		m_pStar->Draw();
		break;
	case 2:
		pos.x -= 13;
		pos.y += 9;
		m_pStar->SetDesPos(pos);
		m_pStar->Draw();
		pos.x += 14;
		pos.y += 2;
		m_pStar->SetDesPos(pos);
		m_pStar->Draw();
		break;
	case 3:
		pos.x -= 13;
		pos.y += 9;
		m_pStar->SetDesPos(pos);
		m_pStar->Draw();
		pos.x += 27;
		m_pStar->SetDesPos(pos);
		m_pStar->Draw();
		pos.x -= 13;
		pos.y += 2;
		m_pStar->SetDesPos(pos);
		m_pStar->Draw();
		break;
	}
}

void CLevelButtom::SetDesPos(D3DXVECTOR2 desPos)
{
	CSprite::SetDesPos(desPos);
	desPos.y -= 37;
	desPos.x -= 5;
	//更新子元素的位置
	m_pTie->SetDesPos(desPos);
	m_CollisionArr = this->getDrawRect();
}

void CLevelButtom::SetLocked(bool locked)
{
	m_bLocked = locked;
	if (m_bLocked)
		m_curAnimation = _T("level_locked");
	else
		m_curAnimation = _T("level_open");
}

void CLevelButtom::OnMouseDown(SEvent event)
{
	POINT pt = { event.mouseInput.x,event.mouseInput.y };
	if (CCollisionObj::isCollision(pt))
	{
		pt.x -= m_sDesPos.x;
		pt.y -= m_sDesPos.y;
		pt.x = (float)pt.x / m_xScale;
		pt.y = (float)pt.y / m_xScale;
		pt.x += 75;
		pt.y += 75;
		if (CResManager::GetInstance()->GetTexture(m_curAnimation)->GetBlendValue(pt))
		{
			m_xScale = 0.6;
			m_yScale = 0.6;
			m_pNumDrawer->SetXScale(0.6);
			m_pNumDrawer->SetYScale(0.6);
			m_bPressed = TRUE;
		}
		else
		{
			m_xScale = 0.65;
			m_yScale = 0.65;
			m_pNumDrawer->SetXScale(0.65);
			m_pNumDrawer->SetYScale(0.65);
		}
	}
	else
	{
		m_xScale = 0.6;
		m_yScale = 0.6;
		m_pNumDrawer->SetXScale(0.6);
		m_pNumDrawer->SetYScale(0.6);
	}
}

void CLevelButtom::OnMouseUp(SEvent event)
{
	POINT pt = { event.mouseInput.x,event.mouseInput.y };
	if (CCollisionObj::isCollision(pt))
	{
		pt.x -= m_sDesPos.x;
		pt.y -= m_sDesPos.y;
		pt.x = (float)pt.x / m_xScale;
		pt.y = (float)pt.y / m_xScale;
		pt.x += 75;
		pt.y += 75;
		if (CResManager::GetInstance()->GetTexture(m_curAnimation)->GetBlendValue(pt))
		{
			if (m_bPressed)
			{
				if (m_pBackFun && !m_bLocked)
					m_pBackFun(m_iNum);
				m_bPressed = false;
			}
			m_xScale = 0.6;
			m_yScale = 0.6;
			m_pNumDrawer->SetXScale(0.6);
			m_pNumDrawer->SetYScale(0.6);
		}
		else
		{
			m_xScale = 0.65;
			m_yScale = 0.65;
			m_pNumDrawer->SetXScale(0.65);
			m_pNumDrawer->SetYScale(0.65);
		}
	}
	else
	{
		m_xScale = 0.6;
		m_yScale = 0.6;
		m_pNumDrawer->SetXScale(0.6);
		m_pNumDrawer->SetYScale(0.6);
	}
}

void CLevelButtom::OnMouseMove(SEvent event)
{
	POINT pt = { event.mouseInput.x,event.mouseInput.y };
	if (CCollisionObj::isCollision(pt))
	{
		pt.x -= m_sDesPos.x;
		pt.y -= m_sDesPos.y;
		pt.x = (float)pt.x / m_xScale;
		pt.y = (float)pt.y / m_xScale;
		pt.x += 75;
		pt.y += 75;
		if (CResManager::GetInstance()->GetTexture(m_curAnimation)->GetBlendValue(pt))
		{
			m_xScale = 0.65;
			m_yScale = 0.65;
			m_pNumDrawer->SetXScale(0.65);
			m_pNumDrawer->SetYScale(0.65);
		}
		else
		{
			m_xScale = 0.6;
			m_yScale = 0.6;
			m_pNumDrawer->SetXScale(0.6);
			m_pNumDrawer->SetYScale(0.6);
		}
	}
	else
	{
		m_xScale = 0.6;
		m_yScale = 0.6;
		m_pNumDrawer->SetXScale(0.6);
		m_pNumDrawer->SetYScale(0.6);
	}
}
