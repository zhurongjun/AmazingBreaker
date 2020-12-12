#include "stdafx.h"
#include "Audio4Bass.h"


CAudio4Bass::CAudio4Bass(void)
{
}


CAudio4Bass::~CAudio4Bass(void)
{
}


//��ʼ��Bass��
HRESULT  CAudio4Bass::Initialize(HWND hwnd)
{
	return BASS_Init(-1,				//�����豸ΪĬ��
		44100,								//������
		0,										//Ч������
		0,										//���ھ��
		0);										//Ĭ�����ݳ�ʼ��DirectSound
}
//�ͷ����нӿ�
void CAudio4Bass::Release()
{
	::BASS_Free();
}

//���������ļ�
bool CAudio4Bass::LoadFileFromXml(const char *PathName)
{
	////------------------------------��ȡXML��----------
	INT TempID = 0;
	TiXmlDocument *MyDoc = new TiXmlDocument;
	MyDoc->LoadFile(PathName);

	TiXmlElement *pBoot;
	BOOL loop;
	pBoot = MyDoc->FirstChildElement();

	TiXmlElement *pCurChild = pBoot->FirstChildElement();
	int id;
	TCHAR tStr[256];
	char  aStr[256];
	while (pCurChild)
	{
		pCurChild->Attribute("loop", &loop);
		if (loop)
			id = this->LoadFile(pCurChild->Attribute("file"), BASS_SAMPLE_LOOP);
		else
			id = this->LoadFile(pCurChild->Attribute("file"), BASS_SAMPLE_MONO);
		strcpy(aStr, pCurChild->Attribute("name"));
		Anis_To_Auto(aStr, 256, tStr, 256);
		m_mapMusic[tStr] = id;
		pCurChild = pCurChild->NextSiblingElement();
	}
	if (MyDoc)
	{
		delete MyDoc;
		MyDoc = NULL;
	}
	////-----------------------------------------------------------------
	return true;
}
int CAudio4Bass::GetId(TSTRING name)
{
	return m_mapMusic[name];
}
//��������
int CAudio4Bass::LoadFile(const char *sName, DWORD flags)
{
	int nSize=m_MusicList.size();
	for (int i=0; i<nSize; i++)
	{
		if(m_MusicList[i].name==sName)
			return i;
	}
	HSTREAM Sound=::BASS_StreamCreateFile(FALSE, sName, 0,0,flags);
	if (Sound)
	{
		MUSICLIST musicinfo;
		musicinfo.name=sName;
		musicinfo.hMusic=Sound;
		m_MusicList.push_back(musicinfo);
		return (int) m_MusicList.size()-1;
	}
	else
		return -1;
}

BOOL CAudio4Bass::ChannelPlay(TSTRING name, bool restart)
{
	return this->ChannelPlay(m_mapMusic[name], restart);
}

//��������
BOOL CAudio4Bass::ChannelPlay(int index, bool restart)
{
	if (NULL !=m_MusicList[index].hMusic)
		return ::BASS_ChannelPlay(m_MusicList[index].hMusic,restart);
	else
		return false;
}
//����ֹͣ
BOOL CAudio4Bass::ChannelStop(int index)
{
	if (NULL !=m_MusicList[index].hMusic)
		return ::BASS_ChannelStop(m_MusicList[index].hMusic);
	else
		return false;
}
//ɾ�����Ź�������
BOOL CAudio4Bass::DeleteMusic(int index)
{
	BOOL b=::BASS_StreamFree(m_MusicList[index].hMusic);
	m_MusicList.erase(m_MusicList.begin()+index);
	return b;
}

//������ͣ
BOOL CAudio4Bass::ChannelPause(int index)
{
	if (NULL !=m_MusicList[index].hMusic)
		return ::BASS_ChannelPause(m_MusicList[index].hMusic);
	else
		return false;
}
//����������С
BOOL CAudio4Bass::SetVolume(float volume)
{
	return ::BASS_SetVolume(volume);
}
//��ȡ���ֵ��ܳ���
QWORD CAudio4Bass::GetChannelLength(int index, DWORD mode)
{
	return ::BASS_ChannelGetLength(m_MusicList[index].hMusic, mode);
}
//��ȡ���ֵĲ���λ��
QWORD CAudio4Bass::GetChannelPosition(int index, DWORD mode)
{
	return ::BASS_ChannelGetPosition(m_MusicList[index].hMusic, mode);
}
BOOL CAudio4Bass::SetChannelPosition(int index, QWORD pos, DWORD mode)
{
	return ::BASS_ChannelSetPosition(m_MusicList[index].hMusic, pos, mode);
}
