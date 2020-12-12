#pragma once
enum IMG_TYPE                                     //ͼƬ���ͣ����ŵ�֡�����Ŷ�֡�����Ŷ�֡
{
	eSTATIC_IMG = 0,	//���ŵ�֡
	eSINGLE_IMG = 1,	//���Ŷ�֡
	eMUL_IMG = 2		//���Ŷ�֡
};

class CTexture;
class CAnimation;

class CResManager :
	public CSingleton<CResManager>//�̳е���
{
	friend class CSingleton<CResManager>;
	CResManager();
public:
	~CResManager();
	void		LoadResFromFile(TSTRING);	//���ĵ�������Դ
	void		AddTexture(TSTRING, CTexture*);	//�������ͼ
	CTexture*	GetTexture(TSTRING);		//��ȡ����ͼ
	void		AddAnimation(TSTRING animationName, CAnimation* animation);	//��Ӷ���
	CAnimation* GetAnimation(TSTRING animationName);	//��ȡ����
private:
	map<TSTRING, CTexture*>		m_mapTexture;	//�洢���е�����
	map<TSTRING, CAnimation*>	m_mapAnim;		//�洢���еĶ���
};

