#pragma once

class CAnimHander
{
public:
	enum EANIM {
		eANIM_SCALINGX = 0,	//x����
		eANIM_SCALINGY = 1,	//y����
		eANIM_POSX = 2,	//xλ��
		eANIM_POSY = 3,	//yλ��
		eANIM_ROTATE = 4,	//��ת
		eANIM_BLEND = 5,	//͸����
		eANIM_LOOP	//���⶯Ч�ڵ�(ѭ��)
	};

public:
	CAnimHander();
	~CAnimHander();
	
	void init(TiXmlElement * element);
	void SetMainTarget(IAnimable * target);		//����������
	IAnimable * GetMainTarget() { return m_pMainTarget; }	//��ȡ������
	void addFixTarget(TSTRING name, IAnimable * target);	//��ӹ�̬��Ч

	void UpdateFixPos();	//���¾�̬�����λ��
	void LoopRender();	//�ظ�����
	void Render(bool renderFix = true);	//���β���
	void Update();	//����
	CAnimHander * Copy(bool copyFix = true);	//����һ��һ���Ķ���
	void Release();	//�ͷ�
	void Reset();	//����
	void EndDraw() { m_bIsEnd = TRUE; }
	bool IsEnd() { return m_bIsEnd; }	//�Ƿ����
protected:
private:
	IAnimable *					m_pMainTarget;	//��Ŀ��
	map<TSTRING, IAnimable *> 	m_mapFixTarget;	//�̶�Ŀ��
	vector<CAnimHander *>		m_vecChildAnim;	//�Ӷ�Ч
	TSTRING		m_sTarget;	//���ö���
	INT			m_iAllLoop;	//ѭ���ܴ���
	INT			m_inowLoop;	//��ǰ��ѭ������
	INT			m_iDelay;	//�ӳٴ���
	INT			m_iCurCount;//��ǰ�ӳټ���
	double		m_iParam;	//����
	BOOL		m_bIsSet;	//�Ƿ�������
	BOOL		m_bIsEnd;	//��Ч�Ƿ����
	EANIM		m_eType;	//��ǩ����
};

