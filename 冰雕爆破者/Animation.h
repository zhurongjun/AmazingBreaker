#pragma once

//存储帧信息的结构体
typedef struct FrameData
{
	int offsetX;	//相对于左上角的X偏移
	int offsetY;	//相对于左上角的Y偏移
	SIZE size;		//图片的大小

}FRAMEDATA, *PFRAMEDATA;

class CAnimation
{
public:
	CAnimation(TSTRING name);
	~CAnimation();

	//添加纹理图
	void AddTexture(TSTRING name, SIZE size, int offsetX = 0, int offsetY = 0);


	void SetAnimationName(TSTRING name) { m_animationName = name; }	//设置动画的名字
	TSTRING GetAnimationName() { return m_animationName; }	//获取动画的名字

	void SetCountFrame(int countFrame) { m_nCountFrame = countFrame; }	//设置帧数
	int GetCountFrame() { return m_nCountFrame; }	//获取帧数
	
	int GetCurFrame() { return m_nCurFrame; }	//获取当期帧的位置
	void SetCurFrame(int curFrame) { m_nCurFrame = curFrame; }		//设置当期帧的位置

	int GetWidth(int curFrame = 0) { return m_gTextureSize[curFrame].size.cx; }	//获取宽度
	int GetHeight(int curFrame = 0) { return m_gTextureSize[curFrame].size.cy; }//获取高度

	int GetOffSetX(int curFrame = 0) { return m_gTextureSize[curFrame].offsetX; }	//获取x偏移量
	int GetOffSetY(int curFrame = 0) { return m_gTextureSize[curFrame].offsetY; }	//获取y偏移量
	LPDIRECT3DTEXTURE9 GetTexture(int curFrame) { return m_gTexture[curFrame]; }	//获取某一帧的纹理图

	void SetOffSetX(int x, int curFrame = 0) { m_gTextureSize[curFrame].offsetX = x; }	//设置x偏移量
	void SetOffSetY(int y, int curFrame = 0) { m_gTextureSize[curFrame].offsetY = y; }	//设置y偏移量


protected:
private:
	TSTRING m_animationName;	//动画名称
	int		m_nCountFrame;		//总帧数
	int		m_nCurFrame;		//当前帧数
	vector<LPDIRECT3DTEXTURE9> m_gTexture;	//图片容器
	vector<FRAMEDATA> m_gTextureSize;		//保存每帧图片起始位置和大小
};

