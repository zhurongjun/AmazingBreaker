#pragma once
class CCollisionObj
{
public:
	CCollisionObj();
	~CCollisionObj();

	void	setCollisionArr(const RECT & arr) { m_CollisionArr = arr; }		//设置碰撞区域
	RECT	getCollisionArr() { return m_CollisionArr; }	//获得碰撞区域

	virtual int		dirOfObj(CCollisionObj * colObj);	//返回传入的对象相对于主调对象的位置(仅方向)
	virtual bool	isCollision(POINT pt) { return PtInRect(&m_CollisionArr, pt); }	//点是否碰撞
	virtual bool	isCollision(const RECT & rect);	//矩形碰撞
	virtual bool	isCollision(CCollisionObj * colObj);	//和碰撞对象判断是否碰撞
	virtual void	move(int x, int y);	//移动

protected:
	RECT	m_CollisionArr;	//碰撞区域

private:
};

