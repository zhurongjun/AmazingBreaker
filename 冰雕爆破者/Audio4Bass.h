//基于bass引擎的声音类
#pragma once
#include "Singleton.h"
#include "string"
#include "vector"
using namespace std;
//bass库
#include "../include/bass.h"
#pragma comment(lib,"../lib/bass.lib")

/** 声音信息*/
struct MUSICLIST
{
	string name;		/**< 声音文件的路径*/
	HSTREAM hMusic;		/**< Bass引擎结构体*/
};
/**
	* @brief  CAudio4Bass 简易说明
	*			声音类，单键类
	*			基于BASS引擎
	*/
class CAudio4Bass: public CSingleton<CAudio4Bass>
{
	friend class CSingleton<CAudio4Bass>;
	/** 构造函数*/
	CAudio4Bass(void);
public:
	/** 析构函数*/
	~CAudio4Bass(void);

	/**
	*@brief  初始化Bass库
	*		
	*@param hwnd  指定窗口句柄，缺省表示当前窗口
	*@return  HRESULT ，初始化是否成功
	*/
	HRESULT  Initialize(HWND hwnd=NULL);

	/**
	*@brief  初始化Bass库
	*		读取XML配置档初始化状态，调用LoadFile()
	*@param PathName  配置档路径
	*@return  初始化是否成功（未启用）
	*/
	bool LoadFileFromXml(const char *PathName);
	/**
	*@brief  根据名字获取音乐
	*@param name 音乐的名字
	*@return  音乐的Id
	*/
	int GetId(TSTRING name);
/**
	*@brief  载入音乐
	*		
	*@param sName  声音文件路径
	*@param flags		播放效果标志位，常用的有以下几个选择
	*		-BASS_SAMPLE_LOOP		声音循环播放
	*		-BASS_SAMPLE_MONO		声音单次播放
	*@return  成功返回音乐ID，失败返回-1
	*/
	int LoadFile(const char *sName, DWORD flags);
	BOOL ChannelPlay(TSTRING name, bool restart);
/**
	*@brief  播放音乐
	*		
	*@param index		声音载入ID
	*@param restart	是否重新开始播放
	*@return  成功返回TRUE，失败返回FALSE
	*/
	BOOL ChannelPlay(int index, bool restart);
/**
	*@brief  音乐停止
	*		
	*@param index		声音载入ID
	*@return  成功返回TRUE，失败返回FALSE
	*/
	BOOL ChannelStop(int index);
/**
	*@brief  音乐暂停
	*		
	*@param index		声音载入ID
	*@return  成功返回TRUE，失败返回FALSE
	*/
	BOOL ChannelPause(int index);
/**
	*@brief  设置声音大小
	*		
	*@param volume		音量
	*@return  成功返回TRUE，失败返回FALSE
	*/
	BOOL SetVolume(float volume);
/**
	*@brief  获取音乐的总长度
	*		单位为字节
	*@param index		声音载入ID
	*@param mode	获取模式，只能是BASS_POS_BYTE
	*@return  音乐的总长度
	*/
	QWORD GetChannelLength(int index, DWORD mode);
/**
	*@brief  获取音乐的播放位置
	*		单位为字节
	*@param index		声音载入ID
	*@param mode	获取模式，只能是BASS_POS_BYTE
	*@return  音乐的播放位置
	*/
	QWORD GetChannelPosition(int index, DWORD mode);
	/**
	*@brief  设置音乐的播放位置
	*		单位为字节
	*@param index		声音载入ID
	*@param mode	获取模式，只能是BASS_POS_BYTE
	*@return  成功返回TRUE，失败返回FALSE
	*/
	BOOL SetChannelPosition(int index, QWORD pos, DWORD mode);
/**
	*@brief  删除播放过的音乐
	*		
	*@param index		声音载入ID
	*@return  成功返回TRUE，失败返回FALSE
	*/
	BOOL DeleteMusic(int index);
/**
	*@brief  释放所有接口
	*		
	*@param NULL
	*@return  void
	*/
	void Release();

//------------------
	int GetCurIndex(){return m_iCurIndex;}
	void SetCurIndex(unsigned int index)
	{
		if((index >=0)  && (index < m_MusicList.size()))
		{
			m_iCurIndex=index;
			ChannelPlay(m_iCurIndex,true);
		}
	}
//-----------------------
private:
	vector<MUSICLIST> m_MusicList;	/**< 存储所有的声音信息*/
	map<TSTRING, int> m_mapMusic;	//存储音乐信息
	int m_iCurIndex;			//当前播放的音乐位置
};

