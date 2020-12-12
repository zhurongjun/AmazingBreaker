#pragma once

class CMouseListener      //����¼�����
{
public:
	virtual void OnMouseDown(SEvent) = 0;
	virtual void OnMouseUp(SEvent) {}
	virtual void OnMouseMove(SEvent) {}
	virtual void OnMouseWheel(SEvent) {}
};

class CKeyListener                  //�����¼�����
{
public:
	virtual void OnKeyDown(SEvent) = 0;
	virtual void OnKeyUp(SEvent) {}
};