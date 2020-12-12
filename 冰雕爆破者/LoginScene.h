#pragma once
#include "GameScene.h"
class CLoginScene :
	public CGameScene
{
public:
	CLoginScene();
	~CLoginScene();

	void Init();	//��ʼ��
	void Render();	//����
	void Update();	//����
	void TurnIn();	//�����׼������
	void TurnOut();	//�˳��ĺ�������

protected:
private:
	CSprite *	m_pBackground;	//����ͼ
	CSprite *	m_pLogo;		//Logo
	CSprite *	m_pStar;		//��������
	CRoundButton	*	m_pPlayButtom;	//������Ϸ��ť
	struct star{
		int		life;	//��������
		float	size;	//��С(����ֵ)
		D3DXVECTOR2	dir;	//����
		D3DXVECTOR2 pos;	//λ��
		void upDate();
	}m_stars[200];

	void toSelectMenu(int num);	//ת�����ý���
};

