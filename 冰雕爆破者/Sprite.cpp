#include "stdafx.h"
#include "Animation.h"
#include "ResManager.h"
#include "Sprite.h"

LPD3DXSPRITE CSprite::s_sprite = nullptr;
INT	CSprite::s_spriteNum = 0;

CSprite::CSprite()
{
	if (!s_sprite)
	{
		LPDIRECT3DDEVICE9 pDevice = nullptr;
		pDevice = (LPDIRECT3DDEVICE9)CPlatform::GetInstance()->GetPlatData(_T("pDev"));	//获取设备
		if (pDevice == nullptr)
		{
			TSTRING str = _T(" pDevice创建错误$$$$$$$$$$ ");
			OutputDebugString(str.c_str());
			MessageBox(nullptr, str.c_str(), nullptr, ERROR);
			return;

		}
		D3DXCreateSprite(pDevice, &s_sprite);	//创建精灵
		m_pSprite = s_sprite;
	}
	else
		m_pSprite = s_sprite;
	
	
	if (m_pSprite == nullptr)
	{
		TSTRING str = _T(" 精灵创建错误$$$$$$$$$$");
		OutputDebugString(str.c_str());
		MessageBox(nullptr, str.c_str(), nullptr, ERROR);
		return;
	}
	m_nCurFrame = 0;
	m_sDesPos = { 0,0 };
	m_sWorldPos = { 0,0 };
	m_nLayer = 0;
	m_nShowWidth = 100;
	m_nShowHeight = 100;
	m_nTransparent = 0xFF000000;
	m_nRGBColor = 0x00FFFFFF;
	m_center = { 0,0,0 };
	m_xScale = 1;
	m_yScale = 1;
	m_angle = 0;
	m_nAnimationSpeedCount = 0;
	m_curAnimation = _T("");
	m_bIsVisible = true;
	m_bIsActive = true;
	m_bMonoPlayStart = false;
	m_bMonoPlayFinish = false;
	ZeroMemory(&m_sSrcRect, sizeof(RECT));	//初始化增量矩形
	D3DXMatrixIdentity(&matMatrix);	//初始化矩阵
	m_nTransparent = 255 << 24;
	s_spriteNum++;
}

CSprite::CSprite(const CSprite & sprite)
{
	m_pAnimation = sprite.m_pAnimation;//存放动画的容器
	m_curAnimation = sprite.m_curAnimation;	//当前动画名称
	m_nCurFrame = sprite.m_nCurFrame;	//当前帧数
	m_pSprite = s_sprite;
	m_nLayer = sprite.m_nLayer;	//层级

	m_sSrcPos = sprite.m_sSrcPos;	//原图坐标
	m_sDesPos = sprite.m_sDesPos;	//目标坐标
	m_sWorldPos = sprite.m_sWorldPos;	//世界坐标位置
	m_sSrcRect = sprite.m_sSrcRect;	//绘制的增量矩形

	m_nShowWidth = sprite.m_nShowHeight;	//显示宽度
	m_nShowHeight = sprite.m_nShowHeight;	//显示高度
	m_nTransparent = sprite.m_nTransparent;	//透明度
	m_nRGBColor = sprite.m_nRGBColor;	//显示色
	m_center = sprite.m_center;//锚点位置

	scaleMatrix = sprite.scaleMatrix;	//缩放矩阵
	rotateMatrix = sprite.rotateMatrix;	//旋转矩阵
	transMatrix = sprite.transMatrix;	//平移矩阵
	matMatrix = sprite.matMatrix;		//综合矩阵
	m_xScale = sprite.m_xScale;	//x轴缩放
	m_yScale = sprite.m_yScale;	//y轴缩放
	m_angle = sprite.m_angle;	//旋转角度
	m_bIsVisible = true;	//是否可见  
	m_bIsActive = true;	//是否激活

	m_bMonoPlayStart  = false;	//单次播放动画开始
	m_bMonoPlayFinish = false;	//单次播放动画结束
	m_nAnimationSpeedCount = 0;	//动画播放速度计数器
	s_spriteNum++;
}

CSprite::~CSprite()
{
	s_spriteNum--;
	if (!s_spriteNum)
		m_pSprite->Release();
}

void CSprite::Draw(bool updateMatrix)
{
	if (m_bIsVisible == true)
	{
		if (updateMatrix)
			this->UpdateMatrix();
		ShowFrame(m_curAnimation);
	}

}

void CSprite::Update()
{
	m_nCurFrame++;
	if (m_nCurFrame >= m_pAnimation[m_curAnimation]->GetCountFrame() && !m_bMonoPlayFinish)
	{
		m_nCurFrame = 0;
		m_bMonoPlayFinish = true;
	}
	//更新矩阵
	UpdateMatrix();
}

void CSprite::ShowFrame(TSTRING animationName)
{
	//确定显示图片矩形
	m_nShowWidth = m_pAnimation[animationName]->GetWidth(m_nCurFrame);
	m_nShowHeight = m_pAnimation[animationName]->GetHeight(m_nCurFrame);
	int srcPosX = m_pAnimation[animationName]->GetOffSetX(m_nCurFrame);
	int srcPosY = m_pAnimation[animationName]->GetOffSetY(m_nCurFrame);
	RECT rect = { srcPosX, srcPosY, srcPosX + m_nShowWidth, srcPosY + m_nShowHeight };
	
	//写入增量矩形
	if (m_sSrcRect.left || m_sSrcRect.right || m_sSrcRect.top || m_sSrcRect.bottom)
	{
		rect.right = rect.left + m_sSrcRect.right;
		rect.bottom = rect.top + m_sSrcRect.bottom;
		rect.left += m_sSrcRect.left;
		rect.top += m_sSrcRect.top;
	}

	//开始绘制
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	//播放当前帧
	m_pSprite->Draw(m_pAnimation[animationName]->GetTexture(m_nCurFrame), &rect, &m_center, NULL, m_nTransparent | m_nRGBColor);

	//结束绘制
	m_pSprite->End();

}

void CSprite::LoopPlayAnimation(TSTRING animationName, int speed)
{
	if (m_bIsVisible == true)
	{
		m_nAnimationSpeedCount++;
		if (m_nAnimationSpeedCount%speed == 0)
		{
			m_nCurFrame++;
			if (m_nCurFrame >= m_pAnimation[animationName]->GetCountFrame())
			{
				m_nCurFrame = 0;
			}
			m_nAnimationSpeedCount = 0;
		}
		ShowFrame(animationName);
	}
}

void CSprite::ResetMonoPlay()
{
	m_nCurFrame = 0;
	m_bMonoPlayStart = false;
	m_bMonoPlayFinish = false;
}

void CSprite::PlayAnimation(TSTRING animationName)
{
	if (m_bIsVisible == true)
	{
		if (m_bMonoPlayStart == false)
		{
			m_nCurFrame = 0;
			m_bMonoPlayStart = true;
			m_bMonoPlayFinish = false;
		}
		if (m_bMonoPlayStart == true && m_bMonoPlayFinish == false)
		{
			m_nCurFrame++;
			if (m_nCurFrame >= m_pAnimation[animationName]->GetCountFrame())
			{
				m_bMonoPlayFinish = true;
				m_nCurFrame = m_pAnimation[animationName]->GetCountFrame() - 1;
			}
		}
		ShowFrame(animationName);
	}
}


void CSprite::Init(TSTRING animationName)
{
	m_pAnimation[animationName] = CResManager::GetInstance()->GetAnimation(animationName);
	m_curAnimation = animationName;
	m_nShowHeight = m_pAnimation[animationName]->GetHeight();
	m_nShowWidth = m_pAnimation[animationName]->GetWidth();
}

void CSprite::Init(vector<TSTRING> &animationName)
{
	for (int i = 0; i < animationName.size(); i++)
	{
		m_pAnimation[animationName[i]] = CResManager::GetInstance()->GetAnimation(animationName[i]);
	}
	m_curAnimation = animationName[0];
	m_nShowHeight = m_pAnimation[animationName[0]]->GetHeight();
	m_nShowWidth = m_pAnimation[animationName[0]]->GetWidth();
}

void CSprite::SetCurAnimation(TSTRING animation)
{
	m_curAnimation = animation;
}

void CSprite::SetCurFrame(int frame)
{
	m_nCurFrame = frame;
}

void CSprite::SetSrcPos(D3DXVECTOR2 desPos)
{
	m_sSrcPos = desPos;
}
void CSprite::SetDesPos(D3DXVECTOR2 desPos)
{
	m_sDesPos = desPos;
}

void CSprite::SetWorldPos(D3DXVECTOR2 worldPos)
{
	m_sWorldPos = worldPos;
}

void CSprite::SetShowWidth(int showWidth)
{
	m_nShowWidth = showWidth;
}

void CSprite::SetShowHeight(int showHeight)
{
	m_nShowHeight = showHeight;
}


SIZE CSprite::getDrawSize()
{
	return SIZE{ abs((long)(m_nShowWidth * m_xScale)), abs((long)(m_nShowHeight*m_yScale)) };
}

RECT CSprite::getDrawRect()
{
	RECT afterDraw;
	afterDraw.left = m_sDesPos.x - (long)m_center.x * m_xScale;
	afterDraw.top = m_sDesPos.y - (long)m_center.y * m_yScale;
	afterDraw.right = afterDraw.left + (long)(m_nShowWidth * m_xScale);
	afterDraw.bottom = afterDraw.top + (long)(m_nShowHeight*m_yScale);
	return afterDraw;
}

void CSprite::SetCenter(D3DXVECTOR3 center)
{
	m_center = center;
}

void CSprite::SetCenter(int centerMod)
{
	//确定原图片大小
	int Width = m_pAnimation[m_curAnimation]->GetWidth(m_nCurFrame);
	int Height = m_pAnimation[m_curAnimation]->GetHeight(m_nCurFrame);

	//如果有增量矩形那么就使用增量矩形的大小
	if (m_sSrcRect.left || m_sSrcRect.right || m_sSrcRect.top || m_sSrcRect.bottom)
	{
		Width = m_sSrcRect.right - m_sSrcRect.left;
		Height = m_sSrcRect.bottom - m_sSrcRect.top;
	}

	//确定中心的宽度值
	if (centerMod & ECEN_LEFT)
		m_center.x = 0;
	if (centerMod & ECEN_CENTER)
		m_center.x = Width / 2;
	if (centerMod & ECEN_RIGHT)
		m_center.x = Width;

	//确定中心的高度值
	if (centerMod & ECEN_TOP)
		m_center.y = 0;
	if (centerMod & ECEN_MIDDLE)
		m_center.y = Height / 2;
	if (centerMod & ECEN_BOTTOM)
		m_center.y = Height;

}

void CSprite::SetTransparent(int transpatent)
{
	if (transpatent > 255)transpatent = 255;
	m_nTransparent = transpatent << 24;
}

void CSprite::SetShowColor(int showColor)
{
	m_nRGBColor = showColor;
}


void CSprite::UpdateMatrix()
{
	//放缩图片
	D3DXMatrixScaling(&scaleMatrix, m_xScale, m_yScale, 0);
	//旋转图片
	D3DXMatrixRotationZ(&rotateMatrix, m_angle);
	// 平移图片到我们的指定位置
	D3DXMatrixTranslation(&transMatrix, m_sDesPos.x, m_sDesPos.y, 0);
	//综合矩阵
	matMatrix = scaleMatrix * rotateMatrix*transMatrix;
	m_pSprite->SetTransform(&matMatrix);
}

void CSprite::SetMatrix(D3DXMATRIX & mat)
{
	matMatrix = mat;
	m_pSprite->SetTransform(&matMatrix);
}

void CSprite::AjustScale(SIZE showSize)
{
	//确定原图片大小
	int Width = m_pAnimation[m_curAnimation]->GetWidth(m_nCurFrame);
	int Height = m_pAnimation[m_curAnimation]->GetHeight(m_nCurFrame);

	//如果有增量矩形那么就使用增量矩形的大小
	if (m_sSrcRect.left || m_sSrcRect.right || m_sSrcRect.top || m_sSrcRect.bottom)
	{
		Width = m_sSrcRect.right - m_sSrcRect.left;
		Height = m_sSrcRect.bottom - m_sSrcRect.top;
	}

	//计算出缩放
	m_xScale = (float)showSize.cx / Width;
	m_yScale = (float)showSize.cy / Height;
}

void CSprite::SetXScale(float xScale)
{
	m_xScale = xScale;
}

void CSprite::SetYScale(float yScale)
{
	m_yScale = yScale;
}

void CSprite::SetAngle(float angle)
{
	m_angle = angle;
}
