#pragma once
class CPlatform
	:public CSingleton<CPlatform>
{
	//单例
	friend class CSingleton<CPlatform>;
	CPlatform();
public:
	~CPlatform();
	void AddPlatData(TSTRING, void*);	//添加数据
	void*GetPlatData(TSTRING);			//获取数据
private:
	map<TSTRING, void*>	m_mapPlat;	//存储平台数据的容器	
};

