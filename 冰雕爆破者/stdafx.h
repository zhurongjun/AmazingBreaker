// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once
#pragma comment(lib,"../lib/TinyXML.lib")
#pragma comment(lib,"Winmm.lib")
#define _CRT_SECURE_NO_WARNINGS
// Windows 头文件: 
#include <windows.h>

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <conio.h>
#include <d3dx9.h>
#include <d3d9.h>
#include <map>
#include <list>
#include <vector>
#include <string>
#include <functional>
using std::list;
using std::vector;
using std::map;
using std::basic_string;
using std::function;
using std::bind;
typedef basic_string<TCHAR> TSTRING;
typedef function<void(int)> PFUN;

// TODO: 在此处引用程序需要的其他头文件
#include "../include/tinystr.h"
#include "../include/tinyxml.h"
#include "../include/vfxdebug.h"
#include "../Include/vfxmemory.h"
#include "../include/bass.h"
#include "Audio4Bass.h"
#include "Singleton.h"
#include "IAnimable.h"
#include "AnimHander.h"
#include "AnimManager.h"
#include "Tool.h"
#include "Platform.h"
#include "ResManager.h"
#include "Animation.h"
#include "Texture.h"
#include "CollisionObj.h"
#include "Sprite.h"
#include "LevelManager.h"
#include "ResManager.h"
#include "GameScene.h"
#include "SEvent.h"
#include "Listener.h"
#include "InputManager.h"
#include "GameManager.h"

//UI
#include "Button.h"
#include "RoundButton.h"

#include "BaseBomb.h"

#define new VNEW
