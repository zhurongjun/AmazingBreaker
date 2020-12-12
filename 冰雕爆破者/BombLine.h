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
	D3DXVECTOR2		m_vecBegin;		//��ʼ��
	D3DXVECTOR2		m_vecEnd;		//������
	D3DXVECTOR2		m_vecDir;		//�����˶�����
	float			m_fSpeed;		//����˶����ٶ�
	INT				m_iStateCount;	//״̬����
	BOOL			m_DrawLight;	//�Ƿ��������
	CSprite *		m_BombLine;		//ը������
	CSprite	*		m_BombLight;	//ը������
};

