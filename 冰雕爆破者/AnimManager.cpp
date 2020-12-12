#include "stdafx.h"
#include "AnimManager.h"


CAnimManager::CAnimManager()
{
}


CAnimManager::~CAnimManager()
{
	for (auto it : m_mapAnimHanders)
	{
		it.second->Release();
		delete it.second;
	}
}

void CAnimManager::LoadAnimFromFile(TSTRING fileName)
{
	TCHAR Tstr[256];
	char  Astr[256];
	TiXmlDocument * doc = new TiXmlDocument;
	Auto_To_Anis(fileName.c_str(), fileName.length() + 1, Astr, 256);
	doc->LoadFile(Astr);
	TiXmlElement * element = doc->FirstChildElement();
	TiXmlElement * tempElement;
	CAnimHander * tempAnim;
	while (element)
	{
		strcpy(Astr, element->Attribute("name"));
		Anis_To_Auto(Astr, 256, Tstr, 256);
		tempAnim = new CAnimHander;
		m_mapAnimHanders[Tstr] = tempAnim;
		//读取主循环
		tempElement = element->FirstChildElement("loop");
		tempAnim->init(tempElement);
		
		//读取静态资源
		tempElement = element->FirstChildElement("fix");
		while (tempElement)
		{
			strcpy(Astr, tempElement->Attribute("name"));
			Anis_To_Auto(Astr, 256, Tstr, 256);
			//读取基础信息
			CSprite * temp = new CSprite;
			temp->Init(Tstr);
			tempAnim->addFixTarget(Tstr, temp);
			//适配中心点
			strcpy(Astr, tempElement->Attribute("center"));
			char * posStr = strtok(Astr, "|");
			int cenMod = 0;
			int cenY = -1;
			if (!strcmp(posStr, "CEN"))
				cenMod |= CSprite::ECEN_CENTER;
			else if (!strcmp(posStr, "LEF"))
				cenMod |= CSprite::ECEN_LEFT;
			else if (!strcmp(posStr, "RIG"))
				cenMod |= CSprite::ECEN_RIGHT;
			else if (!strcmp(posStr, "MID"))
				cenMod |= CSprite::ECEN_MIDDLE;
			else if (!strcmp(posStr, "TOP"))
				cenMod |= CSprite::ECEN_TOP;
			else if (!strcmp(posStr, "BOT"))
				cenMod |= CSprite::ECEN_BOTTOM;
			else
				cenMod = atoi(posStr);

			posStr = strtok(nullptr, "|");
			if (!strcmp(posStr, "CEN"))
				cenMod |= CSprite::ECEN_CENTER;
			else if (!strcmp(posStr, "LEF"))
				cenMod |= CSprite::ECEN_LEFT;
			else if (!strcmp(posStr, "RIG"))
				cenMod |= CSprite::ECEN_RIGHT;
			else if (!strcmp(posStr, "MID"))
				cenMod |= CSprite::ECEN_MIDDLE;
			else if (!strcmp(posStr, "TOP"))
				cenMod |= CSprite::ECEN_TOP;
			else if (!strcmp(posStr, "BOT"))
				cenMod |= CSprite::ECEN_BOTTOM;
			else
				cenY = atoi(posStr);

			if (cenY >= 0)
				temp->SetCenter({ (float)cenMod,(float)cenY,0 });
			else
				temp->SetCenter(cenMod);
			tempElement = tempElement->NextSiblingElement("fix");
		}

		element = element->NextSiblingElement();
	}
	delete doc;
}
