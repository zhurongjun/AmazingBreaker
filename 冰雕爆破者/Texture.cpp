#include "stdafx.h"
#include "Texture.h"
CTexture::CTexture()
{
	m_pTexture = nullptr;
	m_Width = 0;
	m_Height = 0;
	m_colRef = 0xffffff;
}

CTexture::~CTexture()
{
	if (m_pTexture)
		m_pTexture->Release();	//�ͷ�����ͼ���ڴ�
}

void CTexture::SetName(TSTRING name)
{
	this->m_name = name;
}
void CTexture::SetPath(TSTRING path)
{
	this->m_path = path;
}
void CTexture::Init()
{
	LPDIRECT3DDEVICE9 pDev = (LPDIRECT3DDEVICE9)CPlatform::GetInstance()->GetPlatData(_T("pDev"));	//��ȡ�豸ָ��
	//��������ͼ
	if (FAILED(D3DXCreateTextureFromFileEx(pDev, m_path.c_str(), D3DX_FROM_FILE, D3DX_FROM_FILE, D3DX_FROM_FILE, 0,
		D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, m_colRef, 0, 0, &m_pTexture)))
		return;
	//��ȡͼƬ��Ϣ
	D3DSURFACE_DESC desc;
	m_pTexture->GetLevelDesc(0, &desc);
	m_Height = desc.Height;
	m_Width = desc.Width;
}



void CTexture::SetColRef(COLORREF color)
{
	m_colRef = color;
}

void CTexture::SetWidth(WORD width)
{
	m_Width = width;
}

void CTexture::SetHeight(WORD height)
{
	m_Height = height;
}

WORD CTexture::GetHeight()
{
	return m_Height;
}

WORD CTexture::GetWidth()
{
	return m_Width;
}

COLORREF CTexture::GetColRef()
{
	return m_colRef;
}

DWORD CTexture::GetColorValue(POINT pos)
{
	D3DLOCKED_RECT pValue;	//�������ݵĽṹ��
	m_pTexture->LockRect(0, &pValue, NULL, 0);//����Ҫ���Ƶ�����
	DWORD color;
	DWORD *pColor = (DWORD*)pValue.pBits;
	color = *(pColor + pos.x + pos.y*pValue.Pitch / 4); //����ָ���������ص���ɫ
	m_pTexture->UnlockRect(0);
	return color;
}

DWORD CTexture::GetBlendValue(POINT pos)
{
	D3DLOCKED_RECT pValue;	//�������ݵĽṹ��
	m_pTexture->LockRect(0, &pValue, NULL, 0);//����Ҫ���Ƶ�����
	DWORD color;
	DWORD *pColor = (DWORD*)pValue.pBits;
	color = *(pColor + pos.x + pos.y*pValue.Pitch / 4); //����ָ���������ص���ɫ
	m_pTexture->UnlockRect(0);
	color = color >> 24;
	color = color << 24;
	return color;
}

void CTexture::Release()
{
	if (m_pTexture)
		m_pTexture->Release();	//�ͷ�����ͼ���ڴ�
	m_pTexture = nullptr;
}
