
#pragma once

class CAnimation;

class CSprite :
	public IAnimable
{
public:
	enum ECENTER_MOD {
		ECEN_MIDDLE = 1,
		ECEN_CENTER = 1 << 1,
		ECEN_LEFT = 1 << 2,
		ECEN_RIGHT = 1 << 3,
		ECEN_TOP = 1 << 4,
		ECEN_BOTTOM = 1 << 5
	};

public:
	CSprite();
	CSprite(const CSprite & sprite);
	virtual ~CSprite();

	virtual void Init() {}		//��ʼ������
	void Init(TSTRING animationName);	//��ʼ�����鲢���һ��������Դ
	void Init(vector<TSTRING> &animationName);	//��ʼ�����鲢���һ�鶯����Դ

	virtual void Draw(bool updateMatrix = true);//���� �麯��
	virtual void Update();//���� �麯��
	virtual void ShowFrame(TSTRING animationName);	//��ʾĳ�������ĵ�ǰ֡

	virtual void LoopPlayAnimation(TSTRING animationName, int speed = 1);//ѭ�����Ŷ���
	void ResetMonoPlay();	//���õ��β��Ŷ���
	virtual void PlayAnimation(TSTRING animationName);//���β��Ŷ���
	bool isEnd() { return m_bMonoPlayFinish; }	//�����Ƿ����

	void SetCurAnimation(TSTRING animation);	//���õ�ǰ����
	void SetCurFrame(int frame);	//���õ�ǰ�����ĵ�ǰ֡��

	void SetSrcPos(D3DXVECTOR2 srcPos);		//����ԭͼλ��
	void SetDesPos(D3DXVECTOR2 desPos);		//������Ļ����λ��
	void SetWorldPos(D3DXVECTOR2 worldPos);	//���þ�����������

	D3DXVECTOR2 GetSrcPos() { return m_sSrcPos; }	//����ԭͼ����
	D3DXVECTOR2 GetDesPos() { return m_sDesPos; }	//����Ŀ������
	D3DXVECTOR2 GetWorldPos() { return m_sWorldPos; }//����Ŀ����������

	int  GetLayer() { return m_nLayer; }//���ز㼶
	void SetLayer(int layer) { m_nLayer = layer; }	//���ò㼶
	void SetShowWidth(int showWidth);	//������ʾ��
	void SetShowHeight(int showHeight);	//������ʾ��
	int GetShowWidth() { return m_nShowWidth; }		//������ʾ��
	int GetShowHeight() { return m_nShowHeight; }	//������ʾ��
	void SetSrcRect(const RECT & rect) { m_sSrcRect = rect; }	//������������
	void ResetSrcRect() { ZeroMemory(&m_sSrcRect, sizeof(RECT)); }	//������������
	SIZE getDrawSize();	//��ȡ���ƺ�Ĵ�С
	RECT getDrawRect();	//��ȡ���ƺ�ľ���

	void SetCenter(D3DXVECTOR3 center);//����ê��
	void SetCenter(int centerMod);//����ê��
	D3DXVECTOR3 GetCenter() { return m_center; }//���ê��
	void SetTransparent(int transpatent);//����͸����
	int GetTransparent() { return m_nTransparent >> 24; }//���͸����
	void SetShowColor(int showColor);//���ü�ɫ
	int GetShowColor() { return m_nRGBColor; }//��ü�ɫ

	void UpdateMatrix();  //���¾���
	void SetMatrix(D3DXMATRIX & mat);	//�����Զ���ľ���
	D3DXMATRIX GetMatrix() { return matMatrix; }	//��ȡ�任����

	void AjustScale(SIZE showSize);	//��ԭͼƬУ׼���̶���С
	void SetXScale(float xScale);	//����X�������
	void SetYScale(float yScale);	//����Y�������
	float GetXScale() { return m_xScale; }	//��ȡX�������
	float GetYScale() { return m_yScale; }	//��ȡY�������
	void SetAngle(float angle); //������ת�Ƕ�
	float GetAngle() { return m_angle; }	//��ȡ��ת�Ƕ�
	
	bool IsVisible() { return m_bIsVisible; }	//���ؾ����Ƿ�ɼ�
	bool IsActive() { return m_bIsActive; }	//���ؾ����Ƿ񼤻�
	void SetVisible(bool visible) { m_bIsVisible = visible; }	//���þ����Ƿ�ɼ�
	void SetActive(bool active) { m_bIsActive = active; }	//���þ����Ƿ񼤻�

protected:
	map<TSTRING, CAnimation*> m_pAnimation;//��Ŷ���������
	TSTRING			m_curAnimation;	//��ǰ��������
	int				m_nCurFrame;	//��ǰ֡��
	LPD3DXSPRITE	m_pSprite;	//ͼƬ����
	int				m_nLayer;	//�㼶

	D3DXVECTOR2		m_sSrcPos;	//ԭͼ����
	D3DXVECTOR2		m_sDesPos;	//Ŀ������
	D3DXVECTOR2		m_sWorldPos;	//��������λ��
	RECT			m_sSrcRect;	//���Ƶ���������

	int m_nShowWidth;	//��ʾ���
	int m_nShowHeight;	//��ʾ�߶�
	int m_nRGBColor;	//��ʾɫ
	unsigned int  m_nTransparent;	//͸����
	D3DXVECTOR3	m_center;//ê��λ��

	D3DXMATRIX	scaleMatrix;	//���ž���
	D3DXMATRIX	rotateMatrix;	//��ת����
	D3DXMATRIX	transMatrix;	//ƽ�ƾ���
	D3DXMATRIX	matMatrix;		//�ۺϾ���
	float m_xScale;	//x������
	float m_yScale;	//y������
	float m_angle;	//��ת�Ƕ�
	bool m_bIsVisible;	//�Ƿ�ɼ�  
	bool m_bIsActive;	//�Ƿ񼤻�

	bool m_bMonoPlayStart;	//���β��Ŷ�����ʼ
	bool m_bMonoPlayFinish;	//���β��Ŷ�������
	int m_nAnimationSpeedCount;	//���������ٶȼ�����
private:
	static LPD3DXSPRITE s_sprite;	//ȫ�־���
	static INT			s_spriteNum;	//��������
};
