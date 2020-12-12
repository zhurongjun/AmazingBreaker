// vfxmemory.h
// 
// VictoryCore Code
// memory allocer
//
// NOTICE:
//		我希望vfxmemory.h vfxmemory.cpp是非常独立的两个文件
// 这两个文件的更改不要影响其他代码，也不依赖别的代码
// 这也是有内存分配的机制决定的
// 
// Author : lanzhengpeng(兰征鹏)
// More author : renwind(任晓宇)
// Create time : 2002-6-13
// Modify time : 2006-12-20 授课时修改, 从引擎剥离, 去掉日志,平台相关等部分, 使代码易于理解
//-----------------------------------------------------------------------------
#ifndef __VFX_MEMORY_H__
#define __VFX_MEMORY_H__

#pragma once

#include <tchar.h>
#include <assert.h>
#include <time.h>
#include <crtdbg.h>

#if _DEBUG
#	define		VFX_DEBUG_MEMORY		1
#else
//#	define		VFX_DEBUG_MEMORY		1
#	undef		VFX_DEBUG_MEMORY
#endif

namespace	VFX_Memory
{
#pragma pack(push)
#pragma pack(4)

	inline void * __alloc(size_t size)
		{	return HeapAlloc(GetProcessHeap(),0,size); }
	inline void __free(void * pmem)
		{	HeapFree(GetProcessHeap(),0,pmem); }
	inline void * __alloc(size_t size,LPCSTR file,DWORD line)
		{	return HeapAlloc(GetProcessHeap(),0,size); }
//		{	return _malloc_dbg(size,_NORMAL_BLOCK,file,line); }

	enum
	{
		__align_size = 8,
		__pool_size = 128,
		__max_bytes = 256
	};

	inline size_t round_up(size_t bytes)
	{
		return (bytes + __align_size - 1) & ~((size_t)__align_size - 1);
	}

	template<class T>
	class vfxMArray
	{
		T *				m_pBuffer;
		size_t			m_nSize;
		size_t			m_nNumber;
	public:
		vfxMArray()
		:m_pBuffer(0),m_nSize(0),m_nNumber(0)
		{	}
		~vfxMArray()
		{	m_nSize = m_nNumber = 0;__free(m_pBuffer); }
		T * GetData()
		{	return m_pBuffer; }
		size_t GetNumber()
		{	return m_nNumber; }
		T & operator [](size_t n)
		{
			return m_pBuffer[n];
		}
		void Add(T & ref);
	};

	template<class T>
	void vfxMArray<T>::Add(T & ref)
	{
		if(m_nNumber >= m_nSize)
		{
			m_nSize += __pool_size;
			T * pNew = reinterpret_cast<T*>(__alloc(sizeof(T) * m_nSize));
			memcpy(pNew,m_pBuffer,m_nNumber * sizeof(T));
			__free(m_pBuffer);
			m_pBuffer = pNew;
		}
		m_pBuffer[m_nNumber] = ref;
		m_nNumber++;
	}

	class _small_cookie
	{
	public:
	#ifdef VFX_DEBUG_MEMORY
		const char * file;
		unsigned int line;
		size_t		 id;
	#endif
		size_t		 size;
	#ifdef VFX_DEBUG_MEMORY
		unsigned int dccc;
	#endif
		union
		{
			_small_cookie * next;
			char	data[1];
		};
	};

	class _large_cookie
	{
	public:
	#ifdef VFX_DEBUG_MEMORY
		_large_cookie * prev;
		_large_cookie * next;
		unsigned int	dccc;
	#endif
		_small_cookie	cookie;
	};

	class __memory_init;

	class small_alloc
	{
		static small_alloc	*	__psmall_alloc;

		_small_cookie *					pool_chunk[__max_bytes / __align_size];
		vfxMArray<_small_cookie *>		poolArray;
	#ifdef VFX_DEBUG_MEMORY
		vfxMArray<size_t>				sizeArray;

		void Dump();
		void dump_memory(_small_cookie * check,size_t size);
	#endif //end VFX_DEBUG_MEMORY
		small_alloc();
		_small_cookie *	alloc_pool(size_t size);

	public:
		~small_alloc();
		void destructor();

	#ifdef VFX_DEBUG_MEMORY
		void * alloc(size_t size,const char * file,unsigned int line);
		void free(void * memory,const char * file,unsigned int line);
		void Check();
	#else
		void * alloc(size_t size);
		void free(void * memory);
	#endif
		static small_alloc & getalloc()
		{	return *__psmall_alloc; }
		
		friend __memory_init;
	};

	class large_alloc
	{
		static large_alloc	*	__plarge_alloc;
		
		_large_cookie		header;

		large_alloc();
	#ifdef VFX_DEBUG_MEMORY
		void Dump();
	#endif //end _DEBUG
	public:
		~large_alloc();
		void destructor();

	#ifdef VFX_DEBUG_MEMORY
		void * alloc(size_t size,const char * file,unsigned int line);
		void free(void * memory,const char * file,unsigned int line);
		void Check();
	#else
		void * alloc(size_t size);
		void free(void * memory);
	#endif
		static large_alloc & getalloc()
		{	return *__plarge_alloc; }

		friend __memory_init;
	};

	class __memory_init
	{
	public:
		__memory_init()
		{
			static large_alloc		_large_alloc;
			static small_alloc		_small_alloc;
			if(small_alloc::__psmall_alloc == 0)
			{
				small_alloc::__psmall_alloc = &_small_alloc;
				large_alloc::__plarge_alloc = &_large_alloc;
			}
		}
	};

#pragma pack(pop)
}

#endif //end __VFX_MEMORY_H__
