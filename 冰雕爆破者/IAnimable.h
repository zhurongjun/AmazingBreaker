#pragma once
#include <d3dx9.h>
//��Ч�ӿ�
struct IAnimable
{
	virtual ~IAnimable() {}
	virtual void SetTransparent(int transpatent) = 0;//����͸����
	virtual int GetTransparent() = 0;//���͸����
	virtual void SetDesPos(D3DXVECTOR2 desPos) = 0;		//������Ļ����λ��
	virtual D3DXVECTOR2 GetDesPos() = 0;	//����Ŀ������
	virtual void Draw(bool updateMatrix = true) = 0;	//����
	virtual void SetXScale(float xScale) = 0;	//����X������
	virtual void SetYScale(float yScale) = 0;	//����Y������
	virtual float GetXScale() = 0;	//��ȡX�������
	virtual float GetYScale() = 0;	//��ȡY�������
	virtual void SetAngle(float angle) = 0; //������ת�Ƕ�
	virtual float GetAngle() = 0;	//��ȡ��ת�Ƕ�

};

