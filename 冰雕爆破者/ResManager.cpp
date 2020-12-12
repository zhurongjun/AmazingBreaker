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
		delete it->second;	//释放图片资源
	m_mapTexture.clear();
	for (auto it = m_mapAnim.begin() ; it != m_mapAnim.end(); it++)
		delete it->second;	//释放图片资源
}

void CResManager::LoadResFromFile(TSTRING fileName)
{
	char file[256];	//ANIS的文件名
	TCHAR name[256], path[256];	//存储资源名字和路径的字符串
	Auto_To_Anis(fileName.c_str(), 256, file, 256);	//转换为ANIS的文件名

	//加载XML文档
	TiXmlDocument *doc = new TiXmlDocument(file);	
	doc->LoadFile();
	TiXmlElement*element = doc->FirstChildElement();

	//循环读取XML文档内容
	while (element)
	{
		int imgType;
		element->Attribute("imgType", &imgType);
		switch ((IMG_TYPE)imgType)
		{
		case eSTATIC_IMG:	//单张单帧
		{
			//读取数据
			int m_colorR, m_colorG, m_colorB, m_colorA;	//过滤的透明色
			element->Attribute("colorA", &m_colorA);
			element->Attribute("colorR", &m_colorR);
			element->Attribute("colorG", &m_colorG);
			element->Attribute("colorB", &m_colorB);
			Anis_To_Auto(element->Attribute("name"), 256, name, 256);
			Anis_To_Auto(element->Attribute("path"), 256, path, 256);

			//加载纹理
			CTexture* pTex = new CTexture;	//创建纹理图片
			pTex->SetName(name);
			pTex->SetPath(path);
			pTex->SetColRef(((COLORREF)((((DWORD)(BYTE)(m_colorA)) << 24) | RGB(m_colorR, m_colorG, m_colorB))));
			pTex->Init();
			AddTexture(name, pTex);

			//加载动画
			CAnimation* pAnimation = new CAnimation(name);
			pAnimation->SetCountFrame(1);
			pAnimation->AddTexture(name, { pTex->m_Width,pTex->m_Height });
			AddAnimation(name, pAnimation);//<将这组动画放入容器
		}
		break;
		case eSINGLE_IMG:	//单张多帧
		{
			//读取数据
			int m_colorR, m_colorG, m_colorB, m_colorA;
			element->Attribute("colorA", &m_colorA);
			element->Attribute("colorR", &m_colorR);
			element->Attribute("colorG", &m_colorG);
			element->Attribute("colorB", &m_colorB);
			Anis_To_Auto(element->Attribute("name"), 256, name, 256);
			Anis_To_Auto(element->Attribute("path"), 256, path, 256);

			//加载纹理
			CTexture* pTex = new CTexture;
			pTex->SetName(name);
			pTex->SetPath(path);
			pTex->SetColRef(((COLORREF)((((DWORD)(BYTE)(m_colorA)) << 24) | RGB(m_colorR, m_colorG, m_colorB))));
			pTex->Init();
			AddTexture(name, pTex);

			//加载动画
			int countFrame;
			int countDir;
			element->Attribute("countFrame", &countFrame);
			element->Attribute("countDir", &countDir);
			int frameWidth = pTex->m_Width / countFrame;
			int frameHeight = pTex->m_Height / countDir;
			SIZE size = { frameWidth,frameHeight };
			TiXmlElement * child;	//孩子
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
				AddAnimation(childName, pAnimation);//<将这组动画放入容器
				child = child->NextSiblingElement();
				j++;
			}
		}
		break;
		case eMUL_IMG:
		{
			//读取公共数据
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
			
			//读取孩子信息并加载
			while (child)
			{
				Anis_To_Auto(child->Attribute("path"), 256, path, 256);
				
				//加载纹理图
				CTexture* pTex = new CTexture;
				pTex->SetName(path);
				pTex->SetPath(path);
				pTex->SetColRef(((COLORREF)((((DWORD)(BYTE)(m_colorA)) << 24) | RGB(m_colorR, m_colorG, m_colorB))));
				pTex->Init();
				AddTexture(path, pTex);
				pAnimation->AddTexture(path, { pTex->m_Width, pTex->m_Height });
				child = child->NextSiblingElement();
			}
			AddAnimation(title, pAnimation);//<将这组动画放入容器
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
		TSTRING str = animationName + _T("图片加载失败$$$$$$$$$$\n");
		OutputDebugString(str.c_str());
		return nullptr;
	}
}
