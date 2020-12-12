#pragma once
#include <d3dx9.h>
//动效接口
struct IAnimable
{
	virtual ~IAnimable() {}
	virtual void SetTransparent(int transpatent) = 0;//设置透明度
	virtual int GetTransparent() = 0;//获得透明度
	virtual void SetDesPos(D3DXVECTOR2 desPos) = 0;		//设置屏幕绘制位置
	virtual D3DXVECTOR2 GetDesPos() = 0;	//返回目标坐标
	virtual void Draw(bool updateMatrix = true) = 0;	//绘制
	virtual void SetXScale(float xScale) = 0;	//设置X轴缩放
	virtual void SetYScale(float yScale) = 0;	//设置Y轴缩放
	virtual float GetXScale() = 0;	//获取X轴的缩放
	virtual float GetYScale() = 0;	//获取Y轴的缩放
	virtual void SetAngle(float angle) = 0; //设置旋转角度
	virtual float GetAngle() = 0;	//获取旋转角度

};

