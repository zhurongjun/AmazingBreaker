//����bass�����������
#pragma once
#include "Singleton.h"
#include "string"
#include "vector"
using namespace std;
//bass��
#include "../include/bass.h"
#pragma comment(lib,"../lib/bass.lib")

/** ������Ϣ*/
struct MUSICLIST
{
	string name;		/**< �����ļ���·��*/
	HSTREAM hMusic;		/**< Bass����ṹ��*/
};
/**
	* @brief  CAudio4Bass ����˵��
	*			�����࣬������
	*			����BASS����
	*/
class CAudio4Bass: public CSingleton<CAudio4Bass>
{
	friend class CSingleton<CAudio4Bass>;
	/** ���캯��*/
	CAudio4Bass(void);
public:
	/** ��������*/
	~CAudio4Bass(void);

	/**
	*@brief  ��ʼ��Bass��
	*		
	*@param hwnd  ָ�����ھ����ȱʡ��ʾ��ǰ����
	*@return  HRESULT ����ʼ���Ƿ�ɹ�
	*/
	HRESULT  Initialize(HWND hwnd=NULL);

	/**
	*@brief  ��ʼ��Bass��
	*		��ȡXML���õ���ʼ��״̬������LoadFile()
	*@param PathName  ���õ�·��
	*@return  ��ʼ���Ƿ�ɹ���δ���ã�
	*/
	bool LoadFileFromXml(const char *PathName);
	/**
	*@brief  �������ֻ�ȡ����
	*@param name ���ֵ�����
	*@return  ���ֵ�Id
	*/
	int GetId(TSTRING name);
/**
	*@brief  ��������
	*		
	*@param sName  �����ļ�·��
	*@param flags		����Ч����־λ�����õ������¼���ѡ��
	*		-BASS_SAMPLE_LOOP		����ѭ������
	*		-BASS_SAMPLE_MONO		�������β���
	*@return  �ɹ���������ID��ʧ�ܷ���-1
	*/
	int LoadFile(const char *sName, DWORD flags);
	BOOL ChannelPlay(TSTRING name, bool restart);
/**
	*@brief  ��������
	*		
	*@param index		��������ID
	*@param restart	�Ƿ����¿�ʼ����
	*@return  �ɹ�����TRUE��ʧ�ܷ���FALSE
	*/
	BOOL ChannelPlay(int index, bool restart);
/**
	*@brief  ����ֹͣ
	*		
	*@param index		��������ID
	*@return  �ɹ�����TRUE��ʧ�ܷ���FALSE
	*/
	BOOL ChannelStop(int index);
/**
	*@brief  ������ͣ
	*		
	*@param index		��������ID
	*@return  �ɹ�����TRUE��ʧ�ܷ���FALSE
	*/
	BOOL ChannelPause(int index);
/**
	*@brief  ����������С
	*		
	*@param volume		����
	*@return  �ɹ�����TRUE��ʧ�ܷ���FALSE
	*/
	BOOL SetVolume(float volume);
/**
	*@brief  ��ȡ���ֵ��ܳ���
	*		��λΪ�ֽ�
	*@param index		��������ID
	*@param mode	��ȡģʽ��ֻ����BASS_POS_BYTE
	*@return  ���ֵ��ܳ���
	*/
	QWORD GetChannelLength(int index, DWORD mode);
/**
	*@brief  ��ȡ���ֵĲ���λ��
	*		��λΪ�ֽ�
	*@param index		��������ID
	*@param mode	��ȡģʽ��ֻ����BASS_POS_BYTE
	*@return  ���ֵĲ���λ��
	*/
	QWORD GetChannelPosition(int index, DWORD mode);
	/**
	*@brief  �������ֵĲ���λ��
	*		��λΪ�ֽ�
	*@param index		��������ID
	*@param mode	��ȡģʽ��ֻ����BASS_POS_BYTE
	*@return  �ɹ�����TRUE��ʧ�ܷ���FALSE
	*/
	BOOL SetChannelPosition(int index, QWORD pos, DWORD mode);
/**
	*@brief  ɾ�����Ź�������
	*		
	*@param index		��������ID
	*@return  �ɹ�����TRUE��ʧ�ܷ���FALSE
	*/
	BOOL DeleteMusic(int index);
/**
	*@brief  �ͷ����нӿ�
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
	vector<MUSICLIST> m_MusicList;	/**< �洢���е�������Ϣ*/
	map<TSTRING, int> m_mapMusic;	//�洢������Ϣ
	int m_iCurIndex;			//��ǰ���ŵ�����λ��
};

