#pragma once
class CBombLine
{
public:
	CBombLine();
	~CBombLine();

	void Init(D3DXVECTOR2 begin, D3DXVECTOR2 end);
	D3DXVECTOR2 GetBeginPos() { return m_vecBegin; }
	D3DXVECTOR2 GetEndPos() { return m_vecEnd; }
	void Draw();
	void Update();

protected:
private:
	D3DXVECTOR2		m_vecBegin;		//起始点
	D3DXVECTOR2		m_vecEnd;		//结束点
	D3DXVECTOR2		m_vecDir;		//光点的运动方向
	float			m_fSpeed;		//光点运动的速度
	INT				m_iStateCount;	//状态计数
	BOOL			m_DrawLight;	//是否绘制闪光
	CSprite *		m_BombLine;		//炸弹连线
	CSprite	*		m_BombLight;	//炸弹高亮
};

