#pragma once

class CStaticBomb;

class CTripleBomb :
	public CBaseBomb
{
public:
	CTripleBomb();
	~CTripleBomb();
	virtual void Update();	//����
	void Draw(bool updateMatrix = true);
	virtual void OnMouseDown(SEvent event);

protected:
private:
	CStaticBomb *	m_StaticBombs[3];		//���ѳ�������ը��
};

