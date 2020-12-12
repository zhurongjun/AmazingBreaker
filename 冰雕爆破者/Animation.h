#pragma once

//�洢֡��Ϣ�Ľṹ��
typedef struct FrameData
{
	int offsetX;	//��������Ͻǵ�Xƫ��
	int offsetY;	//��������Ͻǵ�Yƫ��
	SIZE size;		//ͼƬ�Ĵ�С

}FRAMEDATA, *PFRAMEDATA;

class CAnimation
{
public:
	CAnimation(TSTRING name);
	~CAnimation();

	//�������ͼ
	void AddTexture(TSTRING name, SIZE size, int offsetX = 0, int offsetY = 0);


	void SetAnimationName(TSTRING name) { m_animationName = name; }	//���ö���������
	TSTRING GetAnimationName() { return m_animationName; }	//��ȡ����������

	void SetCountFrame(int countFrame) { m_nCountFrame = countFrame; }	//����֡��
	int GetCountFrame() { return m_nCountFrame; }	//��ȡ֡��
	
	int GetCurFrame() { return m_nCurFrame; }	//��ȡ����֡��λ��
	void SetCurFrame(int curFrame) { m_nCurFrame = curFrame; }		//���õ���֡��λ��

	int GetWidth(int curFrame = 0) { return m_gTextureSize[curFrame].size.cx; }	//��ȡ���
	int GetHeight(int curFrame = 0) { return m_gTextureSize[curFrame].size.cy; }//��ȡ�߶�

	int GetOffSetX(int curFrame = 0) { return m_gTextureSize[curFrame].offsetX; }	//��ȡxƫ����
	int GetOffSetY(int curFrame = 0) { return m_gTextureSize[curFrame].offsetY; }	//��ȡyƫ����
	LPDIRECT3DTEXTURE9 GetTexture(int curFrame) { return m_gTexture[curFrame]; }	//��ȡĳһ֡������ͼ

	void SetOffSetX(int x, int curFrame = 0) { m_gTextureSize[curFrame].offsetX = x; }	//����xƫ����
	void SetOffSetY(int y, int curFrame = 0) { m_gTextureSize[curFrame].offsetY = y; }	//����yƫ����


protected:
private:
	TSTRING m_animationName;	//��������
	int		m_nCountFrame;		//��֡��
	int		m_nCurFrame;		//��ǰ֡��
	vector<LPDIRECT3DTEXTURE9> m_gTexture;	//ͼƬ����
	vector<FRAMEDATA> m_gTextureSize;		//����ÿ֡ͼƬ��ʼλ�úʹ�С
};

