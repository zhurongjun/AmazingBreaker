#pragma once

//方向枚举
enum EDIR 
{
	eDIR_UP = 1,
	eDIR_RIGHT = 1 << 1,
	eDIR_DOWM = 1 << 2,
	eDIR_LEFT = 1 << 3,
};

//颜色枚举
enum ECOLOR {
	ECOLOR_PINK = 0,
	ECOLOR_BLUE = 1,
	ECOLOR_GREEN = 2,
	ECOLOR_BIG = 3
};

#ifndef TCHARTOOL
inline void Anis_To_Auto(const char *ascStr, int aSize, TCHAR *tStr, int tSize)
{
#ifdef _UNICODE	//根据字符集进行指定的转换
	MultiByteToWideChar(CP_ACP, MB_COMPOSITE, ascStr, aSize, tStr, tSize);
#else
	strcpy(tStr, ascStr);
#endif // _UNICODE
}

inline void Auto_To_Anis(const TCHAR *tstr, int tsize, char *ascStr, int asize)
{
#ifdef _UNICODE	//根据字符集进行指定的转换
	WideCharToMultiByte(CP_ACP, 0, tstr, tsize, ascStr, asize, 0, 0);
#else
	strcpy(ascStr, tstr);
#endif // _UNICODE
}

inline void AlphaBlend(int Blend, BYTE * destColor, BYTE * srcColor)
{
	*destColor = ((*destColor * (255 - Blend)) + (*srcColor * Blend)) / 255.0;
}

#define TCHARTOOL
#endif // TCHARTOOL