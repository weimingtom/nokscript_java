#pragma once

#include <stdlib.h>

#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete [] (p);	  (p)=NULL; } }

extern void Outputf(const char* str,...);
extern int MBF_ERR(const char* str,...);
extern int msgf(char *text,char *caption,unsigned int type,...);
extern bool ReadFile(char* buffer, int size, const char* lpszFileName);
extern char* ReadFile(const char* lpszFileName);
extern bool chrstr(int chr, const char* chrArray);
extern char* strstr2(char* str, const char* chrArray);
extern char* newstr(const char* pFront, char* pRear);
extern bool strstrstr(char* str1, const char* str2,int c);
extern bool strstrstr2(char* str1, const char* str2, int c);
extern int GetFileSize(const char* lpszFileName);
bool IsConst(char* str);
bool IsVar(char* str);
