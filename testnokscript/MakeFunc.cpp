#ifdef _MSC_VER
#include <windows.h>
#endif

#include "Misc.h"
#include "NkScript.h"
#include "DxFunc.h"
#include "MakeFunc.h"
#include "HelpFunc.h"
#include "OpFunc.h"
#include "CtrlFunc.h"

CFunc* NewFunc2(CBlock *pBlock, CTokenArray *pTokenArray, int first, int &last)
{
	int i = first;
	if (*(*pTokenArray)[i] == ";")
	{
		last++;
		return NewFunc2(pBlock, pTokenArray, i + 1, last);
	}
	else if (*(*pTokenArray)[i] == "{")
	{
		last = pTokenArray->GetCorrespondence(i, "{", "}");
		if (last < 0)
		{
			return NULL;
		}
		CLogicalBlock *pBlock2 = new CLogicalBlock;
		if (pBlock2->Read(pBlock, pTokenArray, i))
		{
			return pBlock2;
		}
		else
		{
			SAFE_DELETE(pBlock);
			return NULL;
		}
	}
	else if (*(*pTokenArray)[i] == "if")
	{
		if (*(*pTokenArray)[i + 1] != "(")
		{
			MB_ERR("ifに(がありません。");
			return NULL;
		}
		int last2 = pTokenArray->GetCorrespondence(i + 1, "(", ")");
		if (last2 < 0)
		{
			MB_ERR("ifに)がありません。");
			return NULL;
		}
		CFunc *pJoken = NewFunc(pBlock, pTokenArray, i + 2, last2);	//条件式
		CFunc *pFunc = NewFunc2(pBlock, pTokenArray, last2 + 1, last);
		CFunc *pElse = NULL;
		if (*(*pTokenArray)[last + 1] == "else")
		{
			pElse = NewFunc2(pBlock, pTokenArray, last + 2, last);
		}
		if (pJoken && pFunc)
		{
			return new CIf(pJoken, pFunc, pElse);
		}
		SAFE_DELETE(pJoken);
		SAFE_DELETE(pBlock);
		SAFE_DELETE(pElse);
		return NULL;
	}
	else if (*(*pTokenArray)[i] == "while") //while
	{
		if (*(*pTokenArray)[i + 1] != "(")
		{
			MB_ERR("whileに(がありません。");
			return NULL;
		}
		int last2 = pTokenArray->GetCorrespondence(i + 1, "(", ")");
		if (last2 < 0)
		{
			MB_ERR("whileに)がありません。");
			return NULL;
		}
		CFunc *pJoken = NewFunc(pBlock, pTokenArray, i + 2, last2);	//条件式
		CFunc *pFunc = NewFunc2(pBlock, pTokenArray, last2 + 1, last);
		if (pJoken && pFunc)
		{
			return new CWhile(pJoken, pFunc);
		}
		SAFE_DELETE(pJoken);
		SAFE_DELETE(pFunc);
		return NULL;
	}
	else if (*(*pTokenArray)[i] == "for")
	{
		if (*(*pTokenArray)[i + 1] != "(")
		{
			MB_ERR("forに(がありません。");
			return NULL;
		}
		int last2 = pTokenArray->GetCorrespondence(i + 1, "(", ")");
		if (last2 < 0)
		{
			MB_ERR("forに)がありません。");
			return NULL;
		}
		int colon1 = pTokenArray->Search(i + 1, ";");
		if (colon1 < 0)
		{
			MB_ERR("forに;がありません。");
			return NULL;
		}
		int colon2 = pTokenArray->Search(colon1 + 1, ";");
		if (colon2 < 0)
		{
			MB_ERR("forに;がありません。");
			return NULL;
		}
		CFunc *pFirst = NewFunc(pBlock, pTokenArray, i + 2, colon1);
		CFunc *pJoken = NewFunc(pBlock, pTokenArray, colon1 + 1, colon2);
		CFunc *pStep = NewFunc(pBlock, pTokenArray, colon2 + 1, last2);
		CFunc *pFunc = NewFunc2(pBlock, pTokenArray, last2 + 1, last);
		if (pFirst && pJoken && pStep && pFunc)
		{
			return new CFor(pFirst,pJoken,pStep,pFunc);
		}
		SAFE_DELETE(pFirst);
		SAFE_DELETE(pJoken);
		SAFE_DELETE(pStep);
		SAFE_DELETE(pFunc);
		return NULL;
	}
	else if (*(*pTokenArray)[i] == "break")
	{
		int gyou = pTokenArray->Search(i, ";");
		if (gyou != i + 1)
		{
			MB_ERR("breakに;がありません。");
			return NULL;
		}
		last = gyou;
		return new CBreak();
	}
	else
	{
		int gyou = pTokenArray->Search(i, ";");
		last = gyou;
		if (gyou == -1)
		{
			MB_ERR(";がありません。");
			return NULL;
		}
		CFunc *pFunc = NewFunc(pBlock, pTokenArray, i, gyou);
		if (pFunc)
		{
			return pFunc;
		}
	}
	return NULL;
}

CFunc* NewFunc(CBlock *pBlock, CTokenArray *pTokenArray, int first, int last)
{
	CFunc *ret = NULL;
	int i = first;
	if (IsConst((*pTokenArray)[i]->GetStr()) && 
		last == i + 1)
	{
		return new CConst(atof((*pTokenArray)[i]->GetStr()));
	}
	if (*(*pTokenArray)[i] == "(" && 
		pTokenArray->GetCorrespondence(i, "(", ")") == last - 1)
	{
		return NewFunc(pBlock, pTokenArray, first + 1, last - 1);
	}
	if (pTokenArray->SearchEnzansi("=,+=,-=,||,&&,==,!=,<,<=,>,>=,+,-,*,/,%", ',', first, last) >= 0)
	{
		vector<TokenIndex> vctTokenIndex;
		int j = pTokenArray->SearchEnzansi("=,+=,-=,||,&&,==,!=,<,<=,>,>=,+,-,*,/,%", ',', first, last);
		while (j >= 0)
		{
			TokenIndex tokenIndex;
			tokenIndex.m_index = j;
			tokenIndex.m_pTokenArray = pTokenArray;
			vctTokenIndex.push_back(tokenIndex);
			j = pTokenArray->SearchEnzansi("=,+=,-=,||,&&,==,!=,<,<=,>,>=,+,-,*,/,%", ',', j + 1, last);
		}
		stable_sort(vctTokenIndex.begin(),vctTokenIndex.end());
		int index = (vctTokenIndex.end() - 1)->m_index;
		int H1F = first;
		int H1L = index;
		int H2F = index + 1;
		int H2L = last;
		CVar* pVariable = NULL;
		CFunc* pFunc = NULL;
		if (*(*pTokenArray)[index] == "=")
		{
			pVariable = pBlock->NewVariable((*pTokenArray)[H1F]->GetStr());
			pFunc = NewFunc(pBlock, pTokenArray, H2F, H2L);
			if (pVariable && pFunc)
			{
				return new CEqual(pVariable,pFunc);
			}
		}
		else if (*(*pTokenArray)[index] == "+=")
		{
			pVariable = pBlock->NewVariable((*pTokenArray)[H1F]->GetStr());
			pFunc = NewFunc(pBlock, pTokenArray, H2F, H2L);
			if (pVariable && pFunc)
			{
				return new CPlusEqual(pVariable, pFunc);
			}
		}
		else if (*(*pTokenArray)[index] == "-=")
		{
			pVariable = pBlock->NewVariable((*pTokenArray)[H1F]->GetStr());
			pFunc = NewFunc(pBlock, pTokenArray, H2F, H2L);
			if (pVariable && pFunc)
			{
				return new CMinusEqual(pVariable, pFunc);
			}
		}
		SAFE_DELETE(pVariable);
		SAFE_DELETE(pFunc);
		CFunc* pFunc1 = NULL;
		CFunc* pFunc2 = NULL;
		if(*(*pTokenArray)[index] == "+")
		{
			pFunc1 = NewFunc(pBlock, pTokenArray, H1F, H1L);
			pFunc2 = NewFunc(pBlock, pTokenArray, H2F, H2L);
			if (pFunc1 && pFunc2)
			{
				return new CPlus(pFunc1, pFunc2);
			}
		}
		else if (*(*pTokenArray)[index] == "-")
		{
			pFunc1 = NewFunc(pBlock, pTokenArray, H1F, H1L);
			pFunc2 = NewFunc(pBlock, pTokenArray, H2F, H2L);
			if (pFunc1 && pFunc2)
			{
				return new CMinus(pFunc1, pFunc2);
			}
		}
		else if (*(*pTokenArray)[index] == "*")
		{
			pFunc1 = NewFunc(pBlock, pTokenArray, H1F, H1L);
			pFunc2 = NewFunc(pBlock, pTokenArray, H2F, H2L);
			if (pFunc1 && pFunc2)
			{
				return new CMultiply(pFunc1, pFunc2);
			}
		}
		else if(*(*pTokenArray)[index] == "/")
		{
			pFunc1 = NewFunc(pBlock, pTokenArray, H1F, H1L);
			pFunc2 = NewFunc(pBlock, pTokenArray, H2F, H2L);
			if (pFunc1 && pFunc2)
			{
				return new CDivide(pFunc1, pFunc2);
			}
		}
		else if(*(*pTokenArray)[index] == "%")
		{
			pFunc1 = NewFunc(pBlock, pTokenArray, H1F, H1L);
			pFunc2 = NewFunc(pBlock, pTokenArray, H2F, H2L);
			if (pFunc1 && pFunc2)
			{
				return new CSurplus(pFunc1, pFunc2);
			}
		}
		else if(*(*pTokenArray)[index] == "==")
		{
			pFunc1 = NewFunc(pBlock, pTokenArray, H1F, H1L);
			pFunc2 = NewFunc(pBlock, pTokenArray, H2F, H2L);
			if (pFunc1 && pFunc2)
			{
				return new CEqualEqual(pFunc1, pFunc2);
			}
		}
		else if(*(*pTokenArray)[index] == "!=")
		{
			pFunc1 = NewFunc(pBlock, pTokenArray, H1F, H1L);
			pFunc2 = NewFunc(pBlock, pTokenArray, H2F, H2L);
			if (pFunc1 && pFunc2)
			{
				return new CNotEqual(pFunc1, pFunc2);
			}
		}
		else if(*(*pTokenArray)[index] == ">")
		{
			pFunc1 = NewFunc(pBlock, pTokenArray, H1F, H1L);
			pFunc2 = NewFunc(pBlock, pTokenArray, H2F, H2L);
			if (pFunc1 && pFunc2)
			{
				return new CIsLarger(pFunc1, pFunc2);
			}
		}
		else if(*(*pTokenArray)[index] == "<")
		{
			pFunc1 = NewFunc(pBlock, pTokenArray, H1F, H1L);
			pFunc2 = NewFunc(pBlock, pTokenArray, H2F, H2L);
			if (pFunc1 && pFunc2)
			{
				return new CIsSmaller(pFunc1, pFunc2);
			}
		}
		else if(*(*pTokenArray)[index]==">=")
		{
			pFunc1 = NewFunc(pBlock,pTokenArray,H1F,H1L);
			pFunc2 = NewFunc(pBlock,pTokenArray,H2F,H2L);
			if(pFunc1 && pFunc2)
				return new CIsLargerEqual(pFunc1, pFunc2);
		}
		else if(*(*pTokenArray)[index] == "<=")
		{
			pFunc1 = NewFunc(pBlock, pTokenArray, H1F, H1L);
			pFunc2 = NewFunc(pBlock, pTokenArray, H2F, H2L);
			if (pFunc1 && pFunc2)
			{
				return new CIsSmallerEqual(pFunc1, pFunc2);
			}
		}
		else if(*(*pTokenArray)[index] == "&&")
		{
			pFunc1 = NewFunc(pBlock, pTokenArray, H1F, H1L);
			pFunc2 = NewFunc(pBlock, pTokenArray, H2F, H2L);
			if(pFunc1 && pFunc2)
			{
				return new CAndAnd(pFunc1, pFunc2);
			}
		}
		else if(*(*pTokenArray)[index] == "||")
		{
			pFunc1 = NewFunc(pBlock, pTokenArray, H1F, H1L);
			pFunc2 = NewFunc(pBlock, pTokenArray, H2F, H2L);
			if (pFunc1 && pFunc2)
			{
				return new COrOr(pFunc1,pFunc2);
			}
		}
		SAFE_DELETE(pFunc1);
		SAFE_DELETE(pFunc2);
		MBF_ERR("%s: この演算子は未実装です。",
			(*pTokenArray)[index]->GetStr());
		return NULL;
	}
	if (IsVar((*pTokenArray)[i]->GetStr()))	//識別子であることを確認しておく
	{
		CVar* pVar = pBlock->NewVariable((*pTokenArray)[i]->GetStr());
		if (pVar != NULL)
		{
			return pVar;
		}
		if(i + 1 == last)
		{
			return NULL;
		}
		int corKakko=pTokenArray->GetCorrespondence(i + 1, "(", ")");
		if(*(*pTokenArray)[i+1] == "(" && corKakko != -1)
		{
			if (corKakko!=last-1)
			{
				MBF_ERR("%s:　；がありません。",(*pTokenArray)[i]->GetStr());
				return NULL;
			}
			{
				//引数なし関数
				int H1L = pTokenArray->GetCorrespondence(first + 1, "(", ")"); //（に対応する）
				if (H1L < 0) 
				{
					return NULL;
				}
				if(*(*pTokenArray)[i] == "rnd")
				{
					return new CRnd;
				}
				if(*(*pTokenArray)[i] == "WaitTime")
				{
					return new CWaitTime;
				}
				if(*(*pTokenArray)[i] == "UpdateInput")
				{
					return new CUpdateInput;
				}
				if(*(*pTokenArray)[i] == "DrawList")
				{
					return new CDrawList;
				}
				if(*(*pTokenArray)[i] == "ShowFPS")
				{
					return new CShowFPS;
				}
				if(*(*pTokenArray)[i] == "Flip")
				{
					return new CFlip;
				}
				if(*(*pTokenArray)[i] == "ShotToItem")
				{
					return new CShotToItem;
				}
			}
			{
				CFunc* ret = NULL;
				int H1F = first + 2;
				if (H1F >= last) 
				{
					return NULL;
				}
				int H1L = pTokenArray->GetCorrespondence(first+1, "(", ")");//（に対応する）
				if (H1L < 0) 
				{
					return NULL;
				}
				if (*(*pTokenArray)[i] == "sin")
				{
					CFunc* pFunc = NewFunc(pBlock,pTokenArray,H1F,H1L);
					if (pFunc != NULL)
					{
						return new CSin(pFunc);
					}
				}
				if (*(*pTokenArray)[i] == "cos")
				{
					CFunc* pFunc = NewFunc(pBlock, pTokenArray, H1F, H1L);
					if (pFunc != NULL)
					{
						return new CCos(pFunc);
					}
				}
				if (*(*pTokenArray)[i] == "tan")
				{
					CFunc* pFunc = NewFunc(pBlock, pTokenArray, H1F, H1L);
					if (pFunc != NULL)
					{
						return new CTan(pFunc);
					}
				}
				if (*(*pTokenArray)[i]=="atan")
				{
					CFunc* pFunc = NewFunc(pBlock,pTokenArray,H1F,H1L);
					if(pFunc!=NULL)
						return new CAtan(pFunc);
				}
				if (*(*pTokenArray)[i] == "sqrt")
				{
					CFunc* pFunc = NewFunc(pBlock, pTokenArray, H1F, H1L);
					if (pFunc != NULL)
					{
						return new CSqrt(pFunc);
					}
				}
				if (*(*pTokenArray)[i] == "abs")
				{
					CFunc* pFunc = NewFunc(pBlock, pTokenArray, H1F, H1L);
					if (pFunc != NULL)
					{
						return new CAbs(pFunc);
					}
				}
				if (*(*pTokenArray)[i] == "GetInputState")
				{
					CFunc* pFunc = NewFunc(pBlock, pTokenArray, H1F, H1L);
					if(pFunc != NULL)
					{
						return new CGetInputState(pFunc);
					}
				}
				if (*(*pTokenArray)[i] == "GetInputEvent")
				{
					CFunc* pFunc = NewFunc(pBlock, pTokenArray, H1F, H1L);
					if(pFunc!=NULL)
					{
						return new CGetInputEvent(pFunc);
					}
				}
			}
			{
				int H1F = i + 2;
				int H1L = pTokenArray->SearchCommma(i + 2, corKakko);
				if (H1L != -1)
				{
					int H2F = H1L+1;
					int H2L = corKakko;
					CFunc* pFunc1 = NULL;
					CFunc* pFunc2 = NULL;
					if (*(*pTokenArray)[i] == "Plus")
					{
						pFunc1 = NewFunc(pBlock,pTokenArray,H1F,H1L);
						pFunc2 = NewFunc(pBlock,pTokenArray,H2F,H2L);
						if (pFunc1 && pFunc2)
						{
							return new CPlus(pFunc1,pFunc2);
						}
					}
					else if (*(*pTokenArray)[i] == "Minus")
					{
						pFunc1 = NewFunc(pBlock,pTokenArray,H1F,H1L);
						pFunc2 = NewFunc(pBlock,pTokenArray,H2F,H2L);
						if (pFunc1 && pFunc2)
						{
							return new CMinus(pFunc1,pFunc2);
						}
					}
					else if (*(*pTokenArray)[i]=="atan2")
					{
						pFunc1 = NewFunc(pBlock, pTokenArray, H1F, H1L);
						pFunc2 = NewFunc(pBlock, pTokenArray, H2F, H2L);
						if (pFunc1 && pFunc2)
						{
							return new CAtan2(pFunc1, pFunc2);
						}
					}
					else if (*(*pTokenArray)[i]=="random")
					{
						pFunc1 = NewFunc(pBlock,pTokenArray,H1F,H1L);
						pFunc2 = NewFunc(pBlock,pTokenArray,H2F,H2L);
						if (pFunc1 && pFunc2)
						{
							return new CRandom(pFunc1, pFunc2);
						}
					}
					else if(*(*pTokenArray)[i] == "mod")
					{
						pFunc1 = NewFunc(pBlock, pTokenArray, H1F, H1L);
						pFunc2 = NewFunc(pBlock, pTokenArray, H2F, H2L);
						if (pFunc1 && pFunc2)
						{
							return new CMod(pFunc1, pFunc2);
						}
					}
					else if(*(*pTokenArray)[i] == "PlaySound")
					{
						pFunc1 = NewFunc(pBlock,pTokenArray,H1F,H1L);
						pFunc2 = NewFunc(pBlock,pTokenArray,H2F,H2L);
						if (pFunc1 && pFunc2)
						{
							return new CPlaySound(pFunc1, pFunc2);
						}
					}
					else if(*(*pTokenArray)[i]=="LoadImage")
					{
						pFunc1 = NewFunc(pBlock,pTokenArray,H1F,H1L);
						char* str = newstr((*pTokenArray)[H2L-1]->GetStr() + 1, strchr((*pTokenArray)[H2L-1]->GetStr() + 1, '\"'));
						if (pFunc1)
						{
							return new CLoadImage(pFunc1,str);
						}
					}
					else if (*(*pTokenArray)[i]=="LoadSound")
					{
						pFunc1 = NewFunc(pBlock,pTokenArray,H1F,H1L);
						char* str = newstr((*pTokenArray)[H2L-1]->GetStr() + 1, strchr((*pTokenArray)[H2L-1]->GetStr() + 1, '\"'));
						if (pFunc1)
						{
							return new CLoadSound(pFunc1, str);
						}
					}
					else if (*(*pTokenArray)[i]=="Tikei")
					{
						pFunc1 = NewFunc(pBlock,pTokenArray,H1F,H1L);
						pFunc2 = NewFunc(pBlock,pTokenArray,H2F,H2L);
						if (pFunc1 && pFunc2)
						{
							return new CIsTikei(pFunc1, pFunc2);
						}
					}
					SAFE_DELETE(pFunc1);
					SAFE_DELETE(pFunc2);
					if (*(*pTokenArray)[i] == "DrawImage")
					{
						int H2F = H1L+1;
						int H2L = pTokenArray->SearchCommma(H2F, corKakko);
						int H3F = H2L+1;
						int H3L = pTokenArray->SearchCommma(H3F, corKakko);
						int H4F = H3L+1;
						int H4L = corKakko;
						CFunc* pFunc1=NewFunc(pBlock,pTokenArray,H1F,H1L);
						CFunc* pFunc2=NewFunc(pBlock,pTokenArray,H2F,H2L);
						CFunc* pFunc3=NewFunc(pBlock,pTokenArray,H3F,H3L);
						CFunc* pFunc4=NewFunc(pBlock,pTokenArray,H4F,H4L);
						if (pFunc1&&pFunc2 && pFunc3 && pFunc4)
						{
							return new CDraw(pFunc1, pFunc2, pFunc3, pFunc4);
						}
						SAFE_DELETE(pFunc1);
						SAFE_DELETE(pFunc2);
						SAFE_DELETE(pFunc3);
						SAFE_DELETE(pFunc4);
					}
					/*
					else if(*(*pTokenArray)[i] == "DrawImageAdd")
					{
						int H2F = H1L + 1;
						int H2L = pTokenArray->SearchCommma(H2F, corKakko);
						int H3F = H2L + 1;
						int H3L = pTokenArray->SearchCommma(H3F, corKakko);
						int H4F = H3L + 1;
						int H4L = corKakko;
						CFunc* pFunc1 = NewFunc(pBlock,pTokenArray, H1F, H1L);
						CFunc* pFunc2 = NewFunc(pBlock,pTokenArray, H2F, H2L);
						CFunc* pFunc3 = NewFunc(pBlock,pTokenArray, H3F, H3L);
						CFunc* pFunc4 = NewFunc(pBlock,pTokenArray, H4F, H4L);
						if(pFunc1 && pFunc2 && pFunc3 && pFunc4)
						{
							return new CDrawAdd(pFunc1, pFunc2, pFunc3, pFunc4);
						}
						SAFE_DELETE(pFunc1);
						SAFE_DELETE(pFunc2);
						SAFE_DELETE(pFunc3);
						SAFE_DELETE(pFunc4);
					}
					*/
					else if(*(*pTokenArray)[i] == "LoadImage2")
					{
						int H2F = H1L + 1;
						int H2L = pTokenArray->SearchCommma(H2F, corKakko);
						int H3F = H2L + 1;
						int H3L = pTokenArray->SearchCommma(H3F, corKakko);
						int H4F = H3L + 1;
						int H4L = corKakko;
						CFunc* pFunc1 = NewFunc(pBlock, pTokenArray, H1F, H1L);
						char* str=newstr((*pTokenArray)[H2L-1]->GetStr() + 1,
							strchr((*pTokenArray)[H2L-1]->GetStr() + 1, '\"'));
						CFunc* pFunc3 = NewFunc(pBlock, pTokenArray, H3F, H3L);
						CFunc* pFunc4 = NewFunc(pBlock, pTokenArray, H4F, H4L);
						if (pFunc1 && str && pFunc3 && pFunc4)
						{
							return new CLoadImage2(pFunc1, str, pFunc3, pFunc4);
						}
						SAFE_DELETE(pFunc1);
						SAFE_DELETE(pFunc3);
						SAFE_DELETE(pFunc4);
					}
				}
			}
			{
				//引数をvectorで渡す多引数関数
				int start = i + 2;
				int final;
				vector<CFunc*> funcarray;//ＮＥＷした関数リスト
				while ((final = pTokenArray->SearchCommma(start,corKakko)) > 0)
				{
					CFunc* pFunc = NewFunc(pBlock, pTokenArray, start, final);
					if (pFunc == NULL)
					{
						for (int i = 0; i < funcarray.size(); ++i)
						{
							SAFE_DELETE(funcarray[i]);
						}
						return NULL;
					}
					funcarray.push_back(pFunc);
					start = final + 1;
				}
				funcarray.push_back(NewFunc(pBlock, pTokenArray, start, corKakko));
				//funcarray初期化完了
				if (*(*pTokenArray)[i] == "Shot")
				{
					return new CShot(funcarray);
				}
				else if (*(*pTokenArray)[i] == "DrawImage2")
				{
					return new CDraw2(funcarray);
				}
				else if (*(*pTokenArray)[i] == "Explode")
				{
					return new CExplode(funcarray);
				}
			}
			MBF_ERR("%s:　存在しない関数です。",
				(*pTokenArray)[i]->GetStr());
			return NULL;
		}
		else
		{
			MBF_ERR("%s:関数に正しいカッコがありません。",
				(*pTokenArray)[i]->GetStr());
			return NULL;
		}
	}
	else
	{
		MBF_ERR("%s%s:文が間違ってます。",(*pTokenArray)[i]->GetStr(),(*pTokenArray)[last]->GetStr());
		return NULL;
	}
}
