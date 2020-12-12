#include "stdafx.h"
#include "BombShooter.h"
#include "LevelManager.h"
#include "RotateBomb.h"
#include "StaticBomb.h"
#include "TripleBomb.h"
#include "NormalBomb.h"
#include "RocketBomb.h"
#include "NewBombs.h"

CLevelManager::CLevelManager()
{
	SIZE * winSize = (SIZE *)CPlatform::GetInstance()->GetPlatData(_T("winsize"));
	m_pDevice = (LPDIRECT3DDEVICE9)CPlatform::GetInstance()->GetPlatData(_T("pDev"));
	D3DXCreateSprite(m_pDevice, &m_pSprite);
	m_bSendPre = FALSE;
	TCHAR tStr[128];
	vector<TSTRING> names;
	for (int i = 0; i < 45; i++)
	{
		_stprintf(tStr, _T("bg_%02d"), i + 1);
		names.push_back(tStr);
	}
	m_pBackground = new CSprite;
	m_pBackground->Init(names);
	m_pBackground->AjustScale(*winSize);
	m_pBackground->SetDesPos({ 0,0 });
	
	m_pBomb = CResManager::GetInstance()->GetTexture(_T("mask_new_384"));
	m_pSculpture = new CTexture;
	m_pMask = new CTexture;
	m_xScalling = 1;
	m_yScalling = 1;
	m_vecSculPos = { 0,0 };
	m_xBombScal = 1;
	m_yBombScal = 1;
}


CLevelManager::~CLevelManager()
{
	m_pSprite->Release();
	UnloadeNowLevel();
	delete m_pBackground;
	delete m_pSculpture;
	delete m_pMask;
	for (int i = 0; i < m_iLevelNum; i++)
	{
		delete m_LevelsInfo[i];
	}
}

void CLevelManager::LoadLevelInfoFromFile(TSTRING DrawInfo, TSTRING DataInfo, TSTRING PlayInfo)
{
	m_strSaveFile = DataInfo;
	TCHAR	tStr[256], tStrB[256];
	char	aStr[256];
	int		intVar;
	TiXmlDocument * drawFile, * dataFile, * levelFile;	//三个数据文件
	TiXmlElement * root, *element, *dataRoot, *dataElement, *levelRoot, *levelElement,*levelRead;
	drawFile = new TiXmlDocument;
	dataFile = new TiXmlDocument;
	levelFile = new TiXmlDocument;


	//初始化关卡信息
	m_iLevelNum = 120;
	for (int i = 0; i < m_iLevelNum; i++)
	{
		m_LevelsInfo.push_back(new SLevelInfo);
	}

	//开始读取文件
	Auto_To_Anis(DrawInfo.c_str(), DrawInfo.length() + 1, aStr, 256);
	drawFile->LoadFile(aStr);
	root = drawFile->FirstChildElement();
	element = root->FirstChildElement();

	Auto_To_Anis(DataInfo.c_str(), DataInfo.length() + 1, aStr, 256);
	dataFile->LoadFile(aStr);
	dataRoot = dataFile->FirstChildElement();
	dataElement = dataRoot->FirstChildElement();

	Auto_To_Anis(PlayInfo.c_str(), DataInfo.length() + 1, aStr, 256);
	levelFile->LoadFile(aStr);
	levelRoot = levelFile->FirstChildElement();
	levelElement = levelRoot->FirstChildElement();

	for (int i = 0; i < m_iLevelNum; i++)
	{
		//读取背景
		intVar = atoi(element->FirstChild()->Value());
		_stprintf(tStr, _T("bg_%02d"),intVar);
		m_LevelsInfo[i]->BKG = tStr;
		element = element->NextSiblingElement();

		//读取冰雕
		Anis_To_Auto(element->FirstChild()->Value(), 256, tStr, 256);
		m_LevelsInfo[i]->Name = tStr;
		_stprintf(tStrB, _T("./picture/levels/JPG/%s.jpg"), tStr);
		m_LevelsInfo[i]->Path = tStrB;
		_stprintf(tStrB, _T("./picture/levels/PNG/%s_.png"), tStr);
		m_LevelsInfo[i]->MaskPath = tStrB;
		element = element->NextSiblingElement();

		//读取得分
		dataElement->Attribute("open", (int *)&m_LevelsInfo[i]->Open);
		dataElement->Attribute("score", &m_LevelsInfo[i]->Score);
		dataElement = dataElement->NextSiblingElement();

		//读取关卡信息
		levelElement->Attribute("xAdjust", &m_LevelsInfo[i]->XAdjust);
		/*读取炸弹列表*/
		levelRead = levelElement->FirstChildElement("bomb");
		while (levelRead)
		{
			levelRead->Attribute("type", &intVar);
			m_LevelsInfo[i]->Bombs.push_back(intVar);
			levelRead = levelRead->NextSiblingElement("bomb");
		}

		/*读取游戏过程中出现的炸弹*/
		levelRead = levelElement->FirstChildElement("newBomb");
		while(levelRead)
		{
			m_LevelsInfo[i]->NewBombs.push_back({ {0,0},0,0 });
			levelRead->Attribute("posX", &intVar);
			m_LevelsInfo[i]->NewBombs.back().Pos.x = intVar;
			levelRead->Attribute("posY", &intVar);
			m_LevelsInfo[i]->NewBombs.back().Pos.y = intVar;
			levelRead->Attribute("type", &m_LevelsInfo[i]->NewBombs.back().Type);
			levelRead->Attribute("time", &m_LevelsInfo[i]->NewBombs.back().ShowTime);
			levelRead->Attribute("life", &m_LevelsInfo[i]->NewBombs.back().Life);


			levelRead = levelRead->NextSiblingElement("newBomb");
		}

		/*读取初始出现在地图上的炸弹*/
		levelRead = levelElement->FirstChildElement("iniBomb");
		while(levelRead)
		{
			m_LevelsInfo[i]->IniBombs.push_back({ 0,0 });
			levelRead->Attribute("posX", &intVar);
			m_LevelsInfo[i]->IniBombs.back().x = intVar;
			levelRead->Attribute("posY", &intVar);
			m_LevelsInfo[i]->IniBombs.back().y = intVar;
			levelRead = levelRead->NextSiblingElement("iniBomb");
		}

		levelElement = levelElement->NextSiblingElement();
	}

	delete drawFile;
	delete dataFile;
	delete levelFile;
}

void CLevelManager::SaveLevelsInfo()
{
	char  aStr[256];
	Auto_To_Anis(m_strSaveFile.c_str(), 256, aStr, 256);
	TiXmlDocument * doc = new TiXmlDocument(aStr);
	TiXmlElement * element = new TiXmlElement("Root");
	doc->InsertEndChild(*element);
	delete element;
	element = new TiXmlElement("Level");
	for (int i = 0; i < 120; i++)
	{
		element->SetAttribute("open", (int)m_LevelsInfo[i]->Open);
		element->SetAttribute("score", m_LevelsInfo[i]->Score);
		doc->FirstChildElement()->InsertEndChild(*element);
	}
	delete element;
	doc->SaveFile();
	delete doc;
}

bool CLevelManager::AnimIsEnd()
{
	for(auto it:m_listBomb)
	{
		if(it->GetState() != CBaseBomb::DESTORY && it->GetState()!= CBaseBomb::PREPAREING)
		{
			return false;
		}
	}
	return true;
}

void CLevelManager::LoadLevel(int level)
{
	if (level > 120) level = 120;
	m_iCurLevel = level;
	//加载图片
	m_pBackground->SetCurAnimation(m_LevelsInfo[level - 1]->BKG);
	m_pSculpture->SetPath(m_LevelsInfo[level - 1]->Path);
	m_pSculpture->SetColRef(RGB(1, 1, 1));
	m_pSculpture->Init();
	m_pMask->SetPath(m_LevelsInfo[level - 1]->MaskPath);
	m_pMask->SetColRef(RGB(1, 1, 1));
	m_pMask->Init();
	m_pShooter->SetBout(1);

	//确定缩放和位置
	SIZE * winSize = (SIZE *)CPlatform::GetInstance()->GetPlatData(_T("winsize"));
	m_xScalling = 0.52;
	m_yScalling = m_xScalling;
	m_vecSculPos.x = (winSize->cx - m_pSculpture->GetWidth() * m_xScalling) / 2;
	m_vecSculPos.y = (winSize->cy - 162 - (float)m_pSculpture->GetHeight() * m_yScalling) / 2 + m_LevelsInfo[level - 1]->XAdjust;
	m_drawRect.left = (INT)m_vecSculPos.x;
	m_drawRect.top = (INT)m_vecSculPos.y;
	m_drawRect.right = (INT)winSize->cx - 40;
	m_drawRect.bottom = (INT)m_pSculpture->GetHeight() * m_yScalling + m_drawRect.top;

	//读取爆破的面积数据
	m_iMaxPicCount = m_iCurPixCount = 0;
	D3DLOCKED_RECT pValue, pMaskVal;
	LPDIRECT3DTEXTURE9 texture = m_pMask->GetTexture();
	texture->LockRect(0, &pValue, nullptr, 0);
	m_pSculpture->GetTexture()->LockRect(0, &pMaskVal, nullptr, 0);
	BYTE *pColor = (BYTE*)pValue.pBits;
	BYTE *pMaskColor = (BYTE *)pMaskVal.pBits + 3;
	//获取关卡需要爆破的总像素数，并抠图
	for (int i = 0; i < m_pMask->m_Height; i++)
	{
		for (int j = 0; j < m_pMask->m_Width; j++)
		{
			if (*(pColor + i * pValue.Pitch + j * 4))
				m_iMaxPicCount++;
			else
				*(pMaskColor + i * pValue.Pitch + j * 4) = 0;
		}
	}
	m_pSculpture->GetTexture()->UnlockRect(0);
	texture->UnlockRect(0);

	//向发射器添加炸弹
	for (auto it : m_LevelsInfo[level - 1]->Bombs)
	{
		switch (it)
		{
		case 1:	//普通炸弹
			m_pShooter->AddBomb(new CNormalBomb);
			break;
		case 2:	//子母弹
			m_pShooter->AddBomb(new CTripleBomb);
			break;

		case 4:	//旋转弹
			m_pShooter->AddBomb(new CRotateBomb);
			break;
		case 5:	//火箭弹
			m_pShooter->AddBomb(new CRocketBomb);
			break;
		}
	}
	m_pShooter->UpdateBombStack();

	//加载固有炸弹信息
	for(auto it: m_LevelsInfo[level - 1]->IniBombs)
	{
		D3DXVECTOR2 pos = { (float)it.x,(float)it.y };
		D3DXVECTOR2 dir;
		//初始化炸弹
		CStaticBomb * temp = new CStaticBomb;
		temp->SetDesPos(pos);

		//计算范围添加链接线
		for(auto it:m_listBomb)
		{
			dir = it->GetDesPos();
			dir -= pos;
			if(D3DXVec2Length(&dir) <= 120)
			{
				it->AddLine(pos);
			}
		}

		//在冰雕上绘制裂纹
		this->DrawInSculpture(CResManager::GetInstance()->GetTexture(_T("Cristal_crack1")), pos, 1, 1);

		//添加炸弹
		m_listBomb.push_front(temp);
	}

	//加载在回合中出现的炸弹的信息
	for (auto it : m_LevelsInfo[level - 1]->NewBombs)
	{
		CNewBombs * temp = new CNewBombs;
		temp->SetType(it.Type);
		temp->SetShowBout(it.ShowTime);
		temp->SetDisappearBout(it.ShowTime + it.Life);
		temp->SetDesPos({ (float)it.Pos.x,(float)it.Pos.y });
		switch (it.Type)
		{
		case 1:
			temp->SetBombNum(2);
			break;
		case 2:
			temp->SetBombNum(2);
			break;
		case 3:
			temp->SetBombNum(1);
			break;
		case 4:
			temp->SetBombNum(3);
			break;
		case 5:
			temp->SetBombNum(2);
			break;
		}
		m_listNewBombs.push_back(temp);
	}
}

void CLevelManager::UnloadeNowLevel()
{
	m_iCurPixCount = 0;
	m_iMaxPicCount = 0;
	m_pShooter->ClearBomb();
	m_pShooter->SetDestroyPre(0, false);
	m_pMask->Release();
	m_pSculpture->Release();

	for(auto it: m_listBomb)
		delete it;
	m_listBomb.clear();
	for (auto it : m_listNewBombs)
		delete it;
	m_listNewBombs.clear();
}

void CLevelManager::SendBombInfo(D3DXVECTOR2 pos)
{
	RECT bombRc, sculRc;
	this->ComputeRect(&bombRc, &sculRc, pos, m_pBomb, 1, 1);
	D3DLOCKED_RECT pValue, bombValue;	//存储锁定的结构
	int width = sculRc.right - sculRc.left;
	int height = sculRc.bottom - sculRc.top;
	m_pSculpture->GetTexture()->LockRect(0, &pValue, &sculRc, 0);
	m_pBomb->GetTexture()->LockRect(0, &bombValue, &bombRc, 0);
	BYTE * sculColor = (BYTE *)pValue.pBits + 3;
	BYTE * bombColor = (BYTE *)bombValue.pBits + 3;

	float bombToScul = m_xScalling / m_xBombScal;
	int blend;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (blend = bombColor[i * bombValue.Pitch + j * 4])
			{
				if (sculColor[i * pValue.Pitch + j * 4])
				{
					if (!bombColor[i * bombValue.Pitch + j * 4 - 3] &&
						!bombColor[i * bombValue.Pitch + j * 4 - 2] &&
						!bombColor[i * bombValue.Pitch + j * 4 - 1])	//纯黑
					{
						sculColor[i * pValue.Pitch + j * 4] = 0;
						m_iCurPixCount++;
					}
					else	//非纯黑，混合颜色
					{
						AlphaBlend(blend, sculColor + i * pValue.Pitch + j * 4 - 3, bombColor + i * bombValue.Pitch + j * 4 - 3);
						AlphaBlend(blend, sculColor + i * pValue.Pitch + j * 4 - 2, bombColor + i * bombValue.Pitch + j * 4 - 2);
						AlphaBlend(blend, sculColor + i * pValue.Pitch + j * 4 - 1, bombColor + i * bombValue.Pitch + j * 4 - 1);

						//sculColor[i * pValue.Pitch + j * 4] -= 20;
					}
				}
			}
		}
	}

	m_pSculpture->GetTexture()->UnlockRect(0);
	m_pBomb->GetTexture()->UnlockRect(0);
	//绘制裂纹
	TCHAR imgName[128];
	int num = rand() % 24;
	if(num < 12 && num != 6)
	{
		_stprintf(imgName, _T("treshina%d-hd"), num + 1);
		this->DrawInSculpture(CResManager::GetInstance()->GetTexture(imgName), { pos.x + rand() % 120 - 60 ,pos.y - (rand() % 30 + 100) }, 1, 1);
	}

	//向相应的炸弹传递爆炸信息
	for(auto it:m_listBomb)
	{
		D3DXVECTOR2 bombPos = it->GetDesPos();
		if(D3DXVec2Length(&(bombPos - pos)) <= 110)
		{
			it->Bomb();
		}
	}

	int curBont = m_pShooter->GetBout();
	for (auto it = m_listNewBombs.begin(); it != m_listNewBombs.end();)
	{
		if ((*it)->GetShowBout() <= curBont && (*it)->GetDisappearBout() > curBont)
		{
			D3DXVECTOR2 bombPos = (*it)->GetDesPos();
			vector<CBaseBomb *> bombs;
			if (D3DXVec2Length(&(bombPos - pos)) <= 110)
			{
				for (int i = 0; i < (*it)->GetBombNum(); i++)
				{
					switch ((*it)->GetType())
					{
					case 1:	//普通炸弹
						bombs.push_back(new CNormalBomb);
						break;
					case 2:	//子母弹
						bombs.push_back(new CTripleBomb);
						break;

					case 4:	//旋转弹
						bombs.push_back(new CRotateBomb);
						break;
					case 5:	//火箭弹
						bombs.push_back(new CRocketBomb);
						break;
					}
				}
				m_pShooter->AddBomb(&bombs);
				it = m_listNewBombs.erase(it);
			}
			else
				it++;
		}
		else
			it++;
	}

	m_bSendPre = TRUE;
	return;
}

void CLevelManager::ReloadLevel()
{
	UnloadeNowLevel();
	LoadLevel(m_iCurLevel);
}

bool CLevelManager::LevelIsOpen(int level)
{
	return m_LevelsInfo[level - 1]->Open;
}

int CLevelManager::LevelScore(int level)
{
	return m_LevelsInfo[level - 1]->Score;
}

void CLevelManager::Render()
{
	//绘制背景
	m_pBackground->Draw();

	//绘制冰雕
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	/*首先绘制一次阴影*/
	D3DXMatrixScaling(&m_mat, m_xScalling, m_yScalling, 0);
	D3DXMATRIX mat;
	D3DXMatrixTranslation(&mat, m_vecSculPos.x + 3, m_vecSculPos.y + 5, 0);
	m_mat *= mat;
	m_pSprite->SetTransform(&m_mat);
	m_pSprite->Draw(m_pSculpture->GetTexture(), nullptr, nullptr, nullptr, 0x80000000);

	/*然后绘制冰雕本体*/
	D3DXMatrixScaling(&m_mat, m_xScalling, m_yScalling, 0);
	D3DXMatrixTranslation(&mat, m_vecSculPos.x, m_vecSculPos.y, 0);
	m_mat *= mat;
	m_pSprite->SetTransform(&m_mat);
	m_pSprite->Draw(m_pSculpture->GetTexture(), nullptr, nullptr, nullptr, 0xFFFFFFFF);

	m_pSprite->End();

	//绘制固有的静态炸弹&链接线
	for (auto it : m_listBomb)
		it->DrawLine();
	for(auto it:m_listBomb)
		it->Draw();

	//绘制新出现的炸弹
	int curBont = m_pShooter->GetBout();
	for (auto it : m_listNewBombs)
	{
		if (it->GetShowBout() <= curBont && it->GetDisappearBout() >= curBont)
			it->Draw();
	}
}

void CLevelManager::Update()
{
	for (auto it : m_listBomb)
		it->Update();
	if (this->AnimIsEnd() && m_bSendPre)
	{
		if ((float)m_iCurPixCount * 100 / m_iMaxPicCount >= 99.0)
		{
			m_iMaxPicCount = m_iCurPixCount = 0;
			D3DLOCKED_RECT lockRc;
			m_pSculpture->GetTexture()->LockRect(0, &lockRc, nullptr, 0);
			BYTE* color = (BYTE*)lockRc.pBits;
			color += 3;
			for (int i = 0; i < m_pSculpture->GetHeight(); i++)
			{
				for (int j = 0; j < m_pSculpture->GetWidth(); j++)
				{
					m_iMaxPicCount++;
					if (!color[i * lockRc.Pitch + j * 4])
					{
						m_iCurPixCount++;
					}
				}
			}
			m_pSculpture->GetTexture()->UnlockRect(0);
		}
		m_pShooter->SetDestroyPre((float)m_iCurPixCount * 100 / m_iMaxPicCount + 0.02, true);
		m_bSendPre = FALSE;
	}
	int curBont = m_pShooter->GetBout();
	for (auto it : m_listNewBombs)
	{
		if (it->GetShowBout() <= curBont && it->GetDisappearBout() >= curBont)
		{
			it->SetCurBout(curBont);
			it->Update();
		}
	}
}

void CLevelManager::AddStaticBomb(CStaticBomb * bomb)
{
	D3DXVECTOR2 dir,pos;
	pos = bomb->GetDesPos();

	//计算范围添加链接线
	for (auto it : m_listBomb)
	{
		dir = it->GetDesPos();
		dir -= pos;
		if (D3DXVec2Length(&dir) <= 110)
		{
			it->AddLine(pos);
		}
	}

	//在冰雕上绘制裂纹
	this->DrawInSculpture(CResManager::GetInstance()->GetTexture(_T("Cristal_crack1")), pos, 1, 1);

	//添加炸弹
	m_listBomb.push_front(bomb);
}

void CLevelManager::DrawInSculpture(CTexture * texture, D3DXVECTOR2 pos, float xScalling, float yScalling)
{
	RECT sculRc, srcRc;
	this->ComputeRect(&srcRc, &sculRc, pos, texture, 1, 1);
	D3DLOCKED_RECT pValue, bombValue;	//存储锁定的结构
	int width = sculRc.right - sculRc.left;
	int height = sculRc.bottom - sculRc.top;
	m_pSculpture->GetTexture()->LockRect(0, &pValue, &sculRc, 0);
	texture->GetTexture()->LockRect(0, &bombValue, &srcRc, 0);
	BYTE * sculColor = (BYTE *)pValue.pBits + 3;
	BYTE * bombColor = (BYTE *)bombValue.pBits + 3;

	int blend;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (blend = bombColor[i * bombValue.Pitch + j * 4])
			{
				AlphaBlend(blend, sculColor + i * pValue.Pitch + j * 4 - 3, bombColor + i * bombValue.Pitch + j * 4 - 3);
				AlphaBlend(blend, sculColor + i * pValue.Pitch + j * 4 - 2, bombColor + i * bombValue.Pitch + j * 4 - 2);
				AlphaBlend(blend, sculColor + i * pValue.Pitch + j * 4 - 1, bombColor + i * bombValue.Pitch + j * 4 - 1);
			}
		}
	}

	m_pSculpture->GetTexture()->UnlockRect(0);
	texture->GetTexture()->UnlockRect(0);
}
