#include "stdafx.h"
#include "Button.h"
Button::Button(TSTRING Normal, TSTRING MouseIn, TSTRING Pressed)
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
}

Button::~Button()
{
	m_pPressed->Release();
	m_pNormal->Release();
	m_pMouseIn->Release();
	delete m_pPressed;
	delete m_pNormal;
	delete m_pMouseIn;
}

void Button::SetDesPos(D3DXVECTOR2 desPos)
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
	m_bUseColor = FALSE;
}

void Button::Render()
{
	m_curState->Render();
	m_curState->Update();
}

void Button::OnMouseDown(SEvent event)
{
	POINT pt;
	pt.x = event.mouseInput.x;
	pt.y = event.mouseInput.y;
	if (this->isCollision(pt))
	{
		if(!m_sDowmSound.empty())
		{
			CAudio4Bass::GetInstance()->ChannelPlay(m_sDowmSound, true);
		}
		m_curState = m_pPressed;
		m_curState->Reset();
		m_bIsPressed = TRUE;
	}
}

void Button::OnMouseUp(SEvent event)
{
	POINT pt;
	pt.x = event.mouseInput.x;
	pt.y = event.mouseInput.y;
	if (this->isCollision(pt) && m_bIsPressed)
	{
		if (m_pCallback)
			m_pCallback(0);
		m_bIsPressed = FALSE;
		m_bMouseIn = TRUE;
		m_curState = m_pNormal;
		m_curState->Reset();
	}
	else
		m_bMouseIn = FALSE;
}

void Button::OnMouseMove(SEvent event)
{
	POINT pt;
	pt.x = event.mouseInput.x;
	pt.y = event.mouseInput.y;
	if (this->isCollision(pt))
	{
		m_bMouseIn = TRUE;
		m_curState = m_pMouseIn;
		m_curState->Reset();
	}
	else
	{
		if (m_bMouseIn)
		{
			m_bMouseIn = FALSE;
			m_curState = m_pNormal;
			m_curState->Reset();
		}
	}
}

RECT Button::GetRect(int width, int height)
{
	RECT rc;
	rc.left = (int)m_vecDesPos.x - width * 0.5;
	rc.right = (int)m_vecDesPos.x + width * 0.5;
	rc.top = (int)m_vecDesPos.y - height * 0.5;
	rc.bottom = (int)m_vecDesPos.y + height * 0.5;
	return rc;
}

void Button::Refresh()
{
	m_pNormal->Update();
	m_pPressed->Update();
	m_pMouseIn->Update();
	m_pNormal->Reset();
	m_pPressed->Reset();
	m_pMouseIn->Reset();
}
