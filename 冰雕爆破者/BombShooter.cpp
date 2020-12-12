#include "stdafx.h"
#include "BombShooter.h"

const D3DXVECTOR2 CBombShooter::s_BombStackPos[5] = {
	{ 74,691 },
	{ 43,661 },
	{ 30,635 },
	{ 21,606 },
	{ 18,577 }
};

const float CBombShooter::s_fBigScalling = 0.4;
const float CBombShooter::s_fSmallScalling = 0.25;


CBombShooter::CBombShooter()
{
	m_preAnimCount = 0;
	m_eState = NORMAL;
	m_curBomb = NULL;
}


CBombShooter::~CBombShooter()
{
	this->ClearBomb();
	delete m_pHolder;
	delete m_pMainUI;
	delete m_pPrecentBar;
	delete m_pSpringL;
	delete m_pSpringR;
	delete m_pLine;
	delete m_pNode;
	delete m_pArrow;
	delete m_pPrecentText;
	delete m_Special;
	delete m_pSpecialSp;
}

void CBombShooter::init()
{
	m_fDestoryPre = 0;
	m_fShootSpeed = 0;
	m_vecShootDir = { 0,0 };

	SIZE * winSize = (SIZE *)CPlatform::GetInstance()->GetPlatData((_T("winsize")));
	vector<TSTRING> names;
	float scalling;


	m_pMainUI = new CSprite;
	m_pMainUI->Init(_T("construction"));
	scalling = (float)winSize->cx / (float)m_pMainUI->GetShowWidth();
	m_pMainUI->SetXScale(scalling);
	m_pMainUI->SetYScale(scalling);
	m_pMainUI->SetCenter({ 0,0,0 });
	m_pMainUI->SetDesPos({ 0,(float)winSize->cy - (float)m_pMainUI->getDrawSize().cy });
	

	names.clear();
	names.push_back(_T("bar"));
	names.push_back(_T("bar_h"));
	m_pPrecentBar = new CSprite;
	m_pPrecentBar->Init(names);
	m_pPrecentBar->SetCurAnimation(_T("bar"));
	m_pPrecentBar->SetXScale(scalling);
	m_pPrecentBar->SetYScale(scalling);
	m_pPrecentBar->SetDesPos({ (float)winSize->cx - 87,(float)winSize->cy - 49});
	m_pPrecentBar->SetCenter(CSprite::ECEN_LEFT|CSprite::ECEN_BOTTOM);

	m_pLine = new CSprite;
	m_pLine->Init(_T("band"));
	m_pLine->AjustScale({ winSize->cx, 8 });
	m_pLine->SetCenter({ 0,0,0 });
	m_pLine->SetDesPos({ 0,m_pMainUI->GetDesPos().y - 5 });

	m_pSpringL = new CSprite;
	m_pSpringL->Init(_T("prujiny2"));
	m_pSpringL->SetCenter(CSprite::ECEN_LEFT | CSprite::ECEN_MIDDLE);
	m_pSpringL->SetXScale(scalling);
	m_pSpringL->SetYScale(scalling - 0.3);
	m_pSpringL->SetDesPos({ (float)winSize->cx / 2 - 110,m_pMainUI->GetDesPos().y});

	m_pSpringR = new CSprite;
	m_pSpringR->Init(_T("prujiny2"));
	m_pSpringR->SetCenter(CSprite::ECEN_LEFT | CSprite::ECEN_MIDDLE);
	m_pSpringR->SetXScale(-scalling);
	m_pSpringR->SetYScale(scalling - 0.3);
	m_pSpringR->SetDesPos({ (float)winSize->cx / 2 + 110,m_pMainUI->GetDesPos().y});

	m_pNode = new CSprite;
	m_pNode->Init(_T("kreplenie"));
	m_pNode->SetCenter(CSprite::ECEN_CENTER | CSprite::ECEN_MIDDLE);
	m_pNode->SetXScale(scalling - 0.3);
	m_pNode->SetYScale(scalling - 0.3);
	
	m_pHolder = new CSprite;
	m_pHolder->Init(_T("zahvat2"));
	m_pHolder->SetCenter(CSprite::ECEN_CENTER | CSprite::ECEN_MIDDLE);
	m_pHolder->SetXScale(scalling - 0.3);
	m_pHolder->SetYScale(scalling - 0.3);

	m_pArrow = new CSprite;
	m_pArrow->Init(_T("arrow"));
	m_pArrow->SetCenter(CSprite::ECEN_CENTER | CSprite::ECEN_MIDDLE);
	m_pArrow->SetXScale(1);
	m_pArrow->SetYScale(scalling - 0.3);

	names.clear();
	names.push_back(_T("Binner_Pre"));
	names.push_back(_T("Binner_0"));
	names.push_back(_T("Binner_1"));
	names.push_back(_T("Binner_2"));
	names.push_back(_T("Binner_3"));
	names.push_back(_T("Binner_4"));
	names.push_back(_T("Binner_5"));
	names.push_back(_T("Binner_6"));
	names.push_back(_T("Binner_7"));
	names.push_back(_T("Binner_8"));
	names.push_back(_T("Binner_9"));
	m_pPrecentText = new CSprite;
	m_pPrecentText->Init(names);
	m_pPrecentText->SetXScale(0.5);
	m_pPrecentText->SetYScale(0.5);
	m_pPrecentText->SetCenter(CSprite::ECEN_CENTER | CSprite::ECEN_MIDDLE);
	m_pPrecentText->SetDesPos({ m_pPrecentBar->GetDesPos().x + 65,m_pPrecentBar->GetDesPos().y - 13 });

	m_bSelect = false;
	m_fMinRound = 30;	//最小半径
	m_fMaxRound = 150;	//最大半径
	m_vecCenterPos = m_pMainUI->GetDesPos();
	m_vecCenterPos.x = winSize->cx / 2;
	m_vecNorPos = m_vecCenterPos;
	m_vecNorPos.y += 55;
	m_pArrow->SetDesPos({ m_vecCenterPos.x,m_vecCenterPos.y });
	m_pHolder->SetDesPos(m_vecNorPos);
	m_pHolder->UpdateMatrix();

	m_pSpecialSp = new CSprite;
	names.clear();
	names.push_back(_T("new_great"));
	names.push_back(_T("new_outstanding"));
	names.push_back(_T("amazing"));
	names.push_back(_T("superb_perfomance"));
	m_pSpecialSp->Init(names); 
	m_pSpecialSp->SetDesPos({ 240,300 });
	m_animBlend = CAnimManager::GetInstance()->GetAnimHander(_T("blendIn"));
	m_Special = CAnimManager::GetInstance()->GetAnimHander(_T("special"))->Copy();
	m_Special->SetMainTarget(m_pSpecialSp);
	m_Special->EndDraw();
	CLevelManager::GetInstance()->SetShooter(this);
}

void CBombShooter::reset()
{
	m_fDestoryPre = 0;
	m_fShootSpeed = 0;
	m_vecShootDir = { 0,0 };
	m_preAnimCount = 0;
}

void CBombShooter::OnMouseDown(SEvent event)
{
	if (m_eState != NORMAL || !m_curBomb)
		return;
	D3DXVECTOR2 pos;
	pos.x = event.mouseInput.x;
	pos.y = event.mouseInput.y;
	pos -= m_pHolder->GetDesPos();	//计算向量
	float len = D3DXVec2Length(&pos);	//计算长度
	if(len <= 15)
	{
		CAudio4Bass::GetInstance()->ChannelPlay(_T("ready"), true);
		m_bSelect = true;
	}

}

void CBombShooter::OnMouseUp(SEvent event)
{
	if (m_eState != NORMAL || !m_curBomb)
		return;
	if (!m_bSelect) return;
	CAudio4Bass::GetInstance()->ChannelPlay(_T("shoot"), true);
	D3DXVECTOR2 shootDir = m_vecCenterPos - m_pHolder->GetDesPos();
	m_curBomb->FreeBomb(shootDir, D3DXVec2Length(&shootDir) * 0.1);
	m_stateCount = 0;
	m_stateParam = 1;
	m_pHolder->SetDesPos(m_vecNorPos);
	m_eState = SHOOTED;
	m_bSelect = false;
	CInputManager::GetInstance()->AddMouseListener(_T("当前炸弹"), m_curBomb);
}

void CBombShooter::OnMouseMove(SEvent event)
{
	if (m_eState != NORMAL || !m_curBomb)
		return;
	if(m_bSelect)
	{
		D3DXVECTOR2 pos;	//鼠标的位置
		D3DXVECTOR2 round;	//鼠标位置和中心点的向量
		pos.x = event.mouseInput.x;
		pos.y = event.mouseInput.y;
		round = pos - m_vecCenterPos;
		if (round.y <= 7)	//如果Y轴过小，取消这次发射
		{
			m_eState = CBombShooter::RESET;
			m_bSelect = false;
			m_stateCount = 0;
			return;
		}
		if (D3DXVec2Length(&round) <= m_fMinRound)	//如果过小，取消这次发射
		{
			m_eState = CBombShooter::RESET;
			m_bSelect = false;
			m_stateCount = 0;
			return;
		}
		if (D3DXVec2Length(&round) >= m_fMaxRound)	//如果过大，校准这次发射
		{
			D3DXVec2Normalize(&round, &round);
			pos = round * m_fMaxRound + m_vecCenterPos;
		}
		if (m_curBomb)
		{
			m_curBomb->SetDesPos(pos);
		}
		m_pHolder->SetDesPos(pos);
	}
}

void CBombShooter::Draw()
{
	m_pPrecentBar->Draw();
	m_pMainUI->Draw();
	m_pLine->Draw();
	m_pNode->SetDesPos(m_pSpringL->GetDesPos());
	m_pNode->Draw();
	m_pNode->SetDesPos(m_pSpringR->GetDesPos());
	m_pNode->Draw();
	m_pSpringL->Draw();
	m_pSpringR->Draw();
	m_pHolder->Draw();
	if(m_bSelect || m_eState == CBombShooter::RESET)
	{
		m_pArrow->Draw();
	}

	TCHAR textNum[64];
	D3DXVECTOR2 pos = m_pPrecentText->GetDesPos();
	m_pPrecentText->SetCurAnimation(_T("Binner_Pre"));
	m_pPrecentText->Draw();
	pos.x -= 17;
	m_pPrecentText->SetDesPos(pos);
	_stprintf(textNum, _T("Binner_%d"), (int)m_fDestoryPre % 10);
	m_pPrecentText->SetCurAnimation(textNum);
	m_pPrecentText->Draw();
	pos.x -= 10;
	m_pPrecentText->SetDesPos(pos);
	_stprintf(textNum, _T("Binner_%d"), (int)m_fDestoryPre % 100 / 10);
	m_pPrecentText->SetCurAnimation(textNum);
	m_pPrecentText->Draw();
	if(m_fDestoryPre >= 100.0)
	{
		pos.x -= 10;
		m_pPrecentText->SetDesPos(pos);
		_stprintf(textNum, _T("Binner_%d"), (int)m_fDestoryPre % 1000 / 100);
		m_pPrecentText->SetCurAnimation(textNum);
		m_pPrecentText->Draw();
		pos.x += 10;
	}
	pos.x += 27;
	m_pPrecentText->SetDesPos(pos);

	for (auto it : m_listBomb)
	{
		it->Draw();
	}
	if (m_curBomb)
		m_curBomb->Draw();

	if (!m_Special->IsEnd())
		m_Special->Render();
}

void CBombShooter::Update()
{
	m_Special->Update();
	if (m_preAnimCount)
	{
		m_preAnimCount--;
		m_fDestoryPre++;
	}
	int HolderWidth = m_pHolder->GetShowWidth();	//Holder的大小
	D3DXVECTOR2 springDir = { 0,0 };	//弹簧的向量
	float angle;	//角度

	//计算进度条的绘制
	RECT drawRect;
	drawRect.bottom = m_pPrecentBar->GetShowHeight();
	drawRect.right = m_pPrecentBar->GetShowWidth();
	drawRect.left = 0;
	drawRect.top = drawRect.bottom - drawRect.bottom * m_fDestoryPre * 0.01;
	m_pPrecentBar->SetSrcRect(drawRect);
	m_pPrecentBar->SetCenter(CSprite::ECEN_LEFT | CSprite::ECEN_BOTTOM);

	//计算Holder的旋转
	springDir = m_pHolder->GetDesPos() - m_vecCenterPos;	//计算出从中点指向holder的向量
	angle = atan2f(springDir.y, springDir.x) - D3DX_PI * 0.5;	//计算出偏转角
	m_pHolder->SetAngle(angle);

	//计算arrow的缩放
	if(m_bSelect || m_eState == CBombShooter::RESET || m_eState == CBombShooter::WAIT)
	{
		springDir = m_vecCenterPos;
		springDir = m_pHolder->GetDesPos() - springDir;	//计算出箭头中心点到发射器的指向
		float len = D3DXVec2Length(&springDir) * 1.4;		//计算出y轴长度
		len /= m_pArrow->GetShowHeight();	//计算缩放量
		m_pArrow->SetYScale(len);
		m_pArrow->SetAngle(angle);
	}

	//计算出左边弹簧的变换数据
	springDir = { 0,0 };
	springDir.x -= HolderWidth / 2;	//计算结束点
	D3DXVec2TransformCoord(&springDir, &springDir, &m_pHolder->GetMatrix());	//应用变换计算真实的结束点
	springDir -= m_pSpringL->GetDesPos();	//计算出弹簧的向量
	m_pSpringL->AjustScale({ (int)D3DXVec2Length(&springDir),m_pSpringL->getDrawSize().cy });	//适配长度
	angle = atan2f(springDir.y, springDir.x);
	m_pSpringL->SetAngle(angle);

	//计算出右边弹簧的变换数据
	springDir = { 0,0 };
	springDir.x += HolderWidth / 2;	//计算结束点
	D3DXVec2TransformCoord(&springDir, &springDir, &m_pHolder->GetMatrix());	//应用变换计算真实的结束点
	springDir -= m_pSpringR->GetDesPos();	//计算出弹簧的向量
	m_pSpringR->AjustScale({ (int)D3DXVec2Length(&springDir),m_pSpringR->getDrawSize().cy });	//适配长度
	m_pSpringR->SetYScale(-m_pSpringR->GetYScale());
	angle = atan2f(springDir.y, springDir.x);
	m_pSpringR->SetAngle(angle);

	//更新状态
	this->UpdateState();

	if (m_curBomb)
		m_curBomb->Update();
	for (auto it : m_listBomb)
	{
		it->Update();
	}
}

void CBombShooter::SetDestroyPre(float precent, bool anim)
{
	if (anim)
	{
		m_preAnimCount = precent - m_fDestoryPre;
		if(precent >= 100)
		{
			CAudio4Bass::GetInstance()->ChannelPlay(_T("performance"), true);
			m_pSpecialSp->SetCurAnimation(_T("superb_perfomance"));
			m_pSpecialSp->SetCenter(CSprite::ECEN_MIDDLE | CSprite::ECEN_CENTER);
			m_Special->Reset();
		}
		else if(m_preAnimCount >= 70)
		{
			CAudio4Bass::GetInstance()->ChannelPlay(_T("amazing"), true);
			m_pSpecialSp->SetCurAnimation(_T("amazing"));
			m_pSpecialSp->SetCenter(CSprite::ECEN_MIDDLE | CSprite::ECEN_CENTER);
			m_Special->Reset();
		}
		else if(m_preAnimCount >= 50)
		{
			CAudio4Bass::GetInstance()->ChannelPlay(_T("outstading"), true);
			m_pSpecialSp->SetCurAnimation(_T("new_outstanding"));
			m_pSpecialSp->SetCenter(CSprite::ECEN_MIDDLE | CSprite::ECEN_CENTER);
			m_Special->Reset();
		}
		else if(m_preAnimCount >= 30)
		{
			CAudio4Bass::GetInstance()->ChannelPlay(_T("great"), true);
			m_pSpecialSp->SetCurAnimation(_T("new_great"));
			m_pSpecialSp->SetCenter(CSprite::ECEN_MIDDLE | CSprite::ECEN_CENTER);
			m_Special->Reset();
		}
		return;
	}
	m_fDestoryPre = precent; 
	if (m_fDestoryPre > 100)
		m_fDestoryPre = 100;	//设置破坏进度
}

void CBombShooter::AddBomb(CBaseBomb * bomb)
{
	bomb->SetXScale(0.4);
	bomb->SetYScale(0.4);
	if (!m_curBomb)
		m_curBomb = bomb;
	else
	{
		m_listBomb.push_back(bomb);
	}
	if (m_curBomb->GetBombType() == CBaseBomb::ROCKET)
		m_fMaxRound = 60;
	else
		m_fMaxRound = 150;
}

void CBombShooter::AddBomb(vector<CBaseBomb*>* vec)
{
	CAudio4Bass::GetInstance()->ChannelPlay(_T("addBomb"), true);
	for (auto it : *vec)
	{
		m_InsertBomb.push_back(it);
	}
}

void CBombShooter::ClearBomb()
{
	if (m_curBomb)
	{
		delete m_curBomb;
		m_curBomb = nullptr;
	}
	for (auto it : m_listBomb)
	{
		delete it;
	}
	m_eState = NORMAL;
	m_listBomb.clear();
	m_Special->EndDraw();
	m_preAnimCount = 0;
}

void CBombShooter::UpdateBombStack()
{
	auto it = m_listBomb.begin();

	if (it == m_listBomb.end())return;
	//1
	(*it)->SetDesPos(s_BombStackPos[0]);
	(*it)->SetXScale(s_fBigScalling);
	(*it)->SetYScale(s_fBigScalling);
	it++;
	if (it == m_listBomb.end())return;

	//2
	(*it)->SetDesPos(s_BombStackPos[1]);
	(*it)->SetXScale(s_fSmallScalling);
	(*it)->SetYScale(s_fSmallScalling);
	it++;
	if (it == m_listBomb.end())return;

	//3
	(*it)->SetDesPos(s_BombStackPos[2]);
	(*it)->SetXScale(s_fSmallScalling);
	(*it)->SetYScale(s_fSmallScalling);
	it++;
	if (it == m_listBomb.end())return;

	//4
	(*it)->SetDesPos(s_BombStackPos[3]);
	(*it)->SetXScale(s_fSmallScalling);
	(*it)->SetYScale(s_fSmallScalling);
	it++;
	if (it == m_listBomb.end())return;

	//5
	(*it)->SetDesPos(s_BombStackPos[4]);
	(*it)->SetXScale(s_fSmallScalling);
	(*it)->SetYScale(s_fSmallScalling);
	it++;
	if (it == m_listBomb.end())return;

	//other
	for (; it != m_listBomb.end(); it++)
	{
		(*it)->SetDesPos({ -20,s_BombStackPos[4].y });
		(*it)->SetXScale(s_fSmallScalling);
		(*it)->SetYScale(s_fSmallScalling);
	}
}

void CBombShooter::ExchangeBomb()
{
	if (m_eState != NORMAL) return;		//不是普通状态，返回
	if (m_listBomb.empty()) return;		//弹夹无炸弹，返回
	m_eState = EXCHANGE;
	m_stateCount = m_stateParam = 0;
}

void CBombShooter::UpdateState()
{
	switch (m_eState)
	{
	case CBombShooter::NORMAL:
	{
		if (m_curBomb)
		{
			m_curBomb->SetDesPos(m_pHolder->GetDesPos());
			m_curBomb->SetAngle(m_pHolder->GetAngle());

		}
	}
		break;
	case CBombShooter::RESET:
	{
		D3DXVECTOR2 resetDir;	//重设方向
		resetDir = m_vecNorPos - m_pHolder->GetDesPos();		//计算指向向量
		if (D3DXVec2Length(&resetDir) <= 12)
		{
			m_pHolder->SetDesPos(m_vecNorPos);
			m_curBomb->SetDesPos(m_vecNorPos);
			m_stateCount++;
			m_pArrow->SetYScale(0.45 - m_stateCount * 0.03);
			if(m_stateCount >= 15)
			{
				m_eState = NORMAL;
			}
		}
		else
		{
			D3DXVec2Normalize(&resetDir, &resetDir);
			m_pHolder->SetDesPos(m_pHolder->GetDesPos() + resetDir * 12);
			if (m_curBomb)
			{
				m_curBomb->SetDesPos(m_pHolder->GetDesPos());
			}
		}
	}
		break;
	case CBombShooter::SHOOTED:
	{
		if(m_stateCount >= 40)
		{
			m_stateCount = 0;
			m_eState = WAIT;
			m_pHolder->SetDesPos(m_vecNorPos);
		}
		else
		{
			m_stateCount++;
			if(!(m_stateCount % 3))
			{
				D3DXVECTOR2 pos = m_pHolder->GetDesPos();
				pos.y = m_vecNorPos.y + m_stateParam * (40 - m_stateCount) * 0.4;
				m_stateParam = -m_stateParam;
				m_pHolder->SetDesPos(pos);
			}
		}
	}
		break;
	case CBombShooter::WAIT:
	{
		if (m_curBomb->GetState() == CBaseBomb::DESTORY && CLevelManager::GetInstance()->AnimIsEnd() && m_Special->IsEnd())
		{
			m_stateCount = m_stateParam = 0;
			delete m_curBomb;
			m_curBomb = nullptr;
			CInputManager::GetInstance()->DeleteMouseListener(_T("当前炸弹"));
			//根据炸弹数选择下一个状态
			if (m_listBomb.empty() && m_InsertBomb.empty())
			{
				m_eState = NORMAL;
			}
			else
			{
				if (m_InsertBomb.empty())
					m_eState = FILL;
				else
				{
					m_stateParam = m_InsertBomb.size();
					m_eState = ADD;
					return;
				}
			}
			//设置装填参数
			if (m_listBomb.size() <= 5) 
				m_stateParam = 0;
			else
			{
				auto it = m_listBomb.begin();
				//定位到加入的炸弹位置
				for (int i = 0; i < 5; i++)
					it++;
				m_animBlend->SetMainTarget(*it);
				m_animBlend->Reset();
				m_stateParam = 1;
			}
		}
	}
		break;
	case CBombShooter::FILL:
	{
		m_stateCount++;
		auto it = m_listBomb.begin();
		D3DXVECTOR2 tempVec, dirVec;

		//动画播放
		if (m_stateParam)
			m_animBlend->Update();

		//第一个炸弹更新
		tempVec = (*it)->GetDesPos();
		dirVec = m_vecNorPos - tempVec;
		float step = D3DXVec2Length(&dirVec) / (17.0 - m_stateCount + 1);
		D3DXVec2Normalize(&dirVec, &dirVec);
		tempVec += dirVec * step;
		(*it)->SetDesPos(tempVec);
		it++;
		for (int i = 0; i < 5; i++)
		{
			//检空
			if(it == m_listBomb.end())
				break;
			//底部目标缩放
			if (i == 0)
			{
				float scalling = (s_fBigScalling - s_fSmallScalling) / 17.0 + (*it)->GetXScale();
				(*it)->SetXScale(scalling);
				(*it)->SetYScale(scalling);
			}
			//位移操作
			tempVec = (*it)->GetDesPos();
			dirVec = s_BombStackPos[i] - tempVec;
			step = D3DXVec2Length(&dirVec) / (17.0 - m_stateCount + 1);
			D3DXVec2Normalize(&dirVec, &dirVec);
			tempVec += dirVec * step;
			(*it)->SetDesPos(tempVec);
			it++;
		}

		//状态切换
		if (m_stateCount >= 17)
		{
			m_stateCount = m_stateParam = 0;
			m_eState = NORMAL;
			m_curBomb = m_listBomb.front();
			m_listBomb.pop_front();
			m_iCurBont++;	//增加回合数
			if (m_curBomb->GetBombType() == CBaseBomb::ROCKET)
				m_fMaxRound = 60;
			else
				m_fMaxRound = 150;
		}
	}
		break;
	case CBombShooter::ADD:
	{
		m_stateCount++;
		auto it = m_listBomb.begin();
		D3DXVECTOR2 tempVec, dirVec;
		float step;

		for (int i = 0; i < 5; i++)
		{
			//检空
			if (it == m_listBomb.end())
				break;
			if (i == 4)
			{
				(*it)->SetDesPos({ -20,s_BombStackPos[4].y });
				break;
			}
			//底部目标缩放
			if (i == 0)
			{
				float scalling = (s_fSmallScalling - s_fBigScalling) / 10.0 + (*it)->GetXScale();
				(*it)->SetXScale(scalling);
				(*it)->SetYScale(scalling);
			}
			//位移操作
			tempVec = (*it)->GetDesPos();
			dirVec = s_BombStackPos[i + 1] - tempVec;
			step = D3DXVec2Length(&dirVec) / (10.0 - m_stateCount + 1);
			D3DXVec2Normalize(&dirVec, &dirVec);
			tempVec += dirVec * step;
			(*it)->SetDesPos(tempVec);
			it++;
		}
		if (!m_listBomb.empty() && m_listBomb.front()->GetTransparent() < 255)
		{
			m_listBomb.front()->SetTransparent(m_listBomb.front()->GetTransparent() + 23);
		}
		//状态切换
		if (m_stateCount >= 10 && m_stateParam)
		{
			m_listBomb.push_front(m_InsertBomb.back());
			m_InsertBomb.back()->SetTransparent(0);
			m_InsertBomb.back()->SetDesPos(s_BombStackPos[0]);
			this->UpdateBombStack();
			m_InsertBomb.pop_back();
			m_stateCount = 0;
			m_stateParam--;
		}
		else if (!m_stateParam && m_listBomb.front()->GetTransparent() == 255)
		{
			this->UpdateBombStack();
			m_eState = FILL;
			m_stateCount = m_stateParam = 0;
		}
	}
		break;
	case CBombShooter::EXCHANGE:
	{
		m_stateCount++;

		//互换炸弹
		D3DXVECTOR2 vec, pos;
		float speed;
		vec = m_curBomb->GetDesPos() - s_BombStackPos[0];
		speed = D3DXVec2Length(&vec) / (18.0 - m_stateCount);
		D3DXVec2Normalize(&vec, &vec);

		pos = m_listBomb.front()->GetDesPos();
		pos += speed * vec;
		m_listBomb.front()->SetDesPos(pos);

		pos = m_curBomb->GetDesPos();
		pos += speed * -vec;
		m_curBomb->SetDesPos(pos);

		//状态切换
		if (m_stateCount >= 17)
		{
			CBaseBomb * temp = m_listBomb.front();
			m_stateCount = m_stateParam = 0;
			m_eState = NORMAL;
			m_listBomb.pop_front();
			m_listBomb.push_front(m_curBomb);
			m_curBomb = temp;
			if (m_curBomb->GetBombType() == CBaseBomb::ROCKET)
				m_fMaxRound = 60;
			else
				m_fMaxRound = 150;
		}
	}
		break;
	}

}
