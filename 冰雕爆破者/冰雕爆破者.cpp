// 冰雕爆破者.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "PlayScene.h"
#include "LoginScene.h"
#include "SelectScene.h"
#include "WaitScene.h"
#include "MenuScene.h"
#include "FailScene.h"
#include "WinScene.h"


bool init(HINSTANCE hInstance);

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	init(hInstance);
	UINT preTime = GetTickCount();
	MSG msg = {};

	while (1)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			continue;
		}
		if (GetTickCount() - preTime >= 16)
		{
			preTime = GetTickCount();
			CGameManager::GetInstance()->Render();
			CGameManager::GetInstance()->Update();
			continue;
		}
		Sleep(1);
	}
	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
	case WM_MOUSEMOVE:
	case WM_MOUSEWHEEL:
		CGameManager::GetInstance()->WinProc(message, wParam, lParam);
		break;
	case WM_DESTROY:
		delete CGameManager::GetInstance();
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

bool init(HINSTANCE hInstance)
{
	srand(GetTickCount());
	WNDCLASS g_MainWinClass;
	//初始化窗口
	g_MainWinClass.cbClsExtra = g_MainWinClass.cbWndExtra = 0;
	g_MainWinClass.hbrBackground = HBRUSH(GetStockObject(WHITE_BRUSH));
	g_MainWinClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	g_MainWinClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	g_MainWinClass.hInstance = hInstance;
	g_MainWinClass.lpfnWndProc = WNDPROC(WndProc);
	g_MainWinClass.lpszClassName = _T("MyGame");
	g_MainWinClass.lpszMenuName = nullptr;
	g_MainWinClass.style = 0;
	RegisterClass(&g_MainWinClass);
	POINT winPos;
	static SIZE Winsize = { 480,720 };
	winPos.x = GetSystemMetrics(SM_CXSCREEN);
	winPos.y = GetSystemMetrics(SM_CYSCREEN);
	winPos.x = winPos.x / 2 - Winsize.cx / 2;
	winPos.y = winPos.y / 2 - Winsize.cy / 2;
	RECT realRect = { winPos.x,winPos.y,winPos.x + Winsize.cx,winPos.y + Winsize.cy };
	CPlatform::GetInstance()->AddPlatData((_T("winsize")), &Winsize);
	AdjustWindowRect(&realRect, WS_OVERLAPPEDWINDOW, false);
	HWND hwnd;
	hwnd = CreateWindow(_T("MyGame"), _T("冰雕爆破者"), WS_OVERLAPPED | WS_SYSMENU, realRect.left, realRect.top, realRect.right - realRect.left, realRect.bottom - realRect.top, nullptr, nullptr, hInstance, nullptr);
	if (!hwnd)
	{
		MessageBox(nullptr, _T("窗口创建失败!"), _T("警告"), MB_OK);
		return false;
	}
	ShowWindow(hwnd, SW_NORMAL);
	UpdateWindow(hwnd);
	//向平台添加信息
	CPlatform::GetInstance()->AddPlatData(_T("hwnd"), hwnd);
	CPlatform::GetInstance()->AddPlatData(_T("pInstance"), hInstance);

	//初始化游戏
	CGameManager::GetInstance()->Init(hwnd);

	//加载资源
	CResManager::GetInstance()->LoadResFromFile(_T("./data/Res.xml"));

	//加载动效
	CAnimManager::GetInstance()->LoadAnimFromFile(_T("./data/Anim.xml"));

	//加载音乐
	CAudio4Bass::GetInstance()->Initialize(hwnd);
	CAudio4Bass::GetInstance()->LoadFileFromXml("./data/Sound.xml");
	CAudio4Bass::GetInstance()->SetVolume(0.5);
	CAudio4Bass::GetInstance()->ChannelPlay(_T("bgm1"), 1);

	//加载关卡
	CLevelManager::GetInstance()->LoadLevelInfoFromFile(_T("./data/levelsLayout.xml"),_T("./data/levelsInfo.xml"),_T("./data/levelsData.xml"));

	//初始化场景
	CGameScene * tempScene;	//中间变量
	tempScene = new CPlayScene;
	tempScene->Init();
	CGameManager::GetInstance()->addScene(_T("游戏界面"), tempScene);

	tempScene = new CLoginScene;
	tempScene->Init();
	CGameManager::GetInstance()->addScene(_T("进入界面"), tempScene);

	tempScene = new CSelectScene;
	tempScene->Init();
	CGameManager::GetInstance()->addScene(_T("选关界面"), tempScene);

	tempScene = new CWaitScene;
	tempScene->Init();
	CGameManager::GetInstance()->addScene(_T("等待界面"), tempScene);

	tempScene = new CMenuScene;
	tempScene->Init();
	CGameManager::GetInstance()->addScene(_T("菜单界面"), tempScene);

	tempScene = new CFailScene;
	tempScene->Init();
	CGameManager::GetInstance()->addScene(_T("失败界面"), tempScene);

	tempScene = new CWinScene;
	tempScene->Init();
	CGameManager::GetInstance()->addScene(_T("胜利界面"), tempScene);

	CGameManager::GetInstance()->setNowScene(_T("进入界面"));
	//CGameManager::GetInstance()->setNowScene(_T("胜利界面"));
	return true;
}
