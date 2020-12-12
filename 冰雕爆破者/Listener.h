#pragma once

class CMouseListener      //鼠标事件监听
{
public:
	virtual void OnMouseDown(SEvent) = 0;
	virtual void OnMouseUp(SEvent) {}
	virtual void OnMouseMove(SEvent) {}
	virtual void OnMouseWheel(SEvent) {}
};

class CKeyListener                  //键盘事件监听
{
public:
	virtual void OnKeyDown(SEvent) = 0;
	virtual void OnKeyUp(SEvent) {}
};