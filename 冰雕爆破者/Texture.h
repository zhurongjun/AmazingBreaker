#pragma once

//�����࣬���浥�ŵ�֡��ͼƬ����Ϊ�ײ����Դ��
class CTexture
{
public:
	CTexture();
	~CTexture();

	void	Init();		//��ʼ��
	void	SetColRef(COLORREF);//���ù��˵�͸��ɫ
	COLORREF	GetColRef();	//��ȡ���˵�͸��ɫ

	void	SetWidth(WORD);		//����ͼƬ�Ŀ�
	void	SetHeight(WORD);	//����ͼƬ�ĸ�
	WORD	GetWidth();		//��ȡͼƬ�Ŀ��
	WORD	GetHeight();	//��ȡͼƬ�ĸ߶�

	void SetName(TSTRING name);		//����ͼƬ������
	void SetPath(TSTRING path);		//����ͼƬ��·��
	DWORD	GetColorValue(POINT);	//��ȡĳһ�����ϵ���ɫ
	DWORD	GetBlendValue(POINT);	//��ȡĳһ���͸����

	LPDIRECT3DTEXTURE9 GetTexture() { return m_pTexture; }	//��ȡ����
	void	Release();		//�ͷ�����

public:
	LPDIRECT3DTEXTURE9	m_pTexture;	//����ָ��
	WORD		m_Width;	//���
	WORD		m_Height;	//�߶�
	COLORREF	m_colRef;	//���˵�͸��ɫ
	TSTRING		m_name;		//ͼƬ������
	TSTRING		m_path;		//ͼƬ��·��
};

