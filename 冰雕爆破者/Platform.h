#pragma once
class CPlatform
	:public CSingleton<CPlatform>
{
	//����
	friend class CSingleton<CPlatform>;
	CPlatform();
public:
	~CPlatform();
	void AddPlatData(TSTRING, void*);	//�������
	void*GetPlatData(TSTRING);			//��ȡ����
private:
	map<TSTRING, void*>	m_mapPlat;	//�洢ƽ̨���ݵ�����	
};

