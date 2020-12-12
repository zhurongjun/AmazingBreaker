#include "stdafx.h"
#include "Platform.h"

CPlatform::CPlatform()
{
}

CPlatform::~CPlatform()
{
	m_mapPlat.clear();	//�ͷ�map
}

void CPlatform::AddPlatData(TSTRING name, void*res)
{
	m_mapPlat[name] = res;	//�������
}

void* CPlatform::GetPlatData(TSTRING name)
{
	//����ֵ���ң�������򷵻أ����򷵻ؿ�
	map<TSTRING, void*>::iterator it = m_mapPlat.find(name);
	if (it != m_mapPlat.end())
		return it->second;
	else
		return nullptr;
}
