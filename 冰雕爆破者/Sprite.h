
#pragma once

class CAnimation;

class CSprite :
	public IAnimable
{
public:
	enum ECENTER_MOD {
		ECEN_MIDDLE = 1,
		ECEN_CENTER = 1 << 1,
		ECEN_LEFT = 1 << 2,
		ECEN_RIGHT = 1 << 3,
		ECEN_TOP = 1 << 4,
		ECEN_BOTTOM = 1 << 5
	};

public:
	CSprite();
	CSprite(const CSprite & sprite);
	virtual ~CSprite();

	virtual void Init() {}		//初始化精灵
	void Init(TSTRING animationName);	//初始化精灵并添加一个动画资源
	void Init(vector<TSTRING> &animationName);	//初始化精灵并添加一组动画资源

	virtual void Draw(bool updateMatrix = true);//绘制 虚函数
	virtual void Update();//更新 虚函数
	virtual void ShowFrame(TSTRING animationName);	//显示某个动画的当前帧

	virtual void LoopPlayAnimation(TSTRING animationName, int speed = 1);//循环播放动画
	void ResetMonoPlay();	//重置单次播放动画
	virtual void PlayAnimation(TSTRING animationName);//单次播放动画
	bool isEnd() { return m_bMonoPlayFinish; }	//播放是否结束

	void SetCurAnimation(TSTRING animation);	//设置当前动画
	void SetCurFrame(int frame);	//设置当前动画的当前帧数

	void SetSrcPos(D3DXVECTOR2 srcPos);		//设置原图位置
	void SetDesPos(D3DXVECTOR2 desPos);		//设置屏幕绘制位置
	void SetWorldPos(D3DXVECTOR2 worldPos);	//设置精灵世界坐标

	D3DXVECTOR2 GetSrcPos() { return m_sSrcPos; }	//返回原图坐标
	D3DXVECTOR2 GetDesPos() { return m_sDesPos; }	//返回目标坐标
	D3DXVECTOR2 GetWorldPos() { return m_sWorldPos; }//返回目标世界坐标

	int  GetLayer() { return m_nLayer; }//返回层级
	void SetLayer(int layer) { m_nLayer = layer; }	//设置层级
	void SetShowWidth(int showWidth);	//设置显示宽
	void SetShowHeight(int showHeight);	//设置显示高
	int GetShowWidth() { return m_nShowWidth; }		//返回显示宽
	int GetShowHeight() { return m_nShowHeight; }	//返回显示高
	void SetSrcRect(const RECT & rect) { m_sSrcRect = rect; }	//设置增量矩形
	void ResetSrcRect() { ZeroMemory(&m_sSrcRect, sizeof(RECT)); }	//重置增量矩形
	SIZE getDrawSize();	//获取绘制后的大小
	RECT getDrawRect();	//获取绘制后的矩形

	void SetCenter(D3DXVECTOR3 center);//设置锚点
	void SetCenter(int centerMod);//设置锚点
	D3DXVECTOR3 GetCenter() { return m_center; }//获得锚点
	void SetTransparent(int transpatent);//设置透明度
	int GetTransparent() { return m_nTransparent >> 24; }//获得透明度
	void SetShowColor(int showColor);//设置加色
	int GetShowColor() { return m_nRGBColor; }//获得加色

	void UpdateMatrix();  //更新矩阵
	void SetMatrix(D3DXMATRIX & mat);	//设置自定义的矩阵
	D3DXMATRIX GetMatrix() { return matMatrix; }	//获取变换矩阵

	void AjustScale(SIZE showSize);	//将原图片校准至固定大小
	void SetXScale(float xScale);	//设置X轴的缩放
	void SetYScale(float yScale);	//设置Y轴的缩放
	float GetXScale() { return m_xScale; }	//获取X轴的缩放
	float GetYScale() { return m_yScale; }	//获取Y轴的缩放
	void SetAngle(float angle); //设置旋转角度
	float GetAngle() { return m_angle; }	//获取旋转角度
	
	bool IsVisible() { return m_bIsVisible; }	//返回精灵是否可见
	bool IsActive() { return m_bIsActive; }	//返回精灵是否激活
	void SetVisible(bool visible) { m_bIsVisible = visible; }	//设置精灵是否可见
	void SetActive(bool active) { m_bIsActive = active; }	//设置精灵是否激活

protected:
	map<TSTRING, CAnimation*> m_pAnimation;//存放动画的容器
	TSTRING			m_curAnimation;	//当前动画名称
	int				m_nCurFrame;	//当前帧数
	LPD3DXSPRITE	m_pSprite;	//图片精灵
	int				m_nLayer;	//层级

	D3DXVECTOR2		m_sSrcPos;	//原图坐标
	D3DXVECTOR2		m_sDesPos;	//目标坐标
	D3DXVECTOR2		m_sWorldPos;	//世界坐标位置
	RECT			m_sSrcRect;	//绘制的增量矩形

	int m_nShowWidth;	//显示宽度
	int m_nShowHeight;	//显示高度
	int m_nRGBColor;	//显示色
	unsigned int  m_nTransparent;	//透明度
	D3DXVECTOR3	m_center;//锚点位置

	D3DXMATRIX	scaleMatrix;	//缩放矩阵
	D3DXMATRIX	rotateMatrix;	//旋转矩阵
	D3DXMATRIX	transMatrix;	//平移矩阵
	D3DXMATRIX	matMatrix;		//综合矩阵
	float m_xScale;	//x轴缩放
	float m_yScale;	//y轴缩放
	float m_angle;	//旋转角度
	bool m_bIsVisible;	//是否可见  
	bool m_bIsActive;	//是否激活

	bool m_bMonoPlayStart;	//单次播放动画开始
	bool m_bMonoPlayFinish;	//单次播放动画结束
	int m_nAnimationSpeedCount;	//动画播放速度计数器
private:
	static LPD3DXSPRITE s_sprite;	//全局精灵
	static INT			s_spriteNum;	//精灵数量
};
