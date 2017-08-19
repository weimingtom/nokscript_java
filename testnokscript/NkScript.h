#pragma once

#include <string.h>
#include <math.h>
#include <stdio.h>

#include <list>
#include <vector>
#include <algorithm>
using namespace std;
#include "Misc.h"

#ifdef _MSC_VER
#include <windows.h>
#define MB_ERR(str) MessageBox(NULL, str, "スクリプトエラー", MB_OK | MB_ICONSTOP)
#else
#include <stdio.h>
#define MB_ERR(str) fprintf(stderr, "%s\n", str)
#endif

class CFunc;
class CBlock;
class CConst;
class CVar;
class CVariable;
class CToken;
class CTokenArray;

class CClassDefine;
class CClassInstance;
class CMemFunc;
class CMemVar;

class CToken
{
public:
	CToken();
	~CToken()
	{
		SAFE_DELETE_ARRAY(m_pString);
	}
	char* Cut(char* str,char* former);
	char* GetStr() const
	{
		return m_pString;
	}
	void Printf()
	{
		//FIXME:
		//Outputf("%s\n",m_pString);
		printf("%s\n",m_pString);
	}
	bool operator ==(const char* str)const
	{
		return m_pString != NULL && strcmp(m_pString,str) == 0;
	}
	bool operator !=(const char* str)const
	{
		return m_pString != NULL && strcmp(m_pString,str) != 0;
	}
	operator char*()const
	{
		return GetStr();
	}
	operator const char*()const
	{
		return GetStr();
	}
private:
	char *m_pString;
};

inline bool operator==(const char* str, CToken token)
{
	return token == str;
};

class CTokenArray
{
public:
	CTokenArray(){};
	~CTokenArray();
	bool CutToToken(char* str);
	int GetCorrespondence(int index,const char* kakko1,const char* kakko2);
	int Search(int index, const char* str);
	int SearchCommma(int first, int last);
	int SearchEnzansi(const char *str, int c, int first, int last);
	vector<CToken*>* GetArray()
	{
		return &m_vctpToken;
	}
	CToken* operator[](int i)
	{
		return m_vctpToken[i];
	}
	void allprintf();
private:
	vector<CToken*> m_vctpToken;
};

struct TokenIndex
{
	int m_index;
	CTokenArray *m_pTokenArray;
	int Get() const;
	bool operator < (const TokenIndex& index) const
	{
		return Get() > index.Get();
	}
	bool operator > (const TokenIndex& index) const
	{
		return Get() < index.Get();
	}
};

class CFunc
{
public:
	virtual double Get() = 0;
};

#define VARNUM 256

class CBlock : public CFunc
{
public:
	virtual double Get() = 0;
	virtual CVar* NewVariable(const char *str) = 0;
	virtual double* GetVarArray() = 0;
	virtual int GetVarNum() = 0;
};

class CLogicalBlock : public CBlock
{
public:
	CLogicalBlock()
	{
		m_pVar = NULL;
		m_varnum = 0;
		m_pBlock = NULL;
	}
	~CLogicalBlock();
	virtual double Get();
	virtual CVar* NewVariable(const char* str);
	bool Read(CBlock* pBlock, CTokenArray* pTokenArray, int first);
public:
	virtual double* GetVarArray()
	{
		return m_pVar;
	}
	virtual int GetVarNum()
	{
		return m_varnum;
	}
	virtual char** GetVarNameArray()
	{
		return m_varname;
	}
	virtual CBlock* GetBlock()
	{
		return m_pBlock;
	}
protected:
	list<CFunc*> m_lstpFunc; 
	double* m_pVar;
	int m_varnum;
	char* m_varname[VARNUM];
	CBlock* m_pBlock;
};

class CConst : public CFunc
{
public:
	CConst(double value)
	{
		m_value = value;
	}
	~CConst()
	{

	}
	virtual double Get()
	{
		return m_value;
	}
private:
	double m_value;
};

class CVar : public CFunc
{
public:
	virtual double Get() = 0;
	virtual double Set(CFunc *pValue) = 0;
	virtual double Set(double value) = 0;
};

class CVariable : public CVar
{
public:
	CVariable(CBlock *pBlock,int index)
	{
		m_pBlock = pBlock;
		m_index = index;
	}
	~CVariable()
	{

	}
	virtual double Get()
	{
		return m_pBlock->GetVarArray()[m_index];
	}
	virtual double Set(CFunc* pValue)
	{
		m_pBlock->GetVarArray()[m_index] = pValue->Get();
		return Get();
	}
	virtual double Set(double value)
	{
		m_pBlock->GetVarArray()[m_index] = value;
		return Get();
	}
private:
	CBlock* m_pBlock; //親オブジェクト
	int m_index; //CBlockの変数配列のインデックス
};

class CClassDefine
{
public:
	CClassDefine()
	{
		m_pMemFunc = NULL;
		m_varnum = 0;
	}
	inline ~CClassDefine();
	bool Read(CTokenArray* pTokenArray, int first, char* str);
public:
	int GetVarNum()
	{
		return m_varnum;
	}
	char** GetVarNameArray()
	{
		return m_varname;
	}
	CMemFunc* GetMemFunc()
	{
		return m_pMemFunc;
	}
	CMemFunc* GetConstructer()
	{
		return m_pConstructer;
	}
private:
	CMemFunc* m_pMemFunc; //メンバ関数func
	CMemFunc* m_pConstructer; //コンストラクタ
	int m_varnum; //メンバ変数の数
	char* m_varname[VARNUM];
};

/*//クラスのインスタンス
class CClassInstance
{
public:
	CClassInstance(CClassDefine* pDefine);
	~CClassInstance(){SAFE_DELETE_ARRAY(m_vararray);};
	inline void Run();//実行
	double* GetVarArray(){return m_vararray;};
private:
	CClassDefine* m_pDefine;	//型の種類(vtable)
	double* m_vararray;		//メンバ変数
};
*/

class CEnemy;

class CClassInstance
{
public:
	CClassInstance(CClassDefine* pDefine,CEnemy* pEnemy);	//引数が違う
	~CClassInstance(){SAFE_DELETE_ARRAY(m_vararray);};
	inline void Run();//実行
	double* GetVarArray(){return m_vararray;};
	CEnemy* GetEnemy(){return m_pEnemy;};
private:
	CClassDefine* m_pDefine;
	double* m_vararray;
	CEnemy* m_pEnemy;
};

class CMemFunc : public CBlock
{
public:
	CMemFunc()
	{
		m_pClassDef = NULL;
		m_pInstance = NULL;
		m_pBlock = NULL;
	}
	~CMemFunc()
	{
		SAFE_DELETE(m_pBlock);
	}
	virtual double Get()
	{
		return 0;
	}
	virtual CVar* NewVariable(const char* str);	//CMemVarをnewする
	virtual char** GetVarNameArray()
	{
		return m_pClassDef->GetVarNameArray();
	}
	virtual int GetVarNum(){return m_pClassDef->GetVarNum();};
	virtual double* GetVarArray(){return m_pInstance->GetVarArray();};
	bool Read(CClassDefine *pClassDef,CTokenArray* pTokenArray,int first);
	void Run(CClassInstance* pInstance);
public:
	CClassInstance* GetInst(){return m_pInstance;};
private:
	CClassDefine* m_pClassDef;
	CClassInstance* m_pInstance;
	CLogicalBlock* m_pBlock;
};

class CMemVar : public CVar
{
public:
	CMemVar(CMemFunc *pMemFunc, int index)
	{
		m_pMemFunc = pMemFunc;
		m_index = index;
	}
	double &GetVar()
	{
		if(m_index >= 0)
		{
			return m_pMemFunc->GetInst()->GetVarArray()[m_index];
		}
		/*FIXME: */
		/*
		switch(m_index)
		{
		case -1:
			return m_pMemFunc->GetInst()->GetEnemy()->GetX();
			break;
		case -2:
			return m_pMemFunc->GetInst()->GetEnemy()->GetY();
			break;
		case -3:
			return m_pMemFunc->GetInst()->GetEnemy()->GetImage();
			break;
		case -4:
			return m_pMemFunc->GetInst()->GetEnemy()->GetHP();
			break;
		case -5:
			return m_pMemFunc->GetInst()->GetEnemy()->GetAnm();
			break;
		case -10:
			return g_pPlayer->GetX();
			break;
		case -11:
			return g_pPlayer->GetY();
			break;
		}
		*/
		//FIXME:
		return m_pMemFunc->GetInst()->GetVarArray()[m_index];
	}
	virtual double Get()
	{
		return GetVar();
	}
	virtual double Set(CFunc* pValue)
	{
		return GetVar() = pValue->Get();
	}
	virtual double Set(double value)
	{
		return GetVar()=value;
	}
private:
	CMemFunc *m_pMemFunc;
	int m_index;
};

void CClassInstance::Run()
{
	m_pDefine->GetMemFunc()->Run(this);
}

CClassDefine::~CClassDefine()
{
	SAFE_DELETE(m_pMemFunc);
	for (int i = 0; i < VARNUM; ++i)
	{
		SAFE_DELETE_ARRAY(m_varname[i]);
	}
}

struct CLASSDEFINE
{
	CClassDefine m_define;
	char* m_name;
	CLASSDEFINE(CTokenArray* pTA,int first)
	{
#ifdef _MSC_VER
		m_name = _strdup((*pTA)[first]->GetStr());
#else
		m_name = strdup((*pTA)[first]->GetStr());
#endif
		m_define.Read(pTA, first + 1, m_name);
	}
	~CLASSDEFINE()
	{
		SAFE_DELETE_ARRAY(m_name);
	}
};

struct FUNCDEFINE
{
	CLogicalBlock m_func;
	char* m_name;

	FUNCDEFINE(CTokenArray* pTA, int first)
	{
#ifdef _MSC_VER
		m_name = _strdup((*pTA)[first]->GetStr());
#else
		m_name = strdup((*pTA)[first]->GetStr());
#endif
		if(*(*pTA)[first+1]!="(")
		{
			MBF_ERR("%s:関数に（がありません。\n",
				(*pTA)[first]->GetStr());
		}
		int i = pTA->Search(first, ")");
		if (i >= 0)
		{
			m_func.Read(NULL, pTA, i + 1);
		}
	}
	~FUNCDEFINE()
	{
		SAFE_DELETE_ARRAY(m_name);
	}
};

class CNkScript
{
public:
	CNkScript(const char* filename);
	~CNkScript();
	int SearchClass(const char *name);
	int SearchFunc(const char *name);
	vector<CLASSDEFINE*>* GetClass()
	{
		return &m_pCDefine;
	}
	vector<FUNCDEFINE*>* GetFunc()
	{
		return &m_pFunc;
	}
private:
	vector<CLASSDEFINE*> m_pCDefine;
	vector<FUNCDEFINE*> m_pFunc;
};
