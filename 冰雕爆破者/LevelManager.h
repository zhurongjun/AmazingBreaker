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
		POINT	Pos;		//出现位置
		int		ShowTime;	//出现的回合数
		int		Life;		//生命
		int		Type;		//类型
	};
	struct SLevelInfo
	{
		TSTRING				BKG;		//背景名称
		TSTRING				Name;		//冰雕名称
		TSTRING				Path;		//路径
		TSTRING				MaskPath;	//遮罩图路径
		vector<int>			Bombs;		//炸弹列表
		vector<POINT>		IniBombs;	//初始炸弹列表
		vector<SNewBomb>	NewBombs;	//新炸弹
		int					Score;		//得分
		int					XAdjust;	//高度校准
		bool				Open;		//关卡是否开放
	};

public:
	~CLevelManager();

	void LoadLevelInfoFromFile(TSTRING DrawInfo, TSTRING DataInfo, TSTRING PlayInfo);	//从文件加载关卡信息
	void SaveLevelsInfo();	//保存通关信息

	int  GetNowLevel() { return m_iCurLevel; }	//获取当前关卡数
	void SetNowLevelScore(int score) { if(score > m_LevelsInfo[m_iCurLevel - 1]->Score)m_LevelsInfo[m_iCurLevel - 1]->Score = score; }
	void UnlockNextLevel() { if(m_iCurLevel < 120) m_LevelsInfo[m_iCurLevel]->Open = true; }
	bool AnimIsEnd();	//返回动画是否已经结束
	void LoadLevel(int level);	//加载关
	void UnloadeNowLevel();	//卸载当前关
	void SendBombInfo(D3DXVECTOR2 pos);	//传送爆炸信息(基于坐标(屏幕坐标))
	void ReloadLevel();	//重新加载当前关
	bool LevelIsOpen(int level);	//关卡是否开启
	int  LevelScore(int level);	//关卡得分
	void SetShooter(CBombShooter * shooter) { m_pShooter = shooter; }
	bool IsCollision(D3DXVECTOR2 pos)	//获取某一点是否碰撞
	{
			if (!PtInRect(&m_drawRect, { (LONG)pos.x,(LONG)pos.y }))
				return false;
			pos -= m_vecSculPos;	//转换为原点绘制坐标
			pos.x /= 0.52;
			pos.y /= 0.52;
			if (pos.x < 0) return false;
			if (pos.y < 0) return false;
			if (pos.x >= m_pSculpture->GetWidth()) return false;
			if (pos.y >= m_pSculpture->GetHeight()) return false;
			return m_pSculpture->GetBlendValue({ (LONG)pos.x,(LONG)pos.y });
	}

	void Render();	//绘制
	void Update();	//更新

	void AddStaticBomb(CStaticBomb * bomb);		//添加静态炸弹
	void DrawInSculpture(CTexture * texture, D3DXVECTOR2 pos, float xScalling, float yScalling);
protected:
private:
	void ComputeRect(RECT * destRc,RECT * srcRc, D3DXVECTOR2 pos, CTexture * texture, float destScalling, float srcScalling)
	{
		float destToSrc = srcScalling / destScalling;	//从源到目标的缩放因子
		float srcToDest = destScalling / srcScalling;	//从目标到源的缩放因子
		SIZE size = { texture->GetWidth(),texture->GetHeight() };	//获取纹理的宽高
		destRc->left = destRc->top = 0;
		destRc->right = size.cx;
		destRc->bottom = size.cy;
		//计算在冰雕图上的rc
		pos -= m_vecSculPos;
		pos /= m_xScalling;
		pos.x -= (float)size.cx * destToSrc * 0.5;
		pos.y -= (float)size.cx * destToSrc * 0.5;
		srcRc->left = pos.x;
		srcRc->top = pos.y;
		srcRc->right = pos.x + (float)size.cx * destToSrc;
		srcRc->bottom = pos.y + (float)size.cy *destToSrc;
		//删去越界部分
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
	LPDIRECT3DDEVICE9		m_pDevice;	//设备指针
	LPD3DXSPRITE	m_pSprite;		//精灵
	CSprite	*		m_pBackground;	//背景
	CTexture *		m_pSculpture;	//冰雕图片
	CTexture *		m_pMask;		//遮罩图
	CTexture *		m_pBomb;		//爆炸范围图
	TSTRING			m_strSaveFile;	//保存信息的文件
	
	vector<SLevelInfo *>		m_LevelsInfo;	//关卡信息
	list<CNewBombs *>			m_listNewBombs;	//新出现的炸弹列表
	int				m_iCurLevel;	//当前关
	int				m_iLevelNum;	//关卡数
	INT				m_iMaxPicCount;	//像素总数
	INT				m_iCurPixCount;	//像素计数
	RECT			m_drawRect;		//绘制区域的矩形
	CBombShooter *	m_pShooter;		//射击者
	BOOL			m_bSendPre;		//是否发送进度
	
	D3DXMATRIX		m_mat;			//变换矩阵
	D3DXVECTOR2		m_vecSculPos;	//冰雕位置
	float			m_xScalling;	//x轴缩放
	float			m_yScalling;	//y轴缩放
	float			m_xBombScal;	//爆炸图的X缩放
	float			m_yBombScal;	//爆炸图的Y缩放

	list<CStaticBomb *>		m_listBomb;		//静态炸弹的列表
};

