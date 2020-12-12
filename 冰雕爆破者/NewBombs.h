#pragma once
class CNewBombs:
	public CSprite
{
public:
	CNewBombs();
	~CNewBombs();
	void Draw(bool updateMatrix = true);
	void Update();

	void SetType(INT Type) { m_iType = Type; }
	INT  GetType() { return m_iType; }
	void SetShowBout(INT bout) { m_showBout = bout; }
	INT  GetShowBout() { return m_showBout; }
	void SetDisappearBout(INT bout) { m_disappearBout = bout; }
	INT  GetDisappearBout() { return m_disappearBout; }
	void SetBombNum(INT num) { m_iBombNum = num; }
	INT  GetBombNum() { return m_iBombNum; }
	void SetCurBout(INT bout) { m_curBout = bout; }
protected:
private:
	INT			m_iType;	//�ӵ�����
	INT			m_showBout;	//���ֵĻغ���
	INT			m_disappearBout;	//��ʧ�Ļغ���
	INT			m_iBombNum;	//ը�����������
	INT			m_curBout;	//��ǰ�غ�
	float		m_fAngle1;	//����1�ĽǶ�
	float		m_fAngle2;	//����2�ĽǶ�
};

