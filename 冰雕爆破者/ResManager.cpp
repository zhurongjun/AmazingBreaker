#include "stdafx.h"
#include "Texture.h"
#include "Animation.h"
#include "ResManager.h"

CResManager::CResManager()
{
}

CResManager::~CResManager()
{
	map<TSTRING, CTexture*>::iterator it = m_mapTexture.begin();
	for (; it != m_mapTexture.end(); it++)
		delete it->second;	//�ͷ�ͼƬ��Դ
	m_mapTexture.clear();
	for (auto it = m_mapAnim.begin() ; it != m_mapAnim.end(); it++)
		delete it->second;	//�ͷ�ͼƬ��Դ
}

void CResManager::LoadResFromFile(TSTRING fileName)
{
	char file[256];	//ANIS���ļ���
	TCHAR name[256], path[256];	//�洢��Դ���ֺ�·�����ַ���
	Auto_To_Anis(fileName.c_str(), 256, file, 256);	//ת��ΪANIS���ļ���

	//����XML�ĵ�
	TiXmlDocument *doc = new TiXmlDocument(file);	
	doc->LoadFile();
	TiXmlElement*element = doc->FirstChildElement();

	//ѭ����ȡXML�ĵ�����
	while (element)
	{
		int imgType;
		element->Attribute("imgType", &imgType);
		switch ((IMG_TYPE)imgType)
		{
		case eSTATIC_IMG:	//���ŵ�֡
		{
			//��ȡ����
			int m_colorR, m_colorG, m_colorB, m_colorA;	//���˵�͸��ɫ
			element->Attribute("colorA", &m_colorA);
			element->Attribute("colorR", &m_colorR);
			element->Attribute("colorG", &m_colorG);
			element->Attribute("colorB", &m_colorB);
			Anis_To_Auto(element->Attribute("name"), 256, name, 256);
			Anis_To_Auto(element->Attribute("path"), 256, path, 256);

			//��������
			CTexture* pTex = new CTexture;	//��������ͼƬ
			pTex->SetName(name);
			pTex->SetPath(path);
			pTex->SetColRef(((COLORREF)((((DWORD)(BYTE)(m_colorA)) << 24) | RGB(m_colorR, m_colorG, m_colorB))));
			pTex->Init();
			AddTexture(name, pTex);

			//���ض���
			CAnimation* pAnimation = new CAnimation(name);
			pAnimation->SetCountFrame(1);
			pAnimation->AddTexture(name, { pTex->m_Width,pTex->m_Height });
			AddAnimation(name, pAnimation);//<�����鶯����������
		}
		break;
		case eSINGLE_IMG:	//���Ŷ�֡
		{
			//��ȡ����
			int m_colorR, m_colorG, m_colorB, m_colorA;
			element->Attribute("colorA", &m_colorA);
			element->Attribute("colorR", &m_colorR);
			element->Attribute("colorG", &m_colorG);
			element->Attribute("colorB", &m_colorB);
			Anis_To_Auto(element->Attribute("name"), 256, name, 256);
			Anis_To_Auto(element->Attribute("path"), 256, path, 256);

			//��������
			CTexture* pTex = new CTexture;
			pTex->SetName(name);
			pTex->SetPath(path);
			pTex->SetColRef(((COLORREF)((((DWORD)(BYTE)(m_colorA)) << 24) | RGB(m_colorR, m_colorG, m_colorB))));
			pTex->Init();
			AddTexture(name, pTex);

			//���ض���
			int countFrame;
			int countDir;
			element->Attribute("countFrame", &countFrame);
			element->Attribute("countDir", &countDir);
			int frameWidth = pTex->m_Width / countFrame;
			int frameHeight = pTex->m_Height / countDir;
			SIZE size = { frameWidth,frameHeight };
			TiXmlElement * child;	//����
			child = element->FirstChildElement();
			int j = 0;
			TCHAR childName[256];
			while(child)
			{
				CAnimation* pAnimation = new CAnimation(name);
				pAnimation->SetCountFrame(countFrame);
				Anis_To_Auto(child->Attribute("name"), 256, childName, 256);
				for (int i = 0; i < countFrame; i++)
				{
					pAnimation->AddTexture(name, size, i*frameWidth, j*frameHeight);
				}
				AddAnimation(childName, pAnimation);//<�����鶯����������
				child = child->NextSiblingElement();
				j++;
			}
		}
		break;
		case eMUL_IMG:
		{
			//��ȡ��������
			TCHAR title[256];
			Anis_To_Auto(element->Attribute("title"), 256, title, 256);
			int countFrame;
			element->Attribute("countFrame", &countFrame);
			int m_colorR, m_colorG, m_colorB, m_colorA;
			element->Attribute("colorA", &m_colorA);
			element->Attribute("colorR", &m_colorR);
			element->Attribute("colorG", &m_colorG);
			element->Attribute("colorB", &m_colorB);
			TiXmlElement*child = element->FirstChildElement();
			CAnimation* pAnimation = new CAnimation(title);
			pAnimation->SetCountFrame(countFrame);
			
			//��ȡ������Ϣ������
			while (child)
			{
				Anis_To_Auto(child->Attribute("path"), 256, path, 256);
				
				//��������ͼ
				CTexture* pTex = new CTexture;
				pTex->SetName(path);
				pTex->SetPath(path);
				pTex->SetColRef(((COLORREF)((((DWORD)(BYTE)(m_colorA)) << 24) | RGB(m_colorR, m_colorG, m_colorB))));
				pTex->Init();
				AddTexture(path, pTex);
				pAnimation->AddTexture(path, { pTex->m_Width, pTex->m_Height });
				child = child->NextSiblingElement();
			}
			AddAnimation(title, pAnimation);//<�����鶯����������
		}
		break;
		}
		element = element->NextSiblingElement();
	}
	delete doc;
}

void CResManager::AddTexture(TSTRING name, CTexture*res)
{
	m_mapTexture[name] = res;
}

CTexture* CResManager::GetTexture(TSTRING name)
{
	map<TSTRING, CTexture*>::iterator it = m_mapTexture.find(name);
	if (it != m_mapTexture.end())
		return it->second;
	else
		return nullptr;
}


void CResManager::AddAnimation(TSTRING animationName, CAnimation * animation)
{
	m_mapAnim[animationName] = animation;
}

CAnimation * CResManager::GetAnimation(TSTRING animationName)
{
	map<TSTRING, CAnimation*>::iterator iter = m_mapAnim.find(animationName);
	if (iter != m_mapAnim.end())
	{
		return iter->second;
	}
	else
	{
		TSTRING str = animationName + _T("ͼƬ����ʧ��$$$$$$$$$$\n");
		OutputDebugString(str.c_str());
		return nullptr;
	}
}
