#include "stdafx.h"
#include "RoundButton.h"


CRoundButton::CRoundButton(TSTRING Normal, TSTRING MouseIn, TSTRING Pressed)
{
	m_bIsPressed = FALSE;
	m_pNormal = CAnimManager::GetInstance()->GetAnimHander(Normal)->Copy();
	m_pMouseIn = CAnimManager::GetInstance()->GetAnimHander(MouseIn)->Copy();
	m_pPressed = CAnimManager::GetInstance()->GetAnimHander(Pressed)->Copy();
	m_pNormal->SetMainTarget(this);
	m_pNormal->UpdateFixPos();
	m_pMouseIn->SetMainTarget(this);
	m_pMouseIn->UpdateFixPos();
	m_pPressed->SetMainTarget(this);
	m_pPressed->UpdateFixPos();
	m_curState = m_pNormal;
	m_nextState = nullptr;
}

CRoundButton::~CRoundButton()
{
	m_pPressed->Release();
	m_pNormal->Release();
	m_pMouseIn->Release();
	delete m_pPressed;
	delete m_pNormal;
	delete m_pMouseIn;
}

void CRoundButton::SetDesPos(D3DXVECTOR2 desPos)
{
	D3DXVECTOR2 move = desPos - m_vecDesPos;
	m_vecDesPos = desPos;
	m_CollisionArr.left += (int)move.x;
	m_CollisionArr.right += (int)move.x;
	m_CollisionArr.top += (int)move.y;
	m_CollisionArr.bottom += (int)move.y;
	m_pNormal->UpdateFixPos();
	m_pPressed->UpdateFixPos();
	m_pMouseIn->UpdateFixPos();
}

void CRoundButton::Render()
{
	m_curState->Render();
	m_curState->Update();
	if (m_curState->IsEnd() && m_nextState)
	{
		m_curState = m_nextState;
		m_nextState = nullptr;
		m_curState->Reset();
	}
}

void CRoundButton::OnMouseDown(SEvent event)
{
	POINT pt;
	pt.x = event.mouseInput.x;
	pt.y = event.mouseInput.y;
	if (this->isCollision(pt))
	{
		if (!m_sDowmSound.empty())
		{
			CAudio4Bass::GetInstance()->ChannelPlay(m_sDowmSound, true);
		}
		m_nextState = m_pPressed;
		m_bIsPressed = TRUE;
	}
}

void CRoundButton::OnMouseUp(SEvent event)
{
	POINT pt;
	pt.x = event.mouseInput.x;
	pt.y = event.mouseInput.y;
	if (this->isCollision(pt) && m_bIsPressed)
	{
		if (m_pCallback)
			m_pCallback(0);
		m_nextState = m_pNormal;
		m_bIsPressed = FALSE;
	}
}

void CRoundButton::OnMouseMove(SEvent event)
{
	POINT pt;
	pt.x = event.mouseInput.x;
	pt.y = event.mouseInput.y;
	if (this->isCollision(pt))
	{
		if (!m_bMouseIn)
		{
			m_bMouseIn = TRUE;
			m_nextState = m_pMouseIn;
		}
	}
	else
	{
		if (m_bMouseIn)
		{
			m_bMouseIn = FALSE;
			m_nextState = m_pNormal;
		}
	}
}

bool CRoundButton::isCollision(POINT pt)
{
	D3DXVECTOR2 pos = { (float)pt.x,(float)pt.y };
	pos -= m_vecDesPos;
	if (D3DXVec2Length(&pos) <= m_fRound)
		return true;
	return false;
}

void CRoundButton::Refresh()
{
	m_pNormal->Update();
	m_pPressed->Update();
	m_pMouseIn->Update();
	m_pNormal->Reset();
	m_pPressed->Reset();
	m_pMouseIn->Reset();
}
