#pragma once
enum IMG_TYPE                                     //图片类型：单张单帧，单张多帧，多张多帧
{
	eSTATIC_IMG = 0,	//单张单帧
	eSINGLE_IMG = 1,	//单张多帧
	eMUL_IMG = 2		//多张多帧
};

class CTexture;
class CAnimation;

class CResManager :
	public CSingleton<CResManager>//继承单例
{
	friend class CSingleton<CResManager>;
	CResManager();
public:
	~CResManager();
	void		LoadResFromFile(TSTRING);	//从文档加载资源
	void		AddTexture(TSTRING, CTexture*);	//添加纹理图
	CTexture*	GetTexture(TSTRING);		//获取纹理图
	void		AddAnimation(TSTRING animationName, CAnimation* animation);	//添加动画
	CAnimation* GetAnimation(TSTRING animationName);	//获取动画
private:
	map<TSTRING, CTexture*>		m_mapTexture;	//存储所有的纹理
	map<TSTRING, CAnimation*>	m_mapAnim;		//存储所有的动画
};

