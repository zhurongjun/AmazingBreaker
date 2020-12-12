#pragma once
class CGameScene
{
public:
	CGameScene();
	virtual ~CGameScene();

	virtual void Init() = 0;	//��ʼ��
	virtual void Render() = 0;	//����
	virtual void Update() = 0;	//����
	virtual void TurnIn() = 0;	//�����׼������
	virtual void TurnOut() = 0;	//�����׼������

protected:
	TSTRING m_nextScene;	//��һ������
};

