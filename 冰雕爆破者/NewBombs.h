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
	INT			m_iType;	//子弹类型
	INT			m_showBout;	//出现的回合数
	INT			m_disappearBout;	//消失的回合数
	INT			m_iBombNum;	//炸弹的填充数量
	INT			m_curBout;	//当前回合
	float		m_fAngle1;	//星星1的角度
	float		m_fAngle2;	//星星2的角度
};

