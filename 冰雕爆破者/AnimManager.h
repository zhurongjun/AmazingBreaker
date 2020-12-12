#pragma once
class CAnimManager :
	public CSingleton<CAnimManager>
{
	friend class CSingleton<CAnimManager>;
	CAnimManager();
public:
	~CAnimManager();

	void LoadAnimFromFile(TSTRING fileName);	//���ļ����ض�̬Ч
	CAnimHander * GetAnimHander(TSTRING name) { return m_mapAnimHanders[name]; }

protected:
private:
	map<TSTRING, CAnimHander*>	m_mapAnimHanders;	//�洢���ж�Ч������
};

