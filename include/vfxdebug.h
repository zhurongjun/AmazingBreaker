// vfxdebug.cpp
// 
// VictoryCore Code
// debuging and memory manager
//
// Author : lanzhengpeng(À¼Õ÷Åô)
// More author :
// Create time : 2002-6-13
// Modify time : 2002-6-23
//-----------------------------------------------------------------------------
#ifndef __VFX_DEBUG_H__
#define __VFX_DEBUG_H__

#pragma once

#	define vT(x)	x

#ifndef _DBG_WIDE
#	if __CPL_FULL_VER < 14000000
#		define __DBG_WIDE(_String)	_String
#	else
#		define __DBG_WIDE(_String)	L##_String
#	endif
#	define _DBG_WIDE(String)	__DBG_WIDE(String)
#endif

#ifndef _MFC
#	ifndef ASSERT
#		include <cassert>
#		define ASSERT assert
#	endif
#	if _DEBUG
#		if !defined(THIS_FILE)
#			define THIS_FILE				__FILE__
#		endif
#		if !defined(TRACE)
#			define TRACE					_vfxTrace
#		endif
#		if !defined(VERIFY)
#			define VERIFY(f)				ASSERT(f)
#		endif
#		if !defined(ASSERT_VALID)
#			define ASSERT_VALID(p)			_vfxAssertValid(p,_DBG_WIDE(__FILE__),__LINE__)
#		endif
#		if !defined(ASSERT_THIS)
#			define ASSERT_THIS(p)			_vfxAssertThis(p,_DBG_WIDE(__FILE__),__LINE__)
#		endif
#		if !defined(TRACE0)
#			define TRACE0(sz)              _vfxTrace(sz)
#		endif
#		if !defined(TRACE1)
#			define TRACE1(sz, p1)          _vfxTrace(sz, p1)
#		endif
#		if !defined(TRACE2)
#			define TRACE2(sz, p1, p2)      _vfxTrace(sz, p1, p2)
#		endif
#		if !defined(TRACE3)
#			define TRACE3(sz, p1, p2, p3)  _vfxTrace(sz, p1, p2, p3)
#		endif
#	else   // _DEBUG
#		if !defined(VERIFY)
#			define VERIFY(f)				((void)(f))
#		endif
#		if !defined(ASSERT_VALID)
#			define ASSERT_VALID(pOb)		((void)0)
#		endif
#		if !defined(ASSERT_THIS)
#			define ASSERT_THISD(p)			((void)0)
#		endif
#		if !defined(TRACE)
#			define TRACE					_vfxTrace
#		endif
#		if !defined(TRACE0)
#			define TRACE0(sz)
#		endif
#		if !defined(TRACE1)
#			define TRACE1(sz, p1)
#		endif
#		if !defined(TRACE2)
#			define TRACE2(sz, p1, p2)
#		endif
#		if !defined(TRACE3)
#			define TRACE3(sz, p1, p2, p3)
#		endif
#	endif // !_DEBUG

#	if !defined(UNUSED)
#		define UNUSED(x)
#	endif

#endif //end _MFC

#	ifndef _NOVNEW

#		define _vfx_New		_vfxMemoryNew
#		define _vfx_Delete	_vfxMemoryDelete

#		include <new>

#		undef new
#		if _DEBUG
#			define VFX_DEBUG_MEMORY		1
#		else
#			undef VFX_DEBUG_MEMORY
//#			define VFX_DEBUG_MEMORY		1
#	endif

void * _vfx_New(size_t nSize,const char * file,unsigned int line);
void _vfx_Delete(void * memory,const char * file,unsigned int line);
void * _vfxMemoryNew(size_t size,const char * file,unsigned int line);

#		ifdef VFX_DEBUG_MEMORY

	inline void * _cdecl operator new (size_t nSize,const char * pFileName,int dwLine)
	{	return _vfx_New(nSize,pFileName,dwLine); }
	inline void * _cdecl operator new (size_t nSize,std::nothrow_t,const char * pFileName,int dwLine)
	{	return _vfx_New(nSize,pFileName,dwLine); }
	inline void _cdecl operator delete (void * pMem,const char * pFileName,int dwLine)
	{	_vfx_Delete(pMem,pFileName,dwLine); }

	inline void * _cdecl operator new [] (size_t nSize,const char * pFileName,int dwLine)
	{	return _vfx_New(nSize,pFileName,dwLine); }
	inline void * _cdecl operator new [] (size_t nSize,std::nothrow_t,const char * pFileName,int dwLine)
	{	return _vfx_New(nSize,pFileName,dwLine); }
	inline void _cdecl operator delete [] (void * pMem,const char * pFileName,int dwLine)
	{ 	_vfx_Delete(pMem,pFileName,dwLine); }

	inline void * _cdecl operator new (size_t nSize)
	{	return _vfx_New(nSize,0,0); }
	inline void * _cdecl operator new (size_t nSize,std::nothrow_t)
	{	return _vfxMemoryNew(nSize,0,0); }
	inline void _cdecl operator delete (void * pMem)
	{	_vfx_Delete(pMem,0,0); }

	inline void * _cdecl operator new [] (size_t nSize)
	{	return _vfx_New(nSize,0,0); }
	inline void * _cdecl operator new [] (size_t nSize,std::nothrow_t)
	{ 	return _vfxMemoryNew(nSize,0,0); }
	inline void _cdecl operator delete [] (void * pMem)
	{	_vfx_Delete(pMem,0,0); }	

#		else

	inline void * _cdecl operator new (size_t nSize)
	{	return _vfx_New(nSize,0,0); }
	inline void * _cdecl operator new (size_t nSize,std::nothrow_t)
	{	return _vfxMemoryNew(nSize,0,0); }
	inline void _cdecl operator delete (void * pMem)
	{	_vfx_Delete(pMem,0,0); }	

	inline void * _cdecl operator new [] (size_t nSize)
	{ 	return _vfx_New(nSize,0,0); }
	inline void * _cdecl operator new [] (size_t nSize,std::nothrow_t)
	{ 	return _vfxMemoryNew(nSize,0,0); }
	inline void _cdecl operator delete [] (void * pMem)
	{	_vfx_Delete(pMem,0,0); }	
	
#		endif //end VFX_DEBUG_MEMORY

#		if VFX_DEBUG_MEMORY
#			define VNEW new(__FILE__,__LINE__)
#			define VNEW_NOTHROW new(std::nothrow,__FILE__,__LINE__)
#		else
#			define VNEW new
#			define VNEW_NOTHROW new(std::nothrow)
#		endif	//end VFX_DEBUG_MEMORY
#	else
#		define VNEW new
#		define VNEW_NOTHROW new(std::nothrow)
#	endif //end _NOVNEW
	
#undef VFX_ASSERT
#include <cassert>
#define VFX_ASSERT assert

#undef VFX_TRACE
#define VFX_TRACE						_vfxTrace
#undef VFX_LTRACE
#define VFX_LTRACE						_vfxLevelTrace
#undef VFX_UNUSED
#define VFX_UNUSED(x)
#if _DEBUG
#	undef VFX_VERIFY
#	define VFX_VERIFY(f)				VFX_ASSERT(f)
#	undef VFX_ASSERT_VALID
#	define VFX_ASSERT_VALID(p)			_vfxAssertValid(p,_DBG_WIDE(__FILE__),__LINE__)
#	undef VFX_ASSERT_THIS
#	define VFX_ASSERT_THIS(p)			_vfxAssertThis(p,_DBG_WIDE(__FILE__),__LINE__)
#	undef VFX_TRACE0
#	define VFX_TRACE0(sz)				_vfxTrace(sz)
#	undef VFX_TRACE1
#	define VFX_TRACE1(sz,p0)			_vfxTrace(sz,p0)
#	undef VFX_TRACE2
#	define VFX_TRACE2(sz,p1,p2)			_vfxTrace(sz,p1,p2)
#	undef VFX_TRACE3
#	define VFX_TRACE3(sz,p1,p2,p3)		_vfxTrace(sz,p1,p2,p3)
#	undef VFX_TRACE4
#	define VFX_TRACE4(sz,p1,p2,p3,p4)	_vfxTrace(sz,p1,p2,p3,p4)
#else   // _DEBUG
#	undef VFX_VERIFY
#	define VFX_VERIFY(f)				((void)(f))
#	undef VFX_ASSERT_VALID
#	define VFX_ASSERT_VALID(p)			((void)0)
#	undef VFX_ASSERT_THIS
#	define VFX_ASSERT_THIS(p)			((void)0)
#	undef VFX_TRACE0
#	define VFX_TRACE0(sz)
#	undef VFX_TRACE1
#	define VFX_TRACE1(sz,p0)
#	undef VFX_TRACE2
#	define VFX_TRACE2(sz,p1,p2)
#	undef VFX_TRACE3
#	define VFX_TRACE3(sz,p1,p2,p3)
#	undef VFX_TRACE4
#	define VFX_TRACE4(sz,p1,p2,p3,p4)
#endif // !_DEBUG

//#ifndef __VFX_LOGO_FILE
//#define __VFX_LOGO_FILE		"vfxlog.txt"
//#endif

void __cdecl _vfxTraceA(LPCSTR lpszFormat, ...);
void __cdecl _vfxLevelTraceA(UINT_PTR level, LPCSTR lpszFormat, ...);
void __cdecl _vfxTraceW(LPCWSTR lpszFormat, ...);
void __cdecl _vfxLevelTraceW(UINT_PTR level, LPCWSTR lpszFormat, ...);
#ifdef UNICODE
#	define _vfxTrace		_vfxTraceW
#	define _vfxLevelTrace	_vfxLevelTraceW
#else
#	define _vfxTrace		_vfxTraceA
#	define _vfxLevelTrace	_vfxLevelTraceA
#endif

#if __CPL_FULL_VER < 14000000
BOOL _vfxAssertValid(LPVOID p,LPCSTR lpszFileName,DWORD dwLine);
BOOL _vfxAssertThis(LPVOID p,LPCSTR lpszFileName,DWORD dwLine);
#else
BOOL _vfxAssertValid(LPVOID p,LPCWSTR lpszFileName,DWORD dwLine);
BOOL _vfxAssertThis(LPVOID p,LPCWSTR lpszFileName,DWORD dwLine);
#endif

UINT_PTR _vfxGetTraceLevel();
void _vfxSetTraceLevel(UINT_PTR uLevel);
void _vfxSetTraceIO(class VFile_Base *);

inline void _vfxReportException(LPCSTR pStrResion,const EXCEPTION_RECORD & SavedExpRecord,const CONTEXT & SavedExpContext)
{
	VFX_LTRACE(5,	_T("%s\r\n")
					_T("error 0x%8.8X at 0x%8.8X\r\n"),
					pStrResion,
					SavedExpRecord.ExceptionCode,
					SavedExpRecord.ExceptionAddress);
#ifdef _WIN64
	VFX_LTRACE(5,	_T("RAX = 0x%p\r\n")
					vT("RBX = 0x%p\r\n")
					vT("RCX = 0x%p\r\n")
					vT("RDX = 0x%p\r\n")
					vT("RSI = 0x%p\r\n")
					vT("RDI = 0x%p\r\n")
					vT("RSP = 0x%p\r\n")
					vT("RBP = 0x%p\r\n")
					vT("RIP = 0x%p\r\n")
					vT("R8 = 0x%p\r\n")
					vT("R9 = 0x%p\r\n")
					vT("R10 = 0x%p\r\n")
					vT("R11 = 0x%p\r\n")
					vT("R12 = 0x%p\r\n")
					vT("R13 = 0x%p\r\n")
					vT("R14 = 0x%p\r\n")
					vT("R15 = 0x%p\r\n")
					vT("EFL = 0x%p\r\n")
					,
					SavedExpContext.Rax,
					SavedExpContext.Rbx,
					SavedExpContext.Rcx,
					SavedExpContext.Rdx,
					SavedExpContext.Rsi,
					SavedExpContext.Rdi,
					SavedExpContext.Rsp,
					SavedExpContext.Rbp,
					SavedExpContext.Rip,
					SavedExpContext.R8,
					SavedExpContext.R9,
					SavedExpContext.R10,
					SavedExpContext.R11,
					SavedExpContext.R12,
					SavedExpContext.R13,
					SavedExpContext.R14,
					SavedExpContext.R15,
					SavedExpContext.EFlags);
#else
	VFX_LTRACE(5,	_T("EAX = 0x%8.8X\r\n")
					vT("EBX = 0x%8.8X\r\n")
					vT("ECX = 0x%8.8X\r\n")
					vT("EDX = 0x%8.8X\r\n")
					vT("ESI = 0x%8.8X\r\n")
					vT("EDI = 0x%8.8X\r\n")
					vT("ESP = 0x%8.8X\r\n")
					vT("EBP = 0x%8.8X\r\n")
					vT("EIP = 0x%8.8X\r\n")
					vT("EFL = 0x%8.8X\r\n"),
					SavedExpContext.Eax,
					SavedExpContext.Ebx,
					SavedExpContext.Ecx,
					SavedExpContext.Edx,
					SavedExpContext.Esi,
					SavedExpContext.Edi,
					SavedExpContext.Esp,
					SavedExpContext.Ebp,
					SavedExpContext.Eip,
					SavedExpContext.EFlags);
#endif
}

#endif	//end __VFX_DEBUG_H__
