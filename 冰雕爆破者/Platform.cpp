#include "stdafx.h"
#include "Platform.h"

CPlatform::CPlatform()
{
}

CPlatform::~CPlatform()
{
	m_mapPlat.clear();	//释放map
}

void CPlatform::AddPlatData(TSTRING name, void*res)
{
	m_mapPlat[name] = res;	//添加数据
}

void* CPlatform::GetPlatData(TSTRING name)
{
	//按键值查找，如果有则返回，否则返回空
	map<TSTRING, void*>::iterator it = m_mapPlat.find(name);
	if (it != m_mapPlat.end())
		return it->second;
	else
		return nullptr;
}
