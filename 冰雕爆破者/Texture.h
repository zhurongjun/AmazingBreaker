#pragma once

//纹理类，储存单张单帧的图片，最为底层的资源类
class CTexture
{
public:
	CTexture();
	~CTexture();

	void	Init();		//初始化
	void	SetColRef(COLORREF);//设置过滤的透明色
	COLORREF	GetColRef();	//获取过滤的透明色

	void	SetWidth(WORD);		//设置图片的宽
	void	SetHeight(WORD);	//设置图片的高
	WORD	GetWidth();		//获取图片的宽度
	WORD	GetHeight();	//获取图片的高度

	void SetName(TSTRING name);		//设置图片的名称
	void SetPath(TSTRING path);		//设置图片的路径
	DWORD	GetColorValue(POINT);	//获取某一像素上的颜色
	DWORD	GetBlendValue(POINT);	//获取某一点的透明度

	LPDIRECT3DTEXTURE9 GetTexture() { return m_pTexture; }	//获取纹理
	void	Release();		//释放纹理

public:
	LPDIRECT3DTEXTURE9	m_pTexture;	//纹理指针
	WORD		m_Width;	//宽度
	WORD		m_Height;	//高度
	COLORREF	m_colRef;	//过滤的透明色
	TSTRING		m_name;		//图片的名称
	TSTRING		m_path;		//图片的路径
};

