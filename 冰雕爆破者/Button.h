#pragma once
class Button :
	public IAnimable,
	public CMouseListener,
	public CCollisionObj
{
public:
	Button(TSTRING Normal,TSTRING MouseIn,TSTRING Pressed);
	~Button();

	//来自动效果接口的函数
	virtual void SetTransparent(int transpatent) {}//设置透明度
	virtual int GetTransparent() { return 0; }//获得透明度
	virtual void SetDesPos(D3DXVECTOR2 desPos);	//设置屏幕绘制位置
	virtual D3DXVECTOR2 GetDesPos() 
	{ 
		return m_vecDesPos; 
	}	//返回目标坐标
	virtual void Draw(bool updateMatrix = true) {}	//绘制
	virtual void SetXScale(float xScale) {}	//设置X轴缩放
	virtual void SetYScale(float yScale) {}	//设置Y轴缩放
	virtual float GetXScale() { return 0; }	//获取X轴的缩放
	virtual float GetYScale() { return 0; }	//获取Y轴的缩放
	virtual void SetAngle(float angle) {} //设置旋转角度
	virtual float GetAngle() { return 0; }	//获取旋转角度

	void Render();	//绘制
	void OnMouseDown(SEvent event);	//处理鼠标按下的函数
	void OnMouseUp(SEvent event);	//处理鼠标弹起的函数
	void OnMouseMove(SEvent  event);//处理鼠标移动的函数

	RECT GetRect(int width, int height);	//获取RC
	void Refresh();	//刷新
	void SetColorUse(BOOL useColor) { m_bUseColor = useColor; }	//设置是否使用颜色

	void  setCallBack(PFUN callBack) { this->m_pCallback = callBack; }	//设置回调函数
	void SetDownSound(TSTRING sound) { m_sDowmSound = sound; }	//设置按下的声音

private:
	CAnimHander *	m_pNormal;		//通常状态
	CAnimHander *	m_pMouseIn;		//鼠标放入
	CAnimHander *	m_pPressed;		//按下
	CAnimHander *	m_curState;		//状态
	D3DXVECTOR2		m_vecDesPos;	//绘制坐标
	TSTRING		m_sDowmSound;	//按下的声音
	PFUN		m_pCallback;	//回调函数
	BOOL		m_bIsPressed;	//是否被按下
	BOOL		m_bMouseIn;	//是否被按下
	BOOL		m_bUseColor;//是否使用颜色
};

