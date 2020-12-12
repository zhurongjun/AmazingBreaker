#pragma once
enum EVENT_TYPE {       //事件类型
	eEVENT_NONE = 0,	//没有事件
	eEVENT_KEY = 1,		//键盘事件
	eEVENT_MOUSE = 2	//鼠标事件
};

enum EVENT_INPUT_MOUSE {                            //鼠标事件类型
	eMOUSE_PRESS_LDOWN = 0,	//左键落下
	eMOUSE_PRESS_RDOWN = 1,	//右键落下
	eMOUSE_PRESS_MDOWN = 2,	//中键落下
	eMOUSE_PRESS_LUP = 3,	//左键弹起
	eMOUSE_PRESS_RUP = 4,	//右键弹起
	eMOUSE_PRESS_MUP = 5,	//中键落下
	eMOUSE_MOVE = 6,		//移动
	eMOUSE_WHEEL = 7		//滚轮
};

struct SMouseInput {                                      //鼠标事件输入
	int x;	//鼠标x
	int y;	//鼠标y
	EVENT_INPUT_MOUSE mouseEvent;	//鼠标事件类型
};

struct SKeyInput {                     //键盘事件输入
	char key;	//按下的键
	bool pressedDown;	//是否按下
};

struct SEvent {                                         //事件
	EVENT_TYPE eventType;	//事件类型
	union
	{
		SKeyInput keyInput;	//键盘事件
		SMouseInput mouseInput;	//鼠标事件
	};
};