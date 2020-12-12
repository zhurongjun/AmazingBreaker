#include "stdafx.h"
#include "GameScene.h"
#include "GameManager.h"


CGameManager::CGameManager()
{
}


CGameManager::~CGameManager()
{
	CAudio4Bass::GetInstance()->Release();
	delete CLevelManager::GetInstance();
	delete CAnimManager::GetInstance();
	delete CPlatform::GetInstance();
	delete CAudio4Bass::GetInstance();
	delete CInputManager::GetInstance();
	delete CResManager::GetInstance();

	for (auto it = m_mapScene.begin(); it != m_mapScene.end(); it++)
	{
		delete it->second;
	}
}

void CGameManager::Init(HWND & hwnd)
{
	//初始化DX
	LPDIRECT3DDEVICE9 g_pD3dDevice;
	LPDIRECT3D9 g_pD3dObj;
	g_pD3dObj = Direct3DCreate9(D3D_SDK_VERSION);
	D3DPRESENT_PARAMETERS d3dpp = {};
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed = true;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	g_pD3dObj->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &g_pD3dDevice);
	//向平台添加信息
	CPlatform::GetInstance()->AddPlatData(_T("pObj"), g_pD3dObj);
	CPlatform::GetInstance()->AddPlatData(_T("pDev"), g_pD3dDevice);
}

void CGameManager::Render()
{
	LPDIRECT3DDEVICE9 pDev = (LPDIRECT3DDEVICE9)CPlatform::GetInstance()->GetPlatData(_T("pDev"));
	pDev->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1, 0);
	pDev->BeginScene();

	m_mapScene[m_curSecne]->Render();

	pDev->EndScene();
	pDev->Present(0, 0, 0, 0);
}

void CGameManager::Update()
{
	m_mapScene[m_curSecne]->Update();
}

void CGameManager::WinProc(UINT msg, WPARAM wParam, LPARAM	lParam)
{
	CInputManager::GetInstance()->WinProc(msg, wParam, lParam);
}

CGameScene * CGameManager::getScene(TSTRING name)
{
	return m_mapScene[name];
}

void CGameManager::setNowScene(TSTRING name)
{
	if (!m_curSecne.empty())
		m_mapScene[m_curSecne]->TurnOut();
	m_curSecne = name;
	m_mapScene[m_curSecne]->TurnIn();
}

void CGameManager::addScene(TSTRING name, CGameScene * scene)
{
	m_mapScene[name] = scene;
}
