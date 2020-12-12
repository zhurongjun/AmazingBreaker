#include "stdafx.h"
#include "AnimHander.h"


CAnimHander::CAnimHander()
{
	m_bIsEnd = FALSE;
	m_inowLoop = 0;
	m_iCurCount = 0;
}


CAnimHander::~CAnimHander()
{
	for (auto it : m_vecChildAnim)
	{
		delete it;
	}
}

void CAnimHander::Release()
{
	for (auto it : m_mapFixTarget)
	{
		delete it.second;
	}
}

void CAnimHander::init(TiXmlElement * element)
{
	TCHAR	Tstr[256];
	char	Astr[256];
	TiXmlElement * tempElement;
	CAnimHander * tempAnimHander;
	strcpy(Astr,element->Value());

	if (!strcmp(Astr, "loop"))	//循环节点
	{
		m_eType = eANIM_LOOP;
		tempElement = element->FirstChildElement();
		while (tempElement)
		{
			tempAnimHander = new CAnimHander;
			tempAnimHander->init(tempElement);
			m_vecChildAnim.push_back(tempAnimHander);
			tempElement = tempElement->NextSiblingElement();
		}

		element->Attribute("set", &m_bIsSet);
		element->Attribute("param", &m_iParam);
		element->Attribute("loop", &m_iAllLoop);
		element->Attribute("delay", &m_iDelay);
		return;
	}
	else if (!strcmp(Astr, "rotate"))	//旋转节点
	{
		m_eType = eANIM_ROTATE;
	}
	else if (!strcmp(Astr, "posx"))	//x位置节点
	{
		m_eType = eANIM_POSX;
	}
	else if (!strcmp(Astr, "posy"))	//y位置节点
	{
		m_eType = eANIM_POSY;
	}
	else if (!strcmp(Astr, "scalingx"))	//x缩放节点
	{
		m_eType = eANIM_SCALINGX;
	}
	else if (!strcmp(Astr, "scalingy"))	//y缩放节点
	{
		m_eType = eANIM_SCALINGY;
	}
	else if (!strcmp(Astr, "blend"))	//透明度节点
	{
		m_eType = eANIM_BLEND;
	}
	else
		return;
	strcpy(Astr, element->Attribute("target"));
	Anis_To_Auto(Astr, 256, Tstr, 256);
	m_sTarget = Tstr;
	element->Attribute("set", &m_bIsSet);
	element->Attribute("param", &m_iParam);
	element->Attribute("loop", &m_iAllLoop);
	element->Attribute("delay", &m_iDelay);
}

void CAnimHander::SetMainTarget(IAnimable * target)
{
	m_pMainTarget = target;
	for (auto it : m_vecChildAnim)
	{
		it->SetMainTarget(target);
	}
}

void CAnimHander::addFixTarget(TSTRING name, IAnimable * target)
{
	m_mapFixTarget[name] = target;
	for(auto it:m_vecChildAnim)
	{
		it->m_mapFixTarget = m_mapFixTarget;
	}
}

void CAnimHander::UpdateFixPos()
{
	D3DXVECTOR2 pos = m_pMainTarget->GetDesPos();
	for (auto it : m_mapFixTarget)
	{
		it.second->SetDesPos(pos);
	}
	for (auto it : m_vecChildAnim)
	{
		it->UpdateFixPos();
	}
}

void CAnimHander::LoopRender()
{
	this->Render();
	if (m_bIsEnd)
		this->Reset();
}

CAnimHander * CAnimHander::Copy(bool copyFix)
{
	CAnimHander * newHander = new CAnimHander;
	newHander->m_bIsEnd = m_bIsEnd;
	newHander->m_bIsSet = m_bIsSet;
	newHander->m_eType = m_eType;
	newHander->m_iAllLoop = m_iAllLoop;
	newHander->m_iCurCount = m_iCurCount;
	newHander->m_iDelay = m_iDelay;
	newHander->m_inowLoop = m_inowLoop;
	newHander->m_iParam = m_iParam;
	newHander->m_pMainTarget = m_pMainTarget;
	newHander->m_sTarget = m_sTarget;
	
	//拷贝子节点
	for (auto it : m_vecChildAnim)
	{
		newHander->m_vecChildAnim.push_back(it->Copy(false));
	}

	//拷贝固定元素
	if (copyFix)
	{
		for (auto it : m_mapFixTarget)
		{
			newHander->addFixTarget(it.first, new CSprite(*((CSprite *)it.second)));
		}
	}
	return newHander;
}

void CAnimHander::Render(bool renderFix)
{
	m_pMainTarget->Draw();	//绘制
	for (auto it : m_vecChildAnim)	//子节点绘制
	{
		it->Render(false);
	}
	if (renderFix)
	{
		for (auto it : m_mapFixTarget)
		{
			it.second->Draw();
		}
	}
}

void CAnimHander::Update()
{
	if (m_bIsEnd) return;	//如果循环结束那么就不刷新信息

	if (m_iCurCount >= m_iDelay)	
	{
		//每次循环都会刷新一次内循环的节点
		for (auto it : m_vecChildAnim)
		{
			it->Update();
		}

		//每个节点的刷新
		switch (m_eType)
		{
		case CAnimHander::eANIM_SCALINGX:
			if (m_bIsSet)
			{
				if (m_sTarget.empty())
					m_pMainTarget->SetXScale(m_iParam);
				else
					m_mapFixTarget[m_sTarget]->SetXScale(m_iParam);
			}
			else
			{
				if (m_sTarget.empty())
					m_pMainTarget->SetXScale(m_pMainTarget->GetXScale() + m_iParam);
				else
					m_mapFixTarget[m_sTarget]->SetXScale(m_mapFixTarget[m_sTarget]->GetXScale() + m_iParam);
			}
			break;
		case CAnimHander::eANIM_SCALINGY:
			if (m_bIsSet)
			{
				if (m_sTarget.empty())
					m_pMainTarget->SetYScale(m_iParam);
				else
					m_mapFixTarget[m_sTarget]->SetYScale(m_iParam);
			}
			else
			{
				if (m_sTarget.empty())
					m_pMainTarget->SetYScale(m_pMainTarget->GetYScale() + m_iParam);
				else
					m_mapFixTarget[m_sTarget]->SetYScale(m_mapFixTarget[m_sTarget]->GetYScale() + m_iParam);
			}
			break;
		case CAnimHander::eANIM_POSX:
		{
			D3DXVECTOR2 pos;
			if (m_bIsSet)
			{
				if (m_sTarget.empty())
				{
					pos = m_pMainTarget->GetDesPos();
					pos.x = m_iParam;
					m_pMainTarget->SetDesPos(pos);
				}
				else
				{
					pos = m_mapFixTarget[m_sTarget]->GetDesPos();
					pos.x = m_iParam;
					m_mapFixTarget[m_sTarget]->SetDesPos(pos);
				}
			}
			else
			{
				if (m_sTarget.empty())
				{
					pos = m_pMainTarget->GetDesPos();
					pos.x += m_iParam;
					m_pMainTarget->SetDesPos(pos);
				}
				else
				{
					pos = m_mapFixTarget[m_sTarget]->GetDesPos();
					pos.x += m_iParam;
					m_mapFixTarget[m_sTarget]->SetDesPos(pos);
				}
			}
		}
		break;
		case CAnimHander::eANIM_POSY:
		{
			D3DXVECTOR2 pos;
			if (m_bIsSet)
			{
				if (m_sTarget.empty())
				{
					pos = m_pMainTarget->GetDesPos();
					pos.y = m_iParam;
					m_pMainTarget->SetDesPos(pos);
				}
				else
				{
					pos = m_mapFixTarget[m_sTarget]->GetDesPos();
					pos.y = m_iParam;
					m_mapFixTarget[m_sTarget]->SetDesPos(pos);
				}
			}
			else
			{
				if (m_sTarget.empty())
				{
					pos = m_pMainTarget->GetDesPos();
					pos.y += m_iParam;
					m_pMainTarget->SetDesPos(pos);
				}
				else
				{
					pos = m_mapFixTarget[m_sTarget]->GetDesPos();
					pos.y += m_iParam;
					m_mapFixTarget[m_sTarget]->SetDesPos(pos);
				}
			}
		}
		break;
		case CAnimHander::eANIM_ROTATE:
			if (m_bIsSet)
			{
				if (m_sTarget.empty())
					m_pMainTarget->SetAngle(m_iParam);
				else
					m_mapFixTarget[m_sTarget]->SetAngle(m_iParam);
			}
			else
			{
				if (m_sTarget.empty())
					m_pMainTarget->SetAngle(m_pMainTarget->GetAngle() + m_iParam);
				else
					m_mapFixTarget[m_sTarget]->SetAngle(m_mapFixTarget[m_sTarget]->GetAngle() + m_iParam);
			}
			break;
		case CAnimHander::eANIM_BLEND:
			if (m_bIsSet)
			{
				if (m_sTarget.empty())
					m_pMainTarget->SetTransparent(m_iParam);
				else
					m_mapFixTarget[m_sTarget]->SetTransparent(m_iParam);
			}
			else
			{
				if (m_sTarget.empty())
					m_pMainTarget->SetTransparent(m_pMainTarget->GetTransparent() + m_iParam);
				else
					m_mapFixTarget[m_sTarget]->SetTransparent(m_mapFixTarget[m_sTarget]->GetTransparent() + m_iParam);
			}
			break;
		}
		m_inowLoop++;
		m_iCurCount = 0;
	}
	if (m_inowLoop >= m_iAllLoop)
	{
		m_bIsEnd = TRUE;
	}
	m_iCurCount++;
}

void CAnimHander::Reset()
{
	m_bIsEnd = FALSE;
	m_iCurCount = 0;
	m_inowLoop = 0;
	for (auto it : m_vecChildAnim)
	{
		it->Reset();
	}
	this->Update();
}
