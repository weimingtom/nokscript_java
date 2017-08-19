#ifdef _MSC_VER
#include <windows.h>
#endif

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>

#include "Misc.h"

void Outputf(const char* str,...)
{
	char* buffer = new char[strlen(str) + 256];
	va_list args;
	va_start(args, str);
	vsprintf(buffer,str,args);
	va_end(args);
#ifdef _MSC_VER
	OutputDebugString(buffer);
#else
	printf("%s", buffer);
#endif
	SAFE_DELETE_ARRAY(buffer);
}

int MBF_ERR(const char* str,...)
{
	char* buffer = new char[strlen(str) + 256];
	va_list args;
	va_start(args, str);
	vsprintf(buffer, str, args);
	va_end(args);
#ifdef _MSC_VER
	int ret = MessageBox(NULL, buffer, "¥¹¥¯¥ê¥×¥È¥¨¥é©`", MB_OK | MB_ICONSTOP);
#else
	int ret = 0;
	fprintf(stderr, "%s\n", buffer);
#endif
	SAFE_DELETE_ARRAY(buffer);
	return ret;
}

char* strstr2(char* str, const char* chrArray)
{
	char* p1;
	for (p1 = str; p1[0] != '\0'; ++p1)
	{
		bool ret = true;
		for (const char* p2 = chrArray; p2[0] != '\0'; ++p2)
		{
			if(p1[0] == p2[0])
			{
				ret = false;
			}
		}
		if (ret)
		{
			return p1;
		}
	}
	return p1;
}

bool chrstr(int chr, const char* chrArray)
{
	for (const char* p = chrArray; p[0]; ++p)
	{
		if (p[0]==chr)
		{
			return true;
		}
	}
	return false;
}

char* newstr(const char* pFront, char* pRear)
{
	char* ret = new char[pRear - pFront + 1];
	strncpy(ret, pFront, pRear - pFront);
	ret[pRear - pFront] = '\0';
	return ret;
}

bool strstrstr(char *str1, const char *str2, int c)
{
	const char *pFront = str2;
	char *pRear;
	while (pRear = strchr((char *)pFront, c))
	{
		char* buffer = newstr(pFront, pRear);
		if (strstr(str1, buffer) == str1)
		{
			SAFE_DELETE_ARRAY(buffer);
			return true;
		}
		SAFE_DELETE_ARRAY(buffer);
		pFront = pRear + 1;
	}
	if (strstr(str1, pFront) == str1)
	{
		return true;
	}
	return false;
}

bool strstrstr2(char *str1, const char *str2, int c)
{
	const char* pFront = str2;
	char* pRear;
	while(pRear = strchr((char *)pFront, c))
	{
		char* buffer = newstr(pFront, pRear);
		if (strcmp(str1, buffer) == 0)
		{
			SAFE_DELETE_ARRAY(buffer);
			return true;
		}
		SAFE_DELETE_ARRAY(buffer);
		pFront = pRear + 1;
	}
	if(strcmp(str1,pFront)==0)
	{
		return true;
	}
	return false;
}

bool IsConst(char *str)
{
	return chrstr(str[0], "0123456789") ||
		chrstr(str[0], "+-.") && 
		chrstr(str[1], "0123456789");
}

bool IsVar(char* str)
{
	return chrstr(str[0], "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_") &&
		strstr2(str, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_0123456789")[0] == '\0';
}

bool ReadFile(char *buffer, int size, const char *lpszFileName)
{
#ifdef _MSC_VER
	HANDLE hFile;
	hFile = CreateFile(lpszFileName,
		GENERIC_READ,
		FILE_SHARE_READ|FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		return 0;
	}
	DWORD dwSize;
	int ret = ReadFile(
		hFile,
		buffer,
		size,
		&dwSize,
		NULL);
	ret &= CloseHandle(hFile);
	if (ret)
	{
		buffer[dwSize] = 0;
	}
	return ret != 0;
#else
	FILE *fp = fopen(lpszFileName, "r");
	if (fp)
	{	
		fread(buffer, size, 1, fp);
		fclose(fp);
		return true;
	}
	else
	{
		return false;
	}
#endif
}

char* ReadFile(const char *lpszFileName)
{
#ifdef _MSC_VER
	HANDLE hFile;
	hFile = CreateFile(lpszFileName,
		GENERIC_READ,
		FILE_SHARE_READ|FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return 0;
	}
	DWORD size;
	size = GetFileSize(hFile,NULL);
	char *ret = new char[size];
	DWORD dwSize;
	int flag = ReadFile(
		hFile,
		ret,
		size,
		&dwSize,
		NULL);
	flag &= CloseHandle(hFile);
	if (flag)
	{
		ret[dwSize] = 0;
		return ret;
	}
	else
	{
		SAFE_DELETE_ARRAY(ret);
		return NULL;
	}
#else
	int size = GetFileSize(lpszFileName);
	char *ret = new char[size];
	if (size < 0)
	{
		return NULL;
	}
	FILE *fp = fopen(lpszFileName, "r");
	if (fp)
	{	
		fread(ret, size, 1, fp);
		fclose(fp);
		return ret;
	}
	else
	{
		return NULL;
	}
#endif
}

int GetFileSize(const char *lpszFileName)
{
#ifdef _MSC_VER
	HANDLE hFile;
	hFile = CreateFile(lpszFileName,
		GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return 0;
	}
	int ret = GetFileSize(hFile, NULL);
	CloseHandle(hFile);
	return ret;
#else
	FILE *fp = fopen(lpszFileName, "r");
	int ret;	
	if (fp)
	{	
		fseek(fp, 0L, SEEK_END);
		ret = ftell(fp);
		fclose(fp);
		return ret;
	}
	else
	{
		return -1;
	}
#endif
}
