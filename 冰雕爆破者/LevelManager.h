#pragma once

class CBombShooter;
class CStaticBomb;
class CNewBombs;

class CLevelManager :
	public CSingleton<CLevelManager>
{
	friend class CSingleton<CLevelManager>;
	CLevelManager();
public:
	struct SNewBomb
	{
		POINT	Pos;		//����λ��
		int		ShowTime;	//���ֵĻغ���
		int		Life;		//����
		int		Type;		//����
	};
	struct SLevelInfo
	{
		TSTRING				BKG;		//��������
		TSTRING				Name;		//��������
		TSTRING				Path;		//·��
		TSTRING				MaskPath;	//����ͼ·��
		vector<int>			Bombs;		//ը���б�
		vector<POINT>		IniBombs;	//��ʼը���б�
		vector<SNewBomb>	NewBombs;	//��ը��
		int					Score;		//�÷�
		int					XAdjust;	//�߶�У׼
		bool				Open;		//�ؿ��Ƿ񿪷�
	};

public:
	~CLevelManager();

	void LoadLevelInfoFromFile(TSTRING DrawInfo, TSTRING DataInfo, TSTRING PlayInfo);	//���ļ����عؿ���Ϣ
	void SaveLevelsInfo();	//����ͨ����Ϣ

	int  GetNowLevel() { return m_iCurLevel; }	//��ȡ��ǰ�ؿ���
	void SetNowLevelScore(int score) { if(score > m_LevelsInfo[m_iCurLevel - 1]->Score)m_LevelsInfo[m_iCurLevel - 1]->Score = score; }
	void UnlockNextLevel() { if(m_iCurLevel < 120) m_LevelsInfo[m_iCurLevel]->Open = true; }
	bool AnimIsEnd();	//���ض����Ƿ��Ѿ�����
	void LoadLevel(int level);	//���ع�
	void UnloadeNowLevel();	//ж�ص�ǰ��
	void SendBombInfo(D3DXVECTOR2 pos);	//���ͱ�ը��Ϣ(��������(��Ļ����))
	void ReloadLevel();	//���¼��ص�ǰ��
	bool LevelIsOpen(int level);	//�ؿ��Ƿ���
	int  LevelScore(int level);	//�ؿ��÷�
	void SetShooter(CBombShooter * shooter) { m_pShooter = shooter; }
	bool IsCollision(D3DXVECTOR2 pos)	//��ȡĳһ���Ƿ���ײ
	{
			if (!PtInRect(&m_drawRect, { (LONG)pos.x,(LONG)pos.y }))
				return false;
			pos -= m_vecSculPos;	//ת��Ϊԭ���������
			pos.x /= 0.52;
			pos.y /= 0.52;
			if (pos.x < 0) return false;
			if (pos.y < 0) return false;
			if (pos.x >= m_pSculpture->GetWidth()) return false;
			if (pos.y >= m_pSculpture->GetHeight()) return false;
			return m_pSculpture->GetBlendValue({ (LONG)pos.x,(LONG)pos.y });
	}

	void Render();	//����
	void Update();	//����

	void AddStaticBomb(CStaticBomb * bomb);		//��Ӿ�̬ը��
	void DrawInSculpture(CTexture * texture, D3DXVECTOR2 pos, float xScalling, float yScalling);
protected:
private:
	void ComputeRect(RECT * destRc,RECT * srcRc, D3DXVECTOR2 pos, CTexture * texture, float destScalling, float srcScalling)
	{
		float destToSrc = srcScalling / destScalling;	//��Դ��Ŀ�����������
		float srcToDest = destScalling / srcScalling;	//��Ŀ�굽Դ����������
		SIZE size = { texture->GetWidth(),texture->GetHeight() };	//��ȡ����Ŀ��
		destRc->left = destRc->top = 0;
		destRc->right = size.cx;
		destRc->bottom = size.cy;
		//�����ڱ���ͼ�ϵ�rc
		pos -= m_vecSculPos;
		pos /= m_xScalling;
		pos.x -= (float)size.cx * destToSrc * 0.5;
		pos.y -= (float)size.cx * destToSrc * 0.5;
		srcRc->left = pos.x;
		srcRc->top = pos.y;
		srcRc->right = pos.x + (float)size.cx * destToSrc;
		srcRc->bottom = pos.y + (float)size.cy *destToSrc;
		//ɾȥԽ�粿��
		size = { m_pSculpture->GetWidth(),m_pSculpture->GetHeight() };
		if (srcRc->left < 0)
		{
			destRc->left -= srcRc->left * srcToDest;
			srcRc->left = 0;
		}
		if (srcRc->top < 0)
		{
			destRc->top -= srcRc->top * srcToDest;
			srcRc->top = 0;
		}
		if (srcRc->right > size.cx)
		{
			destRc->right -= (srcRc->right - size.cx) * srcToDest;
			srcRc->right = size.cx;
		}
		if (srcRc->bottom > size.cy)
		{
			destRc->bottom -= (srcRc->bottom - size.cy) * srcToDest;
			srcRc->bottom = size.cy;
		}
	}
private:
	LPDIRECT3DDEVICE9		m_pDevice;	//�豸ָ��
	LPD3DXSPRITE	m_pSprite;		//����
	CSprite	*		m_pBackground;	//����
	CTexture *		m_pSculpture;	//����ͼƬ
	CTexture *		m_pMask;		//����ͼ
	CTexture *		m_pBomb;		//��ը��Χͼ
	TSTRING			m_strSaveFile;	//������Ϣ���ļ�
	
	vector<SLevelInfo *>		m_LevelsInfo;	//�ؿ���Ϣ
	list<CNewBombs *>			m_listNewBombs;	//�³��ֵ�ը���б�
	int				m_iCurLevel;	//��ǰ��
	int				m_iLevelNum;	//�ؿ���
	INT				m_iMaxPicCount;	//��������
	INT				m_iCurPixCount;	//���ؼ���
	RECT			m_drawRect;		//��������ľ���
	CBombShooter *	m_pShooter;		//�����
	BOOL			m_bSendPre;		//�Ƿ��ͽ���
	
	D3DXMATRIX		m_mat;			//�任����
	D3DXVECTOR2		m_vecSculPos;	//����λ��
	float			m_xScalling;	//x������
	float			m_yScalling;	//y������
	float			m_xBombScal;	//��ըͼ��X����
	float			m_yBombScal;	//��ըͼ��Y����

	list<CStaticBomb *>		m_listBomb;		//��̬ը�����б�
};

