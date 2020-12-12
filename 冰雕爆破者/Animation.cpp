#include "stdafx.h"
#include "Texture.h"
#include "ResManager.h"
#include "Animation.h"

CAnimation::CAnimation(TSTRING name)
{
	m_animationName = name;
	m_nCurFrame = 0;
}

CAnimation::~CAnimation()
{
}


void CAnimation::AddTexture(TSTRING name, SIZE size, int offsetX, int offsetY)
{
	CTexture* texture = CResManager::GetInstance()->GetTexture(name);
	//将纹理放入容器
	m_gTexture.push_back(texture->m_pTexture);
	FRAMEDATA frameData;
	frameData.size = size;
	frameData.offsetX = offsetX;
	frameData.offsetY = offsetY;
	m_gTextureSize.push_back(frameData);
}
