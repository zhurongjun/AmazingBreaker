#pragma once
#include "BaseBomb.h"

class CBombLine;

class CStaticBomb :
	public CBaseBomb
{
public:
	enum ECOLOR
	{
		RED,
		GREEN,
		VIOLET
	};
public:
	CStaticBomb();
	~CStaticBomb();

	virtual void Update();	//����

	void SetColor(ECOLOR color);
	virtual void Draw(bool updateMatrix = true); //����
	void DrawLine();	//����������
	void Bomb();	//������̬ը��
	void AddLine(D3DXVECTOR2 pos);
	void DeleteLine(D3DXVECTOR2 pos);

protected:
private:
	ECOLOR		m_color;		//��̬ը������ɫ
	list<CBombLine *>	m_listLines;	//ά����һ��ָ�����б�
};

