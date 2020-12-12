#pragma once
class CCollisionObj
{
public:
	CCollisionObj();
	~CCollisionObj();

	void	setCollisionArr(const RECT & arr) { m_CollisionArr = arr; }		//������ײ����
	RECT	getCollisionArr() { return m_CollisionArr; }	//�����ײ����

	virtual int		dirOfObj(CCollisionObj * colObj);	//���ش���Ķ�����������������λ��(������)
	virtual bool	isCollision(POINT pt) { return PtInRect(&m_CollisionArr, pt); }	//���Ƿ���ײ
	virtual bool	isCollision(const RECT & rect);	//������ײ
	virtual bool	isCollision(CCollisionObj * colObj);	//����ײ�����ж��Ƿ���ײ
	virtual void	move(int x, int y);	//�ƶ�

protected:
	RECT	m_CollisionArr;	//��ײ����

private:
};

