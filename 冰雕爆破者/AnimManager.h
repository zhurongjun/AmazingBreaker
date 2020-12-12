#pragma once
class CAnimManager :
	public CSingleton<CAnimManager>
{
	friend class CSingleton<CAnimManager>;
	CAnimManager();
public:
	~CAnimManager();

	void LoadAnimFromFile(TSTRING fileName);	//从文件加载动态效
	CAnimHander * GetAnimHander(TSTRING name) { return m_mapAnimHanders[name]; }

protected:
private:
	map<TSTRING, CAnimHander*>	m_mapAnimHanders;	//存储所有动效的容器
};

