#pragma once
#include "Sprite.h"

//所有炸弹的基类
class CBaseBomb :
	public CSprite ,
	public CMouseListener
{
public:
	enum eSTATE
	{
		PREPAREING,	//准备中(束缚状态，准备发射)
		FLYING,		//飞行中(飞行状态)
		SECOND,		//二段状态
		BOMB,		//爆炸中
		DESTORY		//已经被破坏
	};
	enum eTYPE {
		NORMAL,		//普通炸弹
		TRIPLE,		//三分散弹
		ROTATE,		//旋转三球
		GHOST,		//幽灵子弹
		ROCKET,		//火箭
		STATIC		//静态嵌入炸弹
	};
public:
	CBaseBomb();
	virtual ~CBaseBomb();

	void SetDamping(float damping) { m_fDamping = damping; }	//设置阻尼
	float GetDamping() { return m_fDamping; }	//获取阻尼
	void SetSpeed(float speed) { m_fSpeed = speed; }	//设置速度
	float GetSpeed() { return m_fSpeed; }	//获取阻尼
	void SetDir(const D3DXVECTOR2 & dir);	//设置方向
	D3DXVECTOR2 GetDir() { return m_vecDir; }	//获取方向
	eSTATE GetState() { return m_eState; }	//获取状态
	eTYPE  GetBombType() { return m_eType; }	//获取类型

	virtual void OnMouseDown(SEvent){}
	virtual void OnMouseUp(SEvent){}
	virtual void OnMouseMove(SEvent){}
	virtual void OnMouseWheel(SEvent){}

	virtual void FreeBomb(const D3DXVECTOR2 &dir, float speed);	//释放炸弹，并给一个初速度和方向
	virtual void Update();	//更新

protected:
	float		m_fDamping;		//炸弹的飞行阻尼
	float		m_fDampFactor;	//炸弹的阻尼系数
	float		m_fSpeed;		//炸弹飞行的速度
 	float		m_colRound;		//碰撞半径
	D3DXVECTOR2	m_vecDir;		//炸弹飞行的方向
	eSTATE		m_eState;		//炸弹的状态
	INT			m_iStateCount;	//状态计数
	INT			m_iStateParam;	//状态参数
	eTYPE		m_eType;		//子弹类型
private:
};

