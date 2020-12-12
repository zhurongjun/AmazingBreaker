// vfxmemory.cpp
// 
// VictoryCore Code
// memory allocer
//
// Author : lanzhengpeng(兰征鹏)
// More author :
// Create time : 2002-6-13
// Modify time :
// 内存分配代码依赖一个全局变量。此全局变量析构的时候，CRT可能已经被(部分)销毁
// 导致在析构函数中不能使用CRT的相关函数。而在最后又需要报告内存信息，需
// 要使用到C运行库的_vsnprintf函数。
// 妥协的做法是在析构函数里不做任何事，而在DLL_PROCESS_DETACH时报告内存信息。
// 事实上的内存分配和错误报告输出相互依赖也导致问题，故错误输出也需要单独写
//-----------------------------------------------------------------------------
#include "stdafx.h"
#ifdef _MFC
	#include <afx.h>
#else
	#include <windows.h>
#endif
#include "vfxmemory.h"
#include <cassert>
#include <new>
#include <stdio.h>

#if defined(_MSC_VER) && _MSC_VER >= 1300
#	define USE_STD	std
#else
#	define USE_STD
#endif

#undef vT
#define vT(x)	x

static void __cdecl __MemoryTrace(const char * lpszFormat, ...)
{
#ifdef VFX_DEBUG_MEMORY
	va_list pArgs;
	va_start(pArgs,lpszFormat);

	char Tmp[1024];
	vsprintf(Tmp,lpszFormat,pArgs);
	::OutputDebugStringA(Tmp);

	va_end(pArgs);
#endif
}

namespace VFX_Memory
{

#ifdef _MT
static long g_memory_critical = 0;
void memory_lock()
{
	if(InterlockedExchange(&g_memory_critical,1))
		while(InterlockedExchange(&g_memory_critical,1)) Sleep(0);
}
void memory_unlock()
{
	InterlockedExchange(&g_memory_critical,0);
}
class auto_memory_lock
{
public:
	auto_memory_lock()
	{ memory_lock();}
	~auto_memory_lock()
	{ memory_unlock();}
};
#endif //end _MT

#ifdef VFX_DEBUG_MEMORY
const unsigned int	c_uMalloc0xCC	= 0xCCCCCCCC;
static size_t g_alloc_times = 0;
static __int64 g_alloc_break_times = -1;
static size_t g_memory_obj = 0;
static size_t g_memoey_used = 0;
static size_t g_memory_max = 0;

static int _vfxMemoryOutputReport();
#endif	//end VFX_DEBUG_MEMORY

small_alloc	*	small_alloc::__psmall_alloc = 0;

small_alloc::small_alloc()
{
	memset(pool_chunk,0,sizeof(pool_chunk));
}

small_alloc::~small_alloc()
{
	destructor();
}

void small_alloc::destructor()
{
#ifdef VFX_DEBUG_MEMORY
	_vfxMemoryOutputReport();
	Dump();

	size_t size = poolArray.GetNumber();
	_small_cookie **	p = poolArray.GetData();
	
	for(size_t i=0;i<size;++i,++p)
		__free(*p);
#endif	//end VFX_DEBUG_MEMORY
}

_small_cookie * small_alloc::alloc_pool(size_t size)
{
	size = round_up(size);
	size_t newsize = round_up(size + sizeof(_small_cookie) - sizeof(_small_cookie *)) + sizeof(unsigned);
	_small_cookie * p = reinterpret_cast<_small_cookie *>(__alloc(newsize * __pool_size));

	{
		_small_cookie * pp = reinterpret_cast<_small_cookie*>(p);
		_small_cookie * next;
		for (size_t i=0;i<__pool_size-1;++i,pp=next)
		{
			next = reinterpret_cast<_small_cookie*>((INT_PTR)pp + newsize);
			pp->next = next;
			pp->size = size;
#ifdef VFX_DEBUG_MEMORY
			pp->file = 0;
			pp->line = 0;
			pp->id = (unsigned int)-1;
#endif	//end VFX_DEBUG_MEMORY
		}
		pp->next = 0;
		pp->size = size;
#ifdef VFX_DEBUG_MEMORY
			pp->file = 0;
			pp->line = 0;
			pp->id = (unsigned int)-1;
#endif	//end VFX_DEBUG_MEMORY
	}

	poolArray.Add(p);
#ifdef VFX_DEBUG_MEMORY
	sizeArray.Add(size);
#endif	//end VFX_DEBUG_MEMORY
	return p;
}

#ifdef VFX_DEBUG_MEMORY
void * small_alloc::alloc(size_t size,const char * file,unsigned int line)
{
	assert(size <= __max_bytes);

	if(size <= 0) size = 1;
	_small_cookie * &free_list = pool_chunk[(size - 1) / __align_size];
	if(free_list == 0)
		free_list = alloc_pool(size);
	void * ret = free_list->data;
	free_list->file = file;
	free_list->line = line;
	free_list->size = size;
	free_list->id = g_alloc_times++;
	free_list->dccc = c_uMalloc0xCC;
	g_memoey_used += free_list->size;
	g_memory_obj++;
	if(g_memory_max < g_memoey_used)
		g_memory_max = g_memoey_used;
	free_list = free_list->next;

	*(unsigned *)((INT_PTR)ret + size) = c_uMalloc0xCC;
	return ret;
}

void small_alloc::free(void * memory,const char * file,unsigned int line)
{
	_small_cookie * pcook = reinterpret_cast<_small_cookie *>
		((INT_PTR)memory - sizeof(_small_cookie) + sizeof(_small_cookie *));
	size_t size = pcook->size;
	assert(size <= __max_bytes);

	if(pcook->dccc != c_uMalloc0xCC)//前效验位有问题，很可能越界或错误的指针
	{
		if(file)
			__MemoryTrace(vT("%s(%d) : Verify pointer(0x%8.8x) failed!!!\n"),file,line,memory);
		if(IsBadReadPtr(pcook->file,4) == 0)
			__MemoryTrace(vT("%s(%d) : Verify pointer(0x%8.8x) failed!!! Alloc ID : %d\n"),pcook->file,pcook->line,memory,pcook->id);
		else
			__MemoryTrace(vT("Unkown position : Verify pointer(0x%8.8x) failed!!! Alloc ID : %d\n"),memory,pcook->id);
	}
	UINT_PTR * pdccc = reinterpret_cast<UINT_PTR*>((INT_PTR)memory + size);
	if(*pdccc != c_uMalloc0xCC)//后效验位有问题，很可能越界或错误的指针
	{
		if(file)
			__MemoryTrace(vT("%s(%d) : Verify pointer(0x%8.8x) failed!\n"),file,line,memory);
		if(IsBadReadPtr(pcook->file,4) == 0)
			__MemoryTrace(vT("%s(%d) : Verify pointer(0x%8.8x) failed! Alloc ID : %d\n"),pcook->file,pcook->line,memory,pcook->id);
		else
			__MemoryTrace(vT("Unkown position : Verify pointer(0x%8.8x) failed! Alloc ID : %d\n"),memory,pcook->id);
	}

	memset(memory,0xCC,size);

	g_memory_obj--;
	g_memoey_used -= size;
	
	_small_cookie * & free_list = pool_chunk[(pcook->size - 1) / __align_size];
	pcook->next = free_list;
	free_list = pcook;
	pcook->id = (unsigned int)-1;
}

#else
void * small_alloc::alloc(size_t size)
{
	if(size <= 0) size = 1;
	_small_cookie * &free_list = pool_chunk[(size - 1) / __align_size];
	if(free_list == 0)
		free_list = alloc_pool(size);
	free_list->size = size;
	void * ret = free_list->data;
	free_list = free_list->next;

	return ret;
}

void small_alloc::free(void * memory)
{
	_small_cookie * pcook = reinterpret_cast<_small_cookie *>
		((INT_PTR)memory - sizeof(_small_cookie) + sizeof(_small_cookie *));
	_small_cookie * & free_list = pool_chunk[(pcook->size - 1) / __align_size];
	pcook->next = free_list;
	free_list = pcook;
}
#endif	//end VFX_DEBUG_MEMORY

#ifdef VFX_DEBUG_MEMORY
void small_alloc::dump_memory(_small_cookie * check,size_t leak_size)
{
	assert(leak_size > 0);

	size_t size = round_up(leak_size + sizeof(_small_cookie) - sizeof(_small_cookie *)) + sizeof(unsigned);
	for(register int i = 0 ; i < __pool_size ; ++i)
	{
		if (check->id != (unsigned int)-1)
		{
			if(check->file && IsBadReadPtr(check->file,4) == 0)
				__MemoryTrace(vT("%s(%d) : Memory leak! There have %d bytes memory had not be delete. Alloc ID : %d\n")
				,check->file,check->line,leak_size,check->id);
			else
				__MemoryTrace(vT("Unkown position : Memory leak! There have %d bytes memory had not be delete. Alloc ID : %d\n")
				,leak_size,check->id);
		}
		check = reinterpret_cast<_small_cookie*>((INT_PTR)check + size);
	}
}

void small_alloc::Dump()
{
	for(unsigned int i = 0; i < poolArray.GetNumber(); ++i)
		dump_memory(poolArray[i],sizeArray[i]);
}
void small_alloc::Check()
{
	for(size_t n=0; n<poolArray.GetNumber(); ++n)
	{
		size_t size = sizeArray[n];
		size_t newsize = round_up(size + sizeof(_small_cookie) - sizeof(_small_cookie *)) + sizeof(unsigned);
		_small_cookie * pcook = poolArray[n];

		for(int i=0; i<__pool_size; ++i)
		{
			//前效验位有问题，很可能越界或错误的指针
			if(pcook->dccc != c_uMalloc0xCC && pcook->id != (unsigned int)-1)
			{
				if(IsBadReadPtr(pcook->file,4) == 0)
					__MemoryTrace(vT("%s(%d) : Verify pointer(0x%8.8x) failed!!! Alloc ID : %d\n"),
								pcook->file,pcook->line,pcook->data,pcook->id);
				else
					__MemoryTrace(vT("Unkown position : Verify pointer(0x%8.8x) failed!!! Alloc ID : %d\n"),
								pcook->data,pcook->id);
			}
			unsigned int * pdccc = reinterpret_cast<unsigned int*>(((INT_PTR)pcook->data) + pcook->size);
			//后效验位有问题，很可能越界或错误的指针
			if(*pdccc != c_uMalloc0xCC && pcook->id != (unsigned int)-1)
			{
				if(IsBadReadPtr(pcook->file,4) == 0)
					__MemoryTrace(vT("%s(%d) : Verify pointer(0x%8.8x) failed! Alloc ID : %d\n"),
								pcook->file,pcook->line,pcook->data,pcook->id);
				else
					__MemoryTrace(vT("Unkown position : Verify pointer(0x%8.8x) failed! Alloc ID : %d\n"),
								pcook->data,pcook->id);
			}

			pcook = (_small_cookie *)((INT_PTR)pcook + newsize);
		}
	}
}
#endif	//end VFX_DEBUG_MEMORY
//-----------------------------------------------------------------------------
large_alloc	*	large_alloc::__plarge_alloc = 0;

large_alloc::large_alloc()
{
	memset(&header,0,sizeof(header));
}

large_alloc::~large_alloc()
{	
	destructor();
}

void large_alloc::destructor()
{
#ifdef VFX_DEBUG_MEMORY
	_vfxMemoryOutputReport();
	Dump();
#endif	//end VFX_DEBUG_MEMORY
}

#ifdef VFX_DEBUG_MEMORY
void * large_alloc::alloc(size_t size,const char * file,unsigned int line)
{
	size = round_up(size);
	_large_cookie * pcook = reinterpret_cast<_large_cookie *>
		(__alloc(size + sizeof(_large_cookie) + sizeof(int *),file,line));

	unsigned int * pdccc = reinterpret_cast<unsigned int *>((INT_PTR)pcook->cookie.data + size);
	*pdccc = c_uMalloc0xCC;
	pcook->dccc = c_uMalloc0xCC;
	pcook->next = header.next;
	pcook->prev = &header;
	if(header.next)
		header.next->prev = pcook;
	header.next = pcook;
	pcook->cookie.file = file;
	pcook->cookie.line = line;
	pcook->cookie.size = size;
	pcook->cookie.id = g_alloc_times++;
	g_memory_obj++;
	g_memoey_used += size;
	if(g_memory_max < g_memoey_used)
		g_memory_max = g_memoey_used;

	return pcook->cookie.data;
}

void large_alloc::free(void * memory,const char * file,unsigned int line)
{
	_large_cookie * pcook = reinterpret_cast<_large_cookie *>
		((INT_PTR)memory - sizeof(_large_cookie) + sizeof(_large_cookie *));
	if(pcook->dccc != c_uMalloc0xCC)//前效验位有问题，很可能越界或错误的指针
	{
		if(file)
			__MemoryTrace(vT("%s(%d) : Verify pointer(0x%8.8x) failed!!!\n"),file,line,memory);
		if(IsBadReadPtr(pcook->cookie.file,4) == 0)
			__MemoryTrace(vT("%s(%d) : Verify pointer(0x%8.8x) failed!!! Alloc ID : %d\n"),pcook->cookie.file,pcook->cookie.line,memory,pcook->cookie.id);
		else
			__MemoryTrace(vT("Unkown position : Verify pointer(0x%8.8x) failed!!! Alloc ID : %d\n"),memory,pcook->cookie.id);
		return ;
	}
	unsigned int * pdccc = reinterpret_cast<unsigned int *>((INT_PTR)memory + pcook->cookie.size);
	if(*pdccc != c_uMalloc0xCC)//后效验位有问题，很可能越界或错误的指针
	{
		if(file)
			__MemoryTrace(vT("%s(%d) : Verify pointer(0x%8.8x) failed!\n"),file,line,memory);
		if(IsBadReadPtr(pcook->cookie.file,4) == 0)
			__MemoryTrace(vT("%s(%d) : Verify pointer(0x%8.8x) failed! Alloc ID : %d\n"),pcook->cookie.file,pcook->cookie.line,memory,pcook->cookie.id);
		else
			__MemoryTrace(vT("Unkown position : Verify pointer(0x%8.8x) failed! Alloc ID : %d\n"),memory,pcook->cookie.id);
	}

	memset(memory,0xCC,pcook->cookie.size);

	g_memory_obj--;
	g_memoey_used -= pcook->cookie.size;
	if(pcook->prev)
		pcook->prev->next = pcook->next;
	if(pcook->next)
		pcook->next->prev = pcook->prev;
	__free(pcook);
}
#else
void * large_alloc::alloc(size_t size)
{
	_large_cookie * pcook = reinterpret_cast<_large_cookie *>
		(__alloc(size + sizeof(_large_cookie) - sizeof(_small_cookie *)));
	pcook->cookie.size = size;

	return (pcook == 0) ? 0 : pcook->cookie.data;
}

void large_alloc::free(void * memory)
{
	_large_cookie * pcook = reinterpret_cast<_large_cookie *>
		((INT_PTR)memory - sizeof(_large_cookie) + sizeof(_small_cookie *));
	__free(pcook);

}
#endif	//end VFX_DEBUG_MEMORY

#ifdef VFX_DEBUG_MEMORY
void large_alloc::Dump()
{
	_large_cookie * p = header.next;
	while(p)
	{
		if(p->cookie.file)
			__MemoryTrace(vT("%s(%d) : Memory leak! There have %d bytes(%d K) memory had not be delete. Alloc ID : %d\n")
			,p->cookie.file,p->cookie.line,p->cookie.size,(p->cookie.size + 1023) / 1024,p->cookie.id);
		else
			__MemoryTrace(vT("Unkown position(0x%8.8x) : Memory leak! There have %d bytes(%d K) memory had not be delete. Alloc ID : %d\n")
			,p->cookie.data,p->cookie.size,(p->cookie.size + 1023) / 1024,p->cookie.id);
		if(p->dccc != c_uMalloc0xCC)//前效验位有问题，很可能越界或错误的指针
		{
			if(IsBadReadPtr(p,p->cookie.size) == 0)
				__MemoryTrace(vT("%s(%d) : Verify pointer(0x%8.8x) failed!!! Alloc ID : %d\n")
				,p->cookie.file,p->cookie.line,p->cookie.data,p->cookie.id);
			else
				__MemoryTrace(vT("Unkown position : Verify pointer(0x%8.8x) failed!!! Alloc ID : %d\n")
				,p->cookie.data,p->cookie.id);
		}
		unsigned int * pdccc = reinterpret_cast<unsigned int *>((INT_PTR)p->cookie.data + p->cookie.size);
		if(*pdccc != c_uMalloc0xCC)//后效验位有问题，很可能越界或错误的指针
		{
			if(IsBadReadPtr(p,p->cookie.size) == 0)
				__MemoryTrace(vT("%s(%d) : Verify pointer(0x%8.8x) failed! Alloc ID : %d\n")
				,p->cookie.file,p->cookie.line,p->cookie.data,p->cookie.id);
			else
				__MemoryTrace(vT("Unkown position : Verify pointer(0x%8.8x) failed! Alloc ID : %d\n")
				,p->cookie.data,p->cookie.id);
		}
		p = p->next;
	}
}

void large_alloc::Check()
{
	_large_cookie * p = header.next;
	while(p)
	{
		if(p->dccc != c_uMalloc0xCC)//前效验位有问题，很可能越界或错误的指针
		{
			if(IsBadReadPtr(p,p->cookie.size) == 0)
				__MemoryTrace(vT("%s(%d) : Verify pointer(0x%8.8x) failed!!! Alloc ID : %d\n")
				,p->cookie.file,p->cookie.line,p->cookie.data,p->cookie.id);
			else
				__MemoryTrace(vT("Unkown position : Verify pointer(0x%8.8x) failed!!! Alloc ID : %d\n")
				,p->cookie.data,p->cookie.id);
		}
		unsigned int * pdccc = reinterpret_cast<unsigned int *>((INT_PTR)p->cookie.data + p->cookie.size);
		if(*pdccc != c_uMalloc0xCC)//后效验位有问题，很可能越界或错误的指针
		{
			if(IsBadReadPtr(p,p->cookie.size) == 0)
				__MemoryTrace(vT("%s(%d) : Verify pointer(0x%8.8x) failed! Alloc ID : %d\n")
				,p->cookie.file,p->cookie.line,p->cookie.data,p->cookie.id);
			else
				__MemoryTrace(vT("Unkown position : Verify pointer(0x%8.8x) failed! Alloc ID : %d\n")
				,p->cookie.data,p->cookie.id);
		}
		p = p->next;
	}
}
#endif	//end VFX_DEBUG_MEMORY


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//static large_alloc large_alloc::getalloc();
//static small_alloc small_alloc::getalloc();


#ifdef VFX_DEBUG_MEMORY
static int _vfxMemoryOutputReport()
{
	if(g_memory_max > 0)
	{
		__MemoryTrace(vT("\n"));
		if(g_memory_obj > 0)
			__MemoryTrace(vT("Memory leak : There have %d objects and %d bytes(%d K) memory had not be delete!\n")
			,g_memory_obj,g_memoey_used,(g_memoey_used + 1023)/1024);
		__MemoryTrace(vT("Max memory used : %u bytes(%d K)\n")
			,g_memory_max,(g_memory_max + 1023) / 1024);
		g_memory_max = 0; 
		__MemoryTrace(vT("\n"));
	}
 	return 0;
}
#endif	//end VFX_DEBUG_MEMORY


void _vfxDestructorMemory()
{
#ifdef VFX_DEBUG_MEMORY
	small_alloc::getalloc().destructor();
	large_alloc::getalloc().destructor();
#endif	//end VFX_DEBUG_MEMORY
}

}

using namespace VFX_Memory;

void * _vfxMemoryNew(size_t size,const char * file,unsigned int line)
{
	static __memory_init	memory_init;

#ifdef _MT
	auto_memory_lock	lock;
#endif //end _MT
	
#ifdef VFX_DEBUG_MEMORY
	/*if(g_alloc_times == g_alloc_break_times)
		__asm int 3;*/

	if(size > __max_bytes)
		return large_alloc::getalloc().alloc(size,file,line);
	else
		return small_alloc::getalloc().alloc(size,file,line);
#else
	if(size > __max_bytes)
		return large_alloc::getalloc().alloc(size);
	else
		return small_alloc::getalloc().alloc(size);
#endif	//end VFX_DEBUG_MEMORY
}

void _vfxMemoryDelete(void * memory,const char * file,unsigned int line)
{
	if(memory == 0)
		return ;

#ifdef _MT
	auto_memory_lock	lock;
#endif //end _MT

	_small_cookie * p = reinterpret_cast<_small_cookie *> 
		((INT_PTR)memory - sizeof(_small_cookie) + sizeof(_small_cookie *));
	
#ifdef VFX_DEBUG_MEMORY
	if(p->size > __max_bytes)
		large_alloc::getalloc().free(memory,file,line);
	else
		small_alloc::getalloc().free(memory,file,line);
#else
	if(p->size > __max_bytes)
		large_alloc::getalloc().free(memory);
	else
		small_alloc::getalloc().free(memory);
#endif	//end VFX_DEBUG_MEMORY
}


void _vfxMemoryReport()
{
#if VFX_DEBUG_MEMORY
#ifdef _MT
	auto_memory_lock	lock;
#endif //end _MT
	large_alloc::getalloc().Check();
	small_alloc::getalloc().Check();
#endif
}

void _vfxMemorySetBreak(size_t n)
{
#ifdef VFX_DEBUG_MEMORY
	g_alloc_break_times = n;
#endif
}
