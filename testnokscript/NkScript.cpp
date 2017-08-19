/*
http://www.nokuno.jp/secret/game.html
*/

#include <string.h>
#include <math.h>
#ifdef _MSC_VER
#include <windows.h>
#endif

#include "Misc.h"
#include "NkScript.h"
#include "MakeFunc.h"

//グラフィック
//CObjArray<CNkImage> g_image(SCRIPT_IMAGE_NUM);
//サウンド
//CObjArray<CNkSound> g_sound(SCRIPT_SOUND_NUM);

CToken::CToken()
{
	m_pString = NULL;
}

char* CToken::Cut(char* str, char* former)
{
	static char *str_old = 0;
	if (str_old==0) str_old = str;
	//printf("========>Cut : %d, pStr==,former:%s\n", str-str_old, former);
	if (str == NULL || str[0] == '\0')
	{
		m_pString = NULL;
		return NULL;
	}
	char* pFront = str;
	while (chrstr(pFront[0], " \t\n\r") != false)
	{
		pFront++;
		if (pFront[0] == '\0')
		{
			return NULL;
		}
	}
	//printf("=========>XXX2 %d\n", pFront - str_old);
	//Cコメントの場合
	if (strstr(pFront, "/*") == pFront)
	{
		char* p = strstr(pFront, "*/");
		if (p == NULL)
		{
			return NULL;
		}
		return Cut(p + 2,former);
	}
	//C++コメントの場合
	else if (strstr(pFront, "//") == pFront)
	{
		char* p = strstr(pFront, "\r\n");
		if (p == NULL)
		{
			return NULL;
		}
		return Cut(p + 1, former);
	}
	//定数の場合
	else if (chrstr(pFront[0], "0123456789") ||	//最初が数字
		chrstr(pFront[0], "+-") && 
		chrstr(pFront[1],"0123456789") &&
		(former==NULL ||
		!IsConst(former) && 
		!IsVar(former) && 
		strcmp(former, ")") != 0))	//符号＋数字（演算子の＋－と区別）
	{
		char* pRear = strstr2(pFront + 1, ".0123456789");
		m_pString = newstr(pFront, pRear);
		return pRear;
	}
	//文字列の場合
	else if (strstr(pFront, "\"") == pFront)
	{
		char* pRear = strchr(pFront + 1, '\"');
		if (pRear != NULL)
		{
			m_pString = newstr(pFront, pRear + 1);
		}
		return pRear + 1;
	}
	//二文字演算子の場合
	else if(strstrstr(pFront, "+=,-=,++,--,==,!=,<=,>=,||,&&",','))
	{
		m_pString = newstr(pFront, pFront + 2);
		return pFront + 2;
	}
	//その他の記号の場合
	else if (chrstr(pFront[0], "()+-*=/<>&|!#%'~^[]{}@`?:;,."))
	{
		//printf("=========>XXX %d\n", pFront + 1 - str_old);
		m_pString = newstr(pFront,pFront + 1);
		return pFront + 1;
	}
	//関数名や変数名の場合
	else if(chrstr(pFront[0],"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_"))
	{
		char* pRear = strstr2(pFront, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_0123456789");
		m_pString = newstr(pFront,pRear);
		return pRear;
	}
	else
	{
		char buffer[2] = {pFront[0], '\0'};
		MBF_ERR("%s:トークンとして認識できません。",buffer);
		return NULL;
	}
}

CTokenArray::~CTokenArray()
{
	for (int i = 0; i < m_vctpToken.size(); ++i)
	{
		SAFE_DELETE(m_vctpToken[i]);
	}
}

bool CTokenArray::CutToToken(char* str)
{
	char* p = str;
	do
	{
		char* former = NULL;
		if (m_vctpToken.size() > 0)
		{
			former = (*(m_vctpToken.end() - 1))->GetStr();
		}
		CToken* pToken = new CToken;
		p = pToken->Cut(p, former);
		if (p != NULL && pToken->GetStr() != NULL)
		{
			m_vctpToken.push_back(pToken);
		}
		else
		{
			SAFE_DELETE(pToken);
		}
	}
	while(p);
	return true;
}

void CTokenArray::allprintf()
{
	for (int i = 0; i < m_vctpToken.size(); ++i)
	{
		m_vctpToken[i]->Printf();
	}
}

int CTokenArray::GetCorrespondence(int index, const char* kakko1, const char* kakko2)
{
	int i;
	int depth = 0;
	for (i = index; i != m_vctpToken.size() && m_vctpToken[i] != NULL; ++i)
	{
		if (m_vctpToken[i]->GetStr() != NULL)
		{
			if (*m_vctpToken[i] == kakko2)
			{
				depth--;
				if (depth == 0)
				{
					return i;
				}
			}
			else if (*m_vctpToken[i] == kakko1)
			{
				depth++;
			}
		}
	}
	return -1;
}

int CTokenArray::Search(int index, const char *str)
{
	for (int i = index; i != m_vctpToken.size(); ++i)
	{
		if (*m_vctpToken[i] == str)
		{
			return i;
		}
	}
	return -1;
}

int CTokenArray::SearchCommma(int first, int last)
{
	int depth = 0;
	for (int i = first; i < last; ++i)
	{
		if (*m_vctpToken[i] == "," && depth == 0)
		{
			return i;
		}
		if (*m_vctpToken[i] == "(")
		{
			depth++;
		}
		if (*m_vctpToken[i] == ")")
		{
			depth--;
		}
	}
	return -1;
}

int CTokenArray::SearchEnzansi(const char* str, int c, int first, int last)
{
	int depth = 0;
	for (int i = first; i < last; ++i)
	{
		if (strstrstr2(m_vctpToken[i]->GetStr(), str, c) && depth == 0)
		{
			return i;
		}
		if (*m_vctpToken[i] == "(")
		{
			depth++;
		}
		if (*m_vctpToken[i] == ")")
		{
			depth--;
		}
	}
	return -1;
}

int TokenIndex::Get()const
{
	char *p = (*m_pTokenArray)[m_index]->GetStr();
	if (strstrstr2(p, "=,+=,-=", ','))
	{
		return 0;
	}
	if (strstr(p,"||") == p)
	{
		return 10;
	}
	if (strstr(p, "&&") == p)
	{
		return 20;
	}
	if (strstrstr2(p, "==,!=", ','))
	{
		return 30;
	}
	if (strstrstr2(p, "<,<=,>,>=", ','))
	{
		return 40;
	}
	if (strstrstr2(p, "+,-", ','))
	{
		return 50;
	}
	if (strstrstr2(p, "*,/,%", ','))
	{
		return 60;
	}
	//その他
	return -1;
}

CVar* CLogicalBlock::NewVariable(const char* str)
{
	for (int i = 0; i < GetVarNum(); ++i)
	{
		if (strcmp(str,GetVarNameArray()[i]) == 0)
		{
			return new CVariable(this, i);
		}
	}
	if (GetBlock())
	{
		return GetBlock()->NewVariable(str);
	}
	return NULL;
}

CLogicalBlock::~CLogicalBlock()
{
	list<CFunc*>::iterator itr;
	for(itr = m_lstpFunc.begin(); itr != m_lstpFunc.end(); ++itr)
	{
		SAFE_DELETE(*itr);
	}
	m_lstpFunc.clear();
}

bool CLogicalBlock::Read(CBlock *pBlock, CTokenArray *pTokenArray, int first)
{
	m_pBlock = pBlock;
	m_pVar = NULL;
	m_varnum = 0;
	int i;
	for (i = 0; i < VARNUM; ++i)
	{
		m_varname[i] = NULL;
	}
	int last = pTokenArray->GetCorrespondence(first, "{", "}");
	if (last == -1)
	{
		MB_ERR("{に対応する}がありません。");
		return false;
	}
	for (i = first + 1; i != last; ++i)
	{
		if (*(*pTokenArray)[i] == "double" || *(*pTokenArray)[i] == "var")
		{
			++i;
			for(; i != last && (*pTokenArray)[i] != NULL; ++i)
			{
				if (IsVar((*pTokenArray)[i]->GetStr()))
				{
#ifdef _MSC_VER
					m_varname[m_varnum] = _strdup(((*pTokenArray)[i])->GetStr());
#else
					m_varname[m_varnum] = strdup(((*pTokenArray)[i])->GetStr());
#endif
					m_varnum++;
					i++;
					if (*(*pTokenArray)[i] == ";")
					{
						break;
					}
					else if(*(*pTokenArray)[i] == ",")
					{
						continue;
					}
					else
					{
						MBF_ERR("%s:変数宣言文内のトークンとして不適切です。",
							(*pTokenArray)[i]->GetStr());
						return false;;
					}
				}
				else
				{
					MBF_ERR("%s:変数名として不適切です。",
						(*pTokenArray)[i]->GetStr());
					return false;
				}
			}
		}
		else
		{
			int last2 = i;
			CFunc* pFunc = NewFunc2(this, pTokenArray, i, last2);
			if (pFunc)
			{
				m_lstpFunc.push_back(pFunc);
			}
			i = last2;
		}
	}
	for (i = 0; i < VARNUM; ++i)
	{
		if (m_varname[i])
		{
			SAFE_DELETE_ARRAY(m_varname[i]);
		}
	}
	return true;
}

double CLogicalBlock::Get()
{
	double *pVar = NULL;
	if (m_varnum != 0)
	{
		m_pVar = new double[m_varnum];
	}
	list<CFunc*>::iterator itr;
	for (itr = m_lstpFunc.begin(); itr != m_lstpFunc.end(); ++itr)
	{
		if (*itr)
		{
			(*itr)->Get();
		}
	}
	SAFE_DELETE_ARRAY(m_pVar);
	return 0;
}

void CMemFunc::Run(CClassInstance* pInstance)
{
	m_pInstance = pInstance;
	m_pBlock->Get();
}

CVar* CMemFunc::NewVariable(const char *str)
{
	if (strcmp(str, "x") == 0)
	{
		return new CMemVar(this, -1);
	}
	else if (strcmp(str, "y") == 0)
	{
		return new CMemVar(this, -2);
	}
	else if (strcmp(str, "img") == 0)
	{
		return new CMemVar(this, -3);
	}
	else if (strcmp(str, "hp") == 0)
	{
		return new CMemVar(this, -4);
	}
	else if (strcmp(str, "anm")==0)
	{
		return new CMemVar(this, -5);
	}
	else if (strcmp(str,"px") == 0)
	{
		return new CMemVar(this,-10);
	}
	else if (strcmp(str, "py") == 0)
	{
		return new CMemVar(this, -11);
	}
	for (int i = 0; i < GetVarNum(); ++i)
	{
		if (strcmp(str,GetVarNameArray()[i]) == 0)
		{
			return new CMemVar(this, i);
		}
	}
	return NULL;
}

bool CMemFunc::Read(CClassDefine *pClassDef,CTokenArray* pTokenArray,int first)
{
	//FIXME:
	//Outputf("%s\n",(*pTokenArray)[first]->GetStr());
	m_pClassDef = pClassDef;
	SAFE_DELETE(m_pBlock);
	m_pBlock = new CLogicalBlock;
	return m_pBlock->Read(this,pTokenArray,first);
}

CClassInstance::CClassInstance(CClassDefine *pDefine, CEnemy *pEnemy)
{
	m_pDefine = pDefine;
	m_pEnemy = pEnemy;
	m_vararray = new double[pDefine->GetVarNum()];
	for (int i = 0; i < pDefine->GetVarNum(); ++i)
	{
		m_vararray[i] = 0;
	}
	if (pDefine->GetConstructer() != NULL)
	{
		pDefine->GetConstructer()->Run(this);
	}
}

bool CClassDefine::Read(CTokenArray *pTokenArray, int first, char *str)
{
	m_varnum = 0;
	int i;
	for (i = 0;i < VARNUM; ++i)
	{
		m_varname[i] = NULL;
	}
	int last = pTokenArray->GetCorrespondence(first, "{", "}");
	if (last == -1)
	{
		MB_ERR("{に対応する}がありません。");
		return false;
	}
	for (i = first + 1; i != last; ++i)
	{
		if(*(*pTokenArray)[i] == "double" || 
			*(*pTokenArray)[i] == "var")
		{
			++i;
			for (; i!=last && (*pTokenArray)[i] != NULL; ++i)
			{
				if (IsVar((*pTokenArray)[i]->GetStr()))
				{
#ifdef _MSC_VER
					m_varname[m_varnum] = _strdup(((*pTokenArray)[i])->GetStr());
#else
					m_varname[m_varnum] = strdup(((*pTokenArray)[i])->GetStr());
#endif
					m_varnum++;
					i++;
					if (*(*pTokenArray)[i] == ";")
					{
						break;
					}
					else if(*(*pTokenArray)[i]==",")
					{
						continue;
					}
					else
					{
						MBF_ERR("%s:変数宣言文内のトークンとして不適切です。",
							(*pTokenArray)[i]->GetStr());
						return false;
					}
				}
				else
				{
					MBF_ERR("%s:変数名として不適切です。",
						(*pTokenArray)[i]->GetStr());
					return false;
				}
			}
		}
		else if (IsVar((*pTokenArray)[i]->GetStr()) && 
			*(*pTokenArray)[i+1] == "(") //メンバ関数
		{
			int kakko = pTokenArray->GetCorrespondence(i + 1, "(", ")");
			if(kakko<0)
			{
				MBF_ERR("%s:メンバ関数に)がありません。",
					(*pTokenArray)[i]->GetStr());
				return false;
			}
			int tyukakko = pTokenArray->GetCorrespondence(kakko + 1, "{", "}");
			if (tyukakko < 0)
			{
				MBF_ERR("%s:メンバ関数に{}がありません。",
					(*pTokenArray)[i]->GetStr());
				return false;
			}
			i = tyukakko;
		}
		else
		{
			MBF_ERR("%s:文として認識できません。",
				(*pTokenArray)[i]->GetStr());
			return false;
		}
	}
	int funcname = pTokenArray->Search(first,str);
	if (funcname < 0 || funcname >= last)
	{
		m_pConstructer = NULL;
	}
	else
	{
		m_pConstructer = new CMemFunc;
		if (!m_pConstructer->Read(this, pTokenArray, funcname + 3))
		{
			SAFE_DELETE(m_pConstructer);
		}
	}
	funcname = pTokenArray->Search(first, "main");
	if (funcname < 0 || funcname >= last)
	{
		MBF_ERR("main関数がありません。");
		return false;
	}
	m_pMemFunc = new CMemFunc;
	return m_pMemFunc->Read(this, pTokenArray, funcname + 3);
}

CNkScript::CNkScript(const char* filename)
{
	int size = GetFileSize(filename);
	char *buffer = new char[size + 1];
	buffer[size] = 0;
	ReadFile(buffer, size, filename);
	//printf("%s\n", buffer);
	if (buffer == NULL)
	{
		MBF_ERR("%sがありません。", filename);
		throw;
	}
	CTokenArray tArray;
	tArray.CutToToken(buffer);

	//FIXME:
	tArray.allprintf();
	printf("%d\n", tArray.GetArray()->size());
	fflush(stderr);
	fflush(stdout);
	for (int i = 0; i < tArray.GetArray()->size() && i >= 0; ++i)
	{
		if (*tArray[i] == "class")
		{
			m_pCDefine.push_back(new CLASSDEFINE(&tArray, i + 1));
			i = tArray.GetCorrespondence(i + 2, "{", "}");
		}
		else if (IsVar(tArray[i]->GetStr()))
		{
			int kakko = tArray.GetCorrespondence(i + 1, "(", ")");
			if (kakko < 0)
			{
				MBF_ERR("%s: function not found", tArray[i]->GetStr());
				throw;
			}
			int tyukakko = tArray.GetCorrespondence(kakko + 1, "{", "}");
			if (tyukakko < 0)
			{
				MBF_ERR("%s:function {} not found.",tArray[i]->GetStr());
				throw;
			}
			m_pFunc.push_back(new FUNCDEFINE(&tArray, i));
			i = tyukakko;
		}
		else if (*tArray[i] != ";")
		{
			MBF_ERR("%s: not recognized.",tArray[i]->GetStr());
			throw;
		}
	}
	SAFE_DELETE_ARRAY(buffer);
}

CNkScript::~CNkScript()
{
	int i;
	for (i = 0; i < m_pCDefine.size(); ++i)
	{
		SAFE_DELETE(m_pCDefine[i]);
	}
	for (i = 0; i < m_pFunc.size(); ++i)
	{
		SAFE_DELETE(m_pFunc[i]);
	}
	/* FIXME: */
	/*
	g_image.DeleteAll();
	g_sound.DeleteAll();
	*/
}

int CNkScript::SearchClass(const char *name)
{
	for (int i = 0; i < m_pCDefine.size(); ++i)
	{
		if (strcmp(m_pCDefine[i]->m_name, name) == 0)
		{
			return i;
		}
	}
	return -1;
}

int CNkScript::SearchFunc(const char *name)
{
	for (int i = 0; i < m_pFunc.size(); ++i)
	{
		if (strcmp(m_pFunc[i]->m_name, name)==0)
		{
			return i;
		}
	}
	return -1;
}

