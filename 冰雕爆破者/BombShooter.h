#pragma once

class CBaseBomb;

class CBombShooter :
	public CMouseListener
{
private:
	const static D3DXVECTOR2 s_BombStackPos[5];
	const static float s_fBigScalling;
	const static float s_fSmallScalling;
public:
	enum eSTATE {
		NORMAL,		//通常状态
		RESET = 1,	//重置(归位)
		SHOOTED = 2,//发射(射出)
		WAIT = 3,	//等待(等待炸弹爆炸)
		FILL = 4,	//填充(填充炸弹)
		ADD = 5,	//添加(添加炸弹)
		EXCHANGE	//交换(用弹夹内的炸弹替换当前炸弹)
	};

public:
	CBombShooter();
	~CBombShooter();

	//初始化
	void init();
	void reset();	//重置

	//事件监听
	void OnMouseDown(SEvent);
	void OnMouseUp(SEvent);
	void OnMouseMove(SEvent);

	//绘制
	void Draw();
	//更新
	void Update();

	//数据设置
	void SetDestroyPre(float precent, bool anim);
	float GetDestoryPre() { return m_fDestoryPre; }	//获取破坏进度
	int  GetNowBomb() { return m_listBomb.size() + m_curBomb ? 1 : 0; }
	eSTATE GetState() { return m_eState; }	//获取状态

	//添加炸弹
	void AddBomb(CBaseBomb * bomb);
	void AddBomb(vector<CBaseBomb *> * vec);
	//清空炸弹
	void ClearBomb();
	//更新炸弹夹
	void UpdateBombStack();
	//交换炸弹
	void ExchangeBomb();
	int GetBout() { return m_iCurBont; }	//获取回合数
	void SetBout(int bout) { m_iCurBont = bout; }	//设置回合数
protected:
	void UpdateState();		//更新状态
private:
	int			m_preAnimCount;	//百分比的增长动画计数
	bool		m_bSelect;		//是否选中
	float		m_fDestoryPre;	//破坏进度(百分比)
	float		m_fShootSpeed;	//发射速度
	float		m_fMaxRound;	//发射拉动的最大半径
	float		m_fMinRound;	//发射拉动的最小的半径
	D3DXVECTOR2	m_vecNorPos;	//通常下的位置
	D3DXVECTOR2 m_vecShootDir;	//发射方向
	D3DXVECTOR2	m_vecCenterPos;	//发射中心点
	CBaseBomb *	m_curBomb;		//发射器当前装载的炸弹
	eSTATE		m_eState;		//当前的状态
	int			m_stateCount;	//状态计数
	int			m_stateParam;	//状态参数
	int			m_iCurBont;		//当前回合
	CAnimHander *		m_animBlend;	//淡入效果
	list<CBaseBomb *>	m_listBomb;		//发射器内的炸弹
	CAnimHander *		m_Special;		//特殊动画
	vector<CBaseBomb *> m_InsertBomb;	//待添加的炸弹

	//构成图片
	CSprite *	m_pArrow;		//指向箭头
	CSprite *	m_pPrecentBar;	//破坏进度条显示
	CSprite *	m_pMainUI;		//主UI
	CSprite *	m_pLine;		//分割线
	CSprite *	m_pSpringL;		//弹簧
	CSprite *	m_pSpringR;		//弹簧
	CSprite *	m_pNode;		//弹簧的连接节点
	CSprite *	m_pHolder;		//抓住炸弹的holder
	CSprite *	m_pPrecentText;	//百分比的字符
	CSprite *	m_pSpecialSp;	//储存特殊动画的精灵
};

