#include "stdafx.h"
#include "CollisionObj.h"


CCollisionObj::CCollisionObj()
{
}


CCollisionObj::~CCollisionObj()
{
}

int CCollisionObj::dirOfObj(CCollisionObj * colObj)
{
	POINT thisPos = { (m_CollisionArr.left + m_CollisionArr.right) / 2,(m_CollisionArr.top + m_CollisionArr.bottom) / 2 };
	POINT colPos = { (colObj->m_CollisionArr.left + colObj->m_CollisionArr.right) / 2,(colObj->m_CollisionArr.top + colObj->m_CollisionArr.bottom) / 2 };
	int dir = 0;
	if (thisPos.x < colPos.x)
		dir |= eDIR_RIGHT;
	else
		dir |= eDIR_LEFT;
	if (thisPos.y < colPos.y)
		dir |= eDIR_DOWM;
	else
		dir |= eDIR_UP;
	return dir;
}

bool CCollisionObj::isCollision(const RECT & rect)
{
	return (m_CollisionArr.left < rect.right && 
			m_CollisionArr.right > rect.left &&
			m_CollisionArr.top > rect.bottom &&
			m_CollisionArr.bottom < rect.top);
}

bool CCollisionObj::isCollision(CCollisionObj * colObj)
{
	return this->isCollision(colObj->m_CollisionArr);
}

void CCollisionObj::move(int x, int y)
{
	m_CollisionArr.left += x;
	m_CollisionArr.right += x;
	m_CollisionArr.top += y;
	m_CollisionArr.bottom += y;
}
