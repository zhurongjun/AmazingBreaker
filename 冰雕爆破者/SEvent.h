#pragma once
enum EVENT_TYPE {       //�¼�����
	eEVENT_NONE = 0,	//û���¼�
	eEVENT_KEY = 1,		//�����¼�
	eEVENT_MOUSE = 2	//����¼�
};

enum EVENT_INPUT_MOUSE {                            //����¼�����
	eMOUSE_PRESS_LDOWN = 0,	//�������
	eMOUSE_PRESS_RDOWN = 1,	//�Ҽ�����
	eMOUSE_PRESS_MDOWN = 2,	//�м�����
	eMOUSE_PRESS_LUP = 3,	//�������
	eMOUSE_PRESS_RUP = 4,	//�Ҽ�����
	eMOUSE_PRESS_MUP = 5,	//�м�����
	eMOUSE_MOVE = 6,		//�ƶ�
	eMOUSE_WHEEL = 7		//����
};

struct SMouseInput {                                      //����¼�����
	int x;	//���x
	int y;	//���y
	EVENT_INPUT_MOUSE mouseEvent;	//����¼�����
};

struct SKeyInput {                     //�����¼�����
	char key;	//���µļ�
	bool pressedDown;	//�Ƿ���
};

struct SEvent {                                         //�¼�
	EVENT_TYPE eventType;	//�¼�����
	union
	{
		SKeyInput keyInput;	//�����¼�
		SMouseInput mouseInput;	//����¼�
	};
};