// vfxdebug.cpp
// 
// VictoryCore Code
// debuging and memory manager
//
// Author : lanzhengpeng(兰征鹏)
// More author :
// Create time : 2002-6-13
// Modify time : 2002-6-23
//-----------------------------------------------------------------------------
#include "stdafx.h"
#include <tchar.h>
#include <iostream>
#include <BaseTsd.h>
#include <WTypes.h>
#include <assert.h>

#define new VNEW

namespace __vfxdebug
{
	UINT_PTR __VFX_TRACE_LEVEL	= 0;
	
#if	 defined(VFX_LOGO_FILE) || defined(VFX_MULT_LOGO_FILE)
	class __vfxLogoFile
	{
		VFile_Base *	m_pFile;
	public:
		CRITICAL_SECTION	m_cs;
		inline void Lock(){::EnterCriticalSection((CRITICAL_SECTION *)&m_cs);}
		inline void Unlock(){::LeaveCriticalSection((CRITICAL_SECTION *)&m_cs);}

		void SetIO(VFile_Base * pFile)
		{
			delete m_pFile;
			m_pFile = pFile;
		}
		void OpenLogo()
		{
			ASSERT(m_pFile == NULL);

			m_pFile = new VFile;
			TCHAR buff[MAX_PATH];
			GetModuleFileName(GetModuleHandle(NULL),buff,MAX_PATH);
			_tcscat(buff,vT(".log"));

			BOOL bOpened = TRUE;
#if	 defined(VFX_MULT_LOGO_FILE)
			if(!m_pFile->Open(buff,VFile::modeWrite | VFile::shareDenyNone))
#endif
				m_pFile->Open(buff,VFile::modeCreate | VFile::modeWrite | VFile::shareDenyNone);
			m_pFile->Seek(0,VFile_Base::end);
		}
		__vfxLogoFile()
		{	
			m_pFile = NULL;
			::InitializeCriticalSection((CRITICAL_SECTION *)&m_cs);
		}
		~__vfxLogoFile()
		{	
			destructor();
			::DeleteCriticalSection((CRITICAL_SECTION *)&m_cs);
		}
		void destructor()
		{
			if(m_pFile != NULL)
			{
				m_pFile->Flush();
				m_pFile->Close();
				delete m_pFile;
			}
		}
		virtual DWORD_PTR Write(const void* lpBuf, DWORD_PTR nCount)
		{
			__try
			{
				__try
				{
					Lock();
					if(m_pFile == NULL)
						OpenLogo();
					if(m_pFile != NULL)
						return m_pFile->Write(lpBuf,nCount);
					return 0;
				}
				__finally
				{
					Unlock();
				}
				return 0;
			}
			__except(EXCEPTION_EXECUTE_HANDLER){
				return 0;
			}
		}
		virtual BOOL Flush()
		{
			__try
			{
				__try
				{
					Lock();
					if(m_pFile != NULL)
						return m_pFile->Flush();
					return FALSE;
				}
				__finally
				{
					Unlock();
				}
				return 0;
			}
			__except(EXCEPTION_EXECUTE_HANDLER){
				return FALSE;
			}
		}
		
	};

	__vfxLogoFile & __vfxgetlogofile()
	{
		static __vfxLogoFile	__logofile;
		return __logofile;
	}
 
#endif

	//void __vfxDestructorOutputDebug()
	//{
	//	__vfxgetlogofile().destructor();
	//}
	static time_t g_OutputDebugTime = 0;
	static CHAR  g_szOutputDebugTime[32];

	void __vfxOutputDebugStringA(LPCSTR lpOutputString)
	{
		::OutputDebugStringA(lpOutputString);
		std::cout << lpOutputString;

	#if	 defined(VFX_LOGO_FILE) || defined(VFX_MULT_LOGO_FILE)
		__try
		{
			time_t tNow = time(NULL);
			if(g_OutputDebugTime != tNow)
			{
				g_OutputDebugTime = tNow;
				tm * tmNow = localtime(&tNow);
				sprintf(g_szOutputDebugTime,"[%.4d-%.2d-%.2d %.2d:%.2d:%.2d]",
					tmNow->tm_year+1900,tmNow->tm_mon+1,tmNow->tm_mday,
					tmNow->tm_hour,tmNow->tm_min,tmNow->tm_sec);
			}
			__vfxgetlogofile().Lock();
			__vfxgetlogofile().Write(g_szOutputDebugTime,21);

			size_t n = strlen(lpOutputString);
			if(n > 1 && lpOutputString[n - 1] == '\n' && lpOutputString[n - 2] != '\r')
			{
				__vfxgetlogofile().Write(lpOutputString,n - 1);
				__vfxgetlogofile().Write("\r\n",2);
			}
			else
				__vfxgetlogofile().Write(lpOutputString,n);
			//__vfxgetlogofile().Flush();
		}
		__finally
		{
			__vfxgetlogofile().Unlock();
		}
	#endif
	}

	void __vfxOutputDebugStringW(LPCWSTR lpOutputString)
	{
		::OutputDebugStringW(lpOutputString);
		std::cout << lpOutputString;
	#if	 defined(VFX_LOGO_FILE) || defined(VFX_MULT_LOGO_FILE)
		__try
		{
			__vfxgetlogofile().Lock();
			size_t n = wcslen(lpOutputString);
			if(n > 1 && lpOutputString[n - 1] == L'\n' && lpOutputString[n - 2] != L'\r')
			{
				__vfxgetlogofile().Write(lpOutputString,n - 1);
				__vfxgetlogofile().Write(L"\r\n",4);
			}
			else
				__vfxgetlogofile().Write(lpOutputString,n);
			//__vfxgetlogofile().Flush();
		}
		__finally
		{
			__vfxgetlogofile().Unlock();
		}
	#endif
	}
}

UINT_PTR _vfxGetTraceLevel()
{
	return __vfxdebug::__VFX_TRACE_LEVEL;
}

void _vfxSetTraceLevel(UINT_PTR uLevel)
{
	__vfxdebug::__VFX_TRACE_LEVEL = uLevel;
}

void _vfxSetTraceIO(class VFile_Base * pFile)
{
	//__vfxdebug::__vfxgetlogofile().SetIO(pFile);
}

void __cdecl _vfxTraceA(LPCSTR lpszFormat, ...)
{
#if _DEBUG
	va_list pArgs;
	va_start(pArgs,lpszFormat);

	CHAR Tmp[1024];
	
	sprintf(Tmp,lpszFormat,pArgs);
	//assert(nBuf < sizeof(Tmp));

#if	 defined(VFX_LOGO_FILE) || defined(VFX_MULT_LOGO_FILE)
	if(IsDebuggerPresent())
		::OutputDebugStringA(Tmp);
	else
		__vfxdebug::__vfxOutputDebugStringA(Tmp);
#else
	::OutputDebugStringA(Tmp);
#endif
	va_end(pArgs);
#endif
}

void __cdecl _vfxLevelTraceA(UINT_PTR level, LPCSTR lpszFormat, ...)
{
	if(level >= __vfxdebug::__VFX_TRACE_LEVEL)
	{
		va_list pArgs;
		va_start(pArgs,lpszFormat);

		CHAR Tmp[1024];

		sprintf(Tmp,lpszFormat,pArgs);
		//assert(nBuf < sizeof(Tmp));

		__vfxdebug::__vfxOutputDebugStringA(Tmp);
		va_end(pArgs);
	}
}

void __cdecl _vfxTraceW(LPCWSTR lpszFormat, ...)
{
#if _DEBUG
	va_list pArgs;
	va_start(pArgs,lpszFormat);

	CHAR Tmp[1024];
	WCHAR ttmp[1024];

	int nBuf = ::_vsnwprintf(ttmp,1024,lpszFormat,pArgs);
	assert(nBuf < sizeof(ttmp));

	//VC对宽字符的调试信息支持不好 :-(
	WideCharToMultiByte( CP_ACP, 0, ttmp, -1, Tmp, 1024, NULL, NULL );
#if	 defined(VFX_LOGO_FILE) || defined(VFX_MULT_LOGO_FILE)
	if(IsDebuggerPresent())
		::OutputDebugStringA(Tmp);
	else
		__vfxdebug::__vfxOutputDebugStringA(Tmp);
#else
	::OutputDebugStringA(Tmp);
#endif
	va_end(pArgs);
#endif
}

void __cdecl _vfxLevelTraceW(UINT_PTR level, LPCWSTR lpszFormat, ...)
{
	if(level >= __vfxdebug::__VFX_TRACE_LEVEL)
	{
		va_list pArgs;
		va_start(pArgs,lpszFormat);

		CHAR Tmp[1024];
		WCHAR ttmp[1024];

		int nBuf = ::_vsnwprintf(ttmp,1024,lpszFormat,pArgs);
		assert(nBuf < sizeof(ttmp));

		//VC对宽字符的调试信息支持不好 :-(
		WideCharToMultiByte( CP_ACP, 0, ttmp, -1, Tmp, 1024, NULL, NULL );
		__vfxdebug::__vfxOutputDebugStringA(Tmp);
		va_end(pArgs);
	}
}

#if __CPL_FULL_VER < 14000000
BOOL _vfxAssertValid(LPVOID p,LPCSTR lpszFileName,DWORD dwLine)
{
	if(p == NULL || ::IsBadReadPtr(p,1) != 0)
	{
		CHAR Tmp[1024];
		::sprintf(Tmp,"Assert Error At:\nfile:%s\nline:%d\nSelect Yes to debug",lpszFileName,dwLine);
		if(::MessageBoxA(NULL,Tmp,"VectoryCore Error",MB_YESNO | MB_ICONERROR | MB_DEFBUTTON1 | MB_SYSTEMMODAL | MB_TOPMOST) == IDYES)
		{
#ifdef _DEBUG
			//_assert(0,(LPCSTR)lpszFileName,dwLine);
#endif
			return TRUE;
		}
		return FALSE;
	}
	return FALSE;
}

BOOL _vfxAssertThis(LPVOID p,LPCSTR lpszFileName,DWORD dwLine)
{
	if(p == NULL || ::IsBadWritePtr(p,4) != 0)
	{
		CHAR Tmp[1024];
		::sprintf(Tmp,"Assert Error At:\nfile:%s\nline:%d\nSelect Yes to debug",lpszFileName,dwLine);
		if(::MessageBoxA(NULL,Tmp,"VectoryCore Error",MB_YESNO | MB_ICONERROR | MB_DEFBUTTON1 | MB_SYSTEMMODAL | MB_TOPMOST) == IDYES)
		{
#ifdef _DEBUG
			//_assert(0,(LPSTR)lpszFileName,dwLine);
#endif
			return TRUE;
		}
		return FALSE;
	}
	return FALSE;
}
#else
BOOL _vfxAssertValid(LPVOID p,LPCWSTR lpszFileName,DWORD dwLine)
{
	if(p == NULL || ::IsBadReadPtr(p,1) != 0)
	{
		WCHAR Tmp[1024];
		::swprintf(Tmp,1024,L"Assert Error At:\nfile:%s\nline:%d\nSelect Yes to debug",lpszFileName,dwLine);
		if(::MessageBoxW(NULL,Tmp,L"VectoryCore Error",MB_YESNO | MB_ICONERROR | MB_DEFBUTTON1 | MB_SYSTEMMODAL | MB_TOPMOST) == IDYES)
		{
#ifdef _DEBUG
			_wassert(0,lpszFileName,dwLine);
#endif
			return TRUE;
		}
		return FALSE;
	}
	return FALSE;
}

BOOL _vfxAssertThis(LPVOID p,LPCWSTR lpszFileName,DWORD dwLine)
{
	if(p == NULL || ::IsBadWritePtr(p,4) != 0)
	{
		WCHAR Tmp[1024];
		::swprintf(Tmp,1024,L"Assert Error At:\nfile:%s\nline:%d\nSelect Yes to debug",lpszFileName,dwLine);
		if(::MessageBoxW(NULL,Tmp,L"VectoryCore Error",MB_YESNO | MB_ICONERROR | MB_DEFBUTTON1 | MB_SYSTEMMODAL | MB_TOPMOST) == IDYES)
		{
#ifdef _DEBUG
			_wassert(0,lpszFileName,dwLine);
#endif
			return TRUE;
		}
		return FALSE;
	}
	return FALSE;
}
#endif