#pragma once

class CAnimHander
{
public:
	enum EANIM {
		eANIM_SCALINGX = 0,	//x缩放
		eANIM_SCALINGY = 1,	//y缩放
		eANIM_POSX = 2,	//x位移
		eANIM_POSY = 3,	//y位移
		eANIM_ROTATE = 4,	//旋转
		eANIM_BLEND = 5,	//透明度
		eANIM_LOOP	//特殊动效节点(循环)
	};

public:
	CAnimHander();
	~CAnimHander();
	
	void init(TiXmlElement * element);
	void SetMainTarget(IAnimable * target);		//设置主对象
	IAnimable * GetMainTarget() { return m_pMainTarget; }	//获取主对象
	void addFixTarget(TSTRING name, IAnimable * target);	//添加固态动效

	void UpdateFixPos();	//更新静态对象的位置
	void LoopRender();	//重复播放
	void Render(bool renderFix = true);	//单次播放
	void Update();	//更新
	CAnimHander * Copy(bool copyFix = true);	//复制一个一样的对象
	void Release();	//释放
	void Reset();	//重置
	void EndDraw() { m_bIsEnd = TRUE; }
	bool IsEnd() { return m_bIsEnd; }	//是否结束
protected:
private:
	IAnimable *					m_pMainTarget;	//主目标
	map<TSTRING, IAnimable *> 	m_mapFixTarget;	//固定目标
	vector<CAnimHander *>		m_vecChildAnim;	//子动效
	TSTRING		m_sTarget;	//作用对象
	INT			m_iAllLoop;	//循环总次数
	INT			m_inowLoop;	//当前的循环次数
	INT			m_iDelay;	//延迟次数
	INT			m_iCurCount;//当前延迟计数
	double		m_iParam;	//参数
	BOOL		m_bIsSet;	//是否是设置
	BOOL		m_bIsEnd;	//动效是否结束
	EANIM		m_eType;	//标签类型
};

