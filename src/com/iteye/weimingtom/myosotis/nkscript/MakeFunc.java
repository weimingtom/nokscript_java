package com.iteye.weimingtom.myosotis.nkscript;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

//FIXME:
public class MakeFunc {
	public static Func NewFunc2(Block pBlock, TokenArray pTokenArray, 
		int first, int[] last) {
		int i = first;
		if (";".equals(pTokenArray.GetArray().get(i).GetStr())) {
			last[0]++;
			return NewFunc2(pBlock, pTokenArray, i + 1, last);
		} else if ("{".equals(pTokenArray.GetArray().get(i).GetStr())) {
			last[0] = pTokenArray.GetCorrespondence(i, "{", "}");
			if (last[0] < 0) {
				return null;
			}
			LogicalBlock pBlock2 = new LogicalBlock();
			if (pBlock2.Read(pBlock, pTokenArray, i)) {
				return pBlock2;
			} else {
				pBlock = null;
				return null;
			}
		} else if ("if".equals(pTokenArray.GetArray().get(i).GetStr())) {
			if ("(".equals(pTokenArray.GetArray().get(i + 1).GetStr())) {
				MB_ERR("if没有(。");
				return null;
			}
			int last2 = pTokenArray.GetCorrespondence(i + 1, "(", ")");
			if (last2 < 0) {
				MB_ERR("if没有)。");
				return null;
			}
			Func pJoken = NewFunc(pBlock, pTokenArray, i + 2, last2);	
			Func pFunc = NewFunc2(pBlock, pTokenArray, last2 + 1, last);
			Func pElse = null;
			if ("else".equals(pTokenArray.GetArray().get(last[0] + 1).GetStr())) {
				pElse = NewFunc2(pBlock, pTokenArray, last[0] + 2, last);
			}
			if (pJoken != null && pFunc != null) {
				return new If(pJoken, pFunc, pElse);
			}
			pJoken = null;
			pBlock = null;
			pElse = null;
			return null;
		} else if ("while".equals(pTokenArray.GetArray().get(i).GetStr())) {
			if ("(".equals(pTokenArray.GetArray().get(i + 1).GetStr())) {
				MB_ERR("while没有(。");
				return null;
			}
			int last2 = pTokenArray.GetCorrespondence(i + 1, "(", ")");
			if (last2 < 0) {
				MB_ERR("while没有)。");
				return null;
			}
			//条件式
			Func pJoken = NewFunc(pBlock, pTokenArray, i + 2, last2);	
			Func pFunc = NewFunc2(pBlock, pTokenArray, last2 + 1, last);
			if (pJoken != null && pFunc != null) {
				return new While(pJoken, pFunc);
			}
			pJoken = null;
			pFunc = null;
			return null;
		} else if ("for".equals(pTokenArray.GetArray().get(i).GetStr())) {
			if ("(".equals(pTokenArray.GetArray().get(i + 1).GetStr())) {
				MB_ERR("for没有(。");
				return null;
			}
			int last2 = pTokenArray.GetCorrespondence(i + 1, "(", ")");
			if (last2 < 0) {
				MB_ERR("for没有)。");
				return null;
			}
			int colon1 = pTokenArray.Search(i + 1, ";");
			if (colon1 < 0) {
				MB_ERR("for没有;。");
				return null;
			}
			int colon2 = pTokenArray.Search(colon1 + 1, ";");
			if (colon2 < 0) {
				MB_ERR("for没有;。");
				return null;
			}
			Func pFirst = NewFunc(pBlock, pTokenArray, i + 2, colon1);
			Func pJoken = NewFunc(pBlock, pTokenArray, colon1 + 1, colon2);
			Func pStep = NewFunc(pBlock, pTokenArray, colon2 + 1, last2);
			Func pFunc = NewFunc2(pBlock, pTokenArray, last2 + 1, last);
			if (pFirst != null && pJoken != null && 
				pStep != null && pFunc != null) {
				return new For(pFirst, pJoken, pStep, pFunc);
			}
			pFirst = null;
			pJoken = null;
			pStep = null;
			pFunc = null;
			return null;
		} else if ("break".equals(pTokenArray.GetArray().get(i).GetStr())) {
			int gyou = pTokenArray.Search(i, ";");
			if (gyou != i + 1) {
				MB_ERR("break没有;。");
				return null;
			}
			last[0] = gyou;
			return new Break();
		} else {
			int gyou = pTokenArray.Search(i, ";");
			last[0] = gyou;
			if (gyou == -1) {
				MB_ERR("没有;。");
				return null;
			}
			Func pFunc = NewFunc(pBlock, pTokenArray, i, gyou);
			if (pFunc != null) {
				return pFunc;
			}
		}
		return null;
	}

	public static Func NewFunc(Block pBlock, 
		TokenArray pTokenArray, int first, int last) {
		Func ret = null;
		int i = first;
		if (Misc.IsConst(pTokenArray.GetArray().get(i).GetStr()) && 
			last == i + 1) {
			return new Const(atof(pTokenArray.GetArray().get(i).GetStr()));
		}
		if ("(".equals(pTokenArray.GetArray().get(i).GetStr()) && 
			pTokenArray.GetCorrespondence(i, "(", ")") == last - 1) {
			return NewFunc(pBlock, pTokenArray, first + 1, last - 1);
		}
		if (pTokenArray.SearchEnzansi("=,+=,-=,||,&&,==,!=,<,<=,>,>=,+,-,*,/,%", ',', first, last) >= 0) {
			List<TokenIndex> vctTokenIndex = new ArrayList<TokenIndex>();
			int j = pTokenArray.SearchEnzansi("=,+=,-=,||,&&,==,!=,<,<=,>,>=,+,-,*,/,%", ',', first, last);
			while (j >= 0) {
				TokenIndex tokenIndex = new TokenIndex();
				tokenIndex.index = j;
				tokenIndex.pTokenArray = pTokenArray;
				vctTokenIndex.add(tokenIndex);
				j = pTokenArray.SearchEnzansi("=,+=,-=,||,&&,==,!=,<,<=,>,>=,+,-,*,/,%", ',', j + 1, last);
			}
			//stable_sort(vctTokenIndex.begin(), vctTokenIndex.end());
			//FIXME:
			Collections.sort(vctTokenIndex, new Comparator<TokenIndex> () {
				@Override
				public int compare(TokenIndex o1, TokenIndex o2) {
					return o1.index - o2.index;
				}
			});
			int index = (vctTokenIndex.get(vctTokenIndex.size() - 1)).index;
			int H1F = first;
			int H1L = index;
			int H2F = index + 1;
			int H2L = last;
			Var pVariable = null;
			Func pFunc = null;
			if ("=".equals(pTokenArray.GetArray().get(index).GetStr())) {
				pVariable = pBlock.NewVariable(pTokenArray.GetArray().get(H1F).GetStr());
				pFunc = NewFunc(pBlock, pTokenArray, H2F, H2L);
				if (pVariable != null && pFunc != null) {
					return new Equal(pVariable, pFunc);
				}
			} else if ("+=".equals(pTokenArray.GetArray().get(index).GetStr())) {
				pVariable = pBlock.NewVariable(pTokenArray.GetArray().get(H1F).GetStr());
				pFunc = NewFunc(pBlock, pTokenArray, H2F, H2L);
				if (pVariable != null && pFunc != null) {
					return new PlusEqual(pVariable, pFunc);
				}
			} else if ("-=".equals(pTokenArray.GetArray().get(index).GetStr())) {
				pVariable = pBlock.NewVariable(pTokenArray.GetArray().get(H1F).GetStr());
				pFunc = NewFunc(pBlock, pTokenArray, H2F, H2L);
				if (pVariable != null && pFunc != null) {
					return new MinusEqual(pVariable, pFunc);
				}
			}
			pVariable = null;
			pFunc = null;
			Func pFunc1 = null;
			Func pFunc2 = null;
			if ("+".equals(pTokenArray.GetArray().get(index).GetStr())) {
				pFunc1 = NewFunc(pBlock, pTokenArray, H1F, H1L);
				pFunc2 = NewFunc(pBlock, pTokenArray, H2F, H2L);
				if (pFunc1 != null && pFunc2 != null) {
					return new Plus(pFunc1, pFunc2);
				}
			} else if ("-".equals(pTokenArray.GetArray().get(index).GetStr())) {
				pFunc1 = NewFunc(pBlock, pTokenArray, H1F, H1L);
				pFunc2 = NewFunc(pBlock, pTokenArray, H2F, H2L);
				if (pFunc1 != null && pFunc2 != null) {
					return new Minus(pFunc1, pFunc2);
				}
			} else if ("*".equals(pTokenArray.GetArray().get(index).GetStr())) {
				pFunc1 = NewFunc(pBlock, pTokenArray, H1F, H1L);
				pFunc2 = NewFunc(pBlock, pTokenArray, H2F, H2L);
				if (pFunc1 != null && pFunc2 != null) {
					return new Multiply(pFunc1, pFunc2);
				}
			} else if ("/".equals(pTokenArray.GetArray().get(index).GetStr())) {
				pFunc1 = NewFunc(pBlock, pTokenArray, H1F, H1L);
				pFunc2 = NewFunc(pBlock, pTokenArray, H2F, H2L);
				if (pFunc1 != null && pFunc2 != null) {
					return new Divide(pFunc1, pFunc2);
				}
			} else if ("%".equals(pTokenArray.GetArray().get(index).GetStr())) {
				pFunc1 = NewFunc(pBlock, pTokenArray, H1F, H1L);
				pFunc2 = NewFunc(pBlock, pTokenArray, H2F, H2L);
				if (pFunc1 != null && pFunc2 != null)
				{
					return new Surplus(pFunc1, pFunc2);
				}
			} else if ("==".equals(pTokenArray.GetArray().get(index).GetStr())) {
				pFunc1 = NewFunc(pBlock, pTokenArray, H1F, H1L);
				pFunc2 = NewFunc(pBlock, pTokenArray, H2F, H2L);
				if (pFunc1 != null && pFunc2 != null) {
					return new EqualEqual(pFunc1, pFunc2);
				}
			} else if ("!=".equals(pTokenArray.GetArray().get(index).GetStr())) {
				pFunc1 = NewFunc(pBlock, pTokenArray, H1F, H1L);
				pFunc2 = NewFunc(pBlock, pTokenArray, H2F, H2L);
				if (pFunc1 != null && pFunc2 != null) {
					return new NotEqual(pFunc1, pFunc2);
				}
			} else if (">".equals(pTokenArray.GetArray().get(index).GetStr())) {
				pFunc1 = NewFunc(pBlock, pTokenArray, H1F, H1L);
				pFunc2 = NewFunc(pBlock, pTokenArray, H2F, H2L);
				if (pFunc1 != null && pFunc2 != null) {
					return new IsLarger(pFunc1, pFunc2);
				}
			} else if ("<".equals(pTokenArray.GetArray().get(index).GetStr())) {
				pFunc1 = NewFunc(pBlock, pTokenArray, H1F, H1L);
				pFunc2 = NewFunc(pBlock, pTokenArray, H2F, H2L);
				if (pFunc1 != null && pFunc2 != null) {
					return new IsSmaller(pFunc1, pFunc2);
				}
			} else if (">=".equals(pTokenArray.GetArray().get(index).GetStr())) {
				pFunc1 = NewFunc(pBlock,pTokenArray,H1F,H1L);
				pFunc2 = NewFunc(pBlock,pTokenArray,H2F,H2L);
				if (pFunc1 != null && pFunc2 != null) {
					return new IsLargerEqual(pFunc1, pFunc2);
				}
			} else if ("<=".equals(pTokenArray.GetArray().get(index).GetStr())) {
				pFunc1 = NewFunc(pBlock, pTokenArray, H1F, H1L);
				pFunc2 = NewFunc(pBlock, pTokenArray, H2F, H2L);
				if (pFunc1 != null && pFunc2 != null) {
					return new IsSmallerEqual(pFunc1, pFunc2);
				}
			} else if ("&&".equals(pTokenArray.GetArray().get(index).GetStr())) {
				pFunc1 = NewFunc(pBlock, pTokenArray, H1F, H1L);
				pFunc2 = NewFunc(pBlock, pTokenArray, H2F, H2L);
				if (pFunc1 != null && pFunc2 != null) {
					return new AndAnd(pFunc1, pFunc2);
				}
			} else if("||".equals(pTokenArray.GetArray().get(index).GetStr())) {
				pFunc1 = NewFunc(pBlock, pTokenArray, H1F, H1L);
				pFunc2 = NewFunc(pBlock, pTokenArray, H2F, H2L);
				if (pFunc1 != null && pFunc2 != null) {
					return new OrOr(pFunc1, pFunc2);
				}
			}
			pFunc1 = null;
			pFunc2 = null;
			MBF_ERR(pTokenArray.GetArray().get(index).GetStr() + 
				": 这个运算符未实现。");
			return null;
		}
		//确认标识符
		if (Misc.IsVar(pTokenArray.GetArray().get(i).GetStr())) {
			Var pVar = pBlock.NewVariable(pTokenArray.GetArray().get(i).GetStr());
			if (pVar != null) {
				return pVar;
			}
			if (i + 1 == last) {
				return null;
			}
			int corKakko = pTokenArray.GetCorrespondence(i + 1, "(", ")");
			if ("(".equals(pTokenArray.GetArray().get(i + 1).GetStr()) && 
				corKakko != -1) {
				if (corKakko != last - 1) {
					MBF_ERR(pTokenArray.GetArray().get(i).GetStr() + ":没有；。");
					return null;
				}
				{
					//无参数函数
					int H1L = pTokenArray.GetCorrespondence(first + 1, "(", ")"); //（に対応する）
					if (H1L < 0) {
						return null;
					}
					if ("rnd".equals(pTokenArray.GetArray().get(i).GetStr())) {
						return new Rnd();
					}
					if ("WaitTime".equals(pTokenArray.GetArray().get(i).GetStr())) {
						return new WaitTime();
					}
					if ("UpdateInput".equals(pTokenArray.GetArray().get(i).GetStr())) {
						return new UpdateInput();
					}
					if ("DrawList".equals(pTokenArray.GetArray().get(i).GetStr())) {
						return new DrawList();
					}
					if ("ShowFPS".equals(pTokenArray.GetArray().get(i).GetStr())) {
						return new ShowFPS();
					}
					if ("Flip".equals(pTokenArray.GetArray().get(i).GetStr())) {
						return new Flip();
					}
					if ("ShotToItem".equals(pTokenArray.GetArray().get(i).GetStr())) {
						return new ShotToItem();
					}
				}
				{
					//Func ret = null;
					int H1F = first + 2;
					if (H1F >= last) {
						return null;
					}
					int H1L = pTokenArray.GetCorrespondence(first+1, "(", ")");//（に対応する）
					if (H1L < 0) {
						return null;
					}
					if ("sin".equals(pTokenArray.GetArray().get(i).GetStr())) {
						Func pFunc = NewFunc(pBlock,pTokenArray,H1F,H1L);
						if (pFunc != null) {
							return new Sin(pFunc);
						}
					}
					if ("cos".equals(pTokenArray.GetArray().get(i).GetStr())) {
						Func pFunc = NewFunc(pBlock, pTokenArray, H1F, H1L);
						if (pFunc != null) {
							return new Cos(pFunc);
						}
					}
					if ("tan".equals(pTokenArray.GetArray().get(i).GetStr())) {
						Func pFunc = NewFunc(pBlock, pTokenArray, H1F, H1L);
						if (pFunc != null)
						{
							return new Tan(pFunc);
						}
					}
					if ("atan".equals(pTokenArray.GetArray().get(i).GetStr())) {
						Func pFunc = NewFunc(pBlock,pTokenArray,H1F,H1L);
						if (pFunc != null) {
							return new Atan(pFunc);
						}
					}
					if ("sqrt".equals(pTokenArray.GetArray().get(i).GetStr())) {
						Func pFunc = NewFunc(pBlock, pTokenArray, H1F, H1L);
						if (pFunc != null) {
							return new Sqrt(pFunc);
						}
					}
					if ("abs".equals(pTokenArray.GetArray().get(i).GetStr())) {
						Func pFunc = NewFunc(pBlock, pTokenArray, H1F, H1L);
						if (pFunc != null) {
							return new Abs(pFunc);
						}
					}
					if ("GetInputState".equals(pTokenArray.GetArray().get(i).GetStr())) {
						Func pFunc = NewFunc(pBlock, pTokenArray, H1F, H1L);
						if(pFunc != null) {
							return new GetInputState(pFunc);
						}
					}
					if ("GetInputEvent".equals(pTokenArray.GetArray().get(i).GetStr())) {
						Func pFunc = NewFunc(pBlock, pTokenArray, H1F, H1L);
						if (pFunc != null) {
							return new GetInputEvent(pFunc);
						}
					}
				}
				{
					int H1F = i + 2;
					int H1L = pTokenArray.SearchCommma(i + 2, corKakko);
					if (H1L != -1) {
						int H2F = H1L+1;
						int H2L = corKakko;
						Func pFunc1 = null;
						Func pFunc2 = null;
						if ("Plus".equals(pTokenArray.GetArray().get(i).GetStr())) {
							pFunc1 = NewFunc(pBlock,pTokenArray,H1F,H1L);
							pFunc2 = NewFunc(pBlock,pTokenArray,H2F,H2L);
							if (pFunc1 != null && pFunc2 != null) {
								return new Plus(pFunc1, pFunc2);
							}
						} else if ("Minus".equals(pTokenArray.GetArray().get(i).GetStr())) {
							pFunc1 = NewFunc(pBlock,pTokenArray,H1F,H1L);
							pFunc2 = NewFunc(pBlock,pTokenArray,H2F,H2L);
							if (pFunc1 != null && pFunc2 != null) {
								return new Minus(pFunc1, pFunc2);
							}
						} else if ("atan2".equals(pTokenArray.GetArray().get(i).GetStr())) {
							pFunc1 = NewFunc(pBlock, pTokenArray, H1F, H1L);
							pFunc2 = NewFunc(pBlock, pTokenArray, H2F, H2L);
							if (pFunc1 != null && pFunc2 != null) {
								return new Atan2(pFunc1, pFunc2);
							}
						} else if ("random".equals(pTokenArray.GetArray().get(i).GetStr())) {
							pFunc1 = NewFunc(pBlock,pTokenArray,H1F,H1L);
							pFunc2 = NewFunc(pBlock,pTokenArray,H2F,H2L);
							if (pFunc1 != null && pFunc2 != null) {
								return new Random(pFunc1, pFunc2);
							}
						} else if ("mod".equals(pTokenArray.GetArray().get(i).GetStr())) {
							pFunc1 = NewFunc(pBlock, pTokenArray, H1F, H1L);
							pFunc2 = NewFunc(pBlock, pTokenArray, H2F, H2L);
							if (pFunc1 != null && pFunc2 != null) {
								return new Mod(pFunc1, pFunc2);
							}
						} else if ("PlaySound".equals(pTokenArray.GetArray().get(i).GetStr())) {
							pFunc1 = NewFunc(pBlock,pTokenArray,H1F,H1L);
							pFunc2 = NewFunc(pBlock,pTokenArray,H2F,H2L);
							if (pFunc1 != null && pFunc2 != null) {
								return new PlaySound(pFunc1, pFunc2);
							}
						} else if ("LoadImage".equals(pTokenArray.GetArray().get(i).GetStr())) {
							pFunc1 = NewFunc(pBlock,pTokenArray,H1F,H1L);
							String str_ = pTokenArray.GetArray().get(H2L - 1).GetStr();
							int idx_ = str_.indexOf("\"", 1);
							String str = str_.substring(1, idx_ >= 0 ? idx_ : str_.length() - 1);
							if (pFunc1 != null) {
								return new LoadImage(pFunc1,str);
							}
						} else if ("LoadSound".equals(pTokenArray.GetArray().get(i).GetStr())) {
							pFunc1 = NewFunc(pBlock,pTokenArray,H1F,H1L);
							String str_ = pTokenArray.GetArray().get(H2L - 1).GetStr();
							int idx_ = str_.indexOf("\"", 1);
							String str = str_.substring(1, idx_ >= 0 ? idx_ : str_.length() - 1);
							if (pFunc1 != null) {
								return new LoadSound(pFunc1, str);
							}
						} else if ("Tikei".equals(pTokenArray.GetArray().get(i).GetStr())) {
							pFunc1 = NewFunc(pBlock, pTokenArray, H1F, H1L);
							pFunc2 = NewFunc(pBlock, pTokenArray, H2F, H2L);
							if (pFunc1 != null && pFunc2 != null) {
								return new IsTikei(pFunc1, pFunc2);
							}
						}
						pFunc1 = null;
						pFunc2 = null;
						if ("DrawImage".equals(pTokenArray.GetArray().get(i).GetStr())) {
							H2F = H1L + 1; //int  
							H2L = pTokenArray.SearchCommma(H2F, corKakko); //int 
							int H3F = H2L + 1;
							int H3L = pTokenArray.SearchCommma(H3F, corKakko);
							int H4F = H3L + 1;
							int H4L = corKakko;
							pFunc1 = NewFunc(pBlock, pTokenArray, H1F, H1L); //Func  
							pFunc2 = NewFunc(pBlock, pTokenArray, H2F, H2L); //Func 
							Func pFunc3 = NewFunc(pBlock, pTokenArray, H3F, H3L);
							Func pFunc4 = NewFunc(pBlock, pTokenArray, H4F, H4L);
							if (pFunc1 != null && pFunc2 != null && 
								pFunc3 != null && pFunc4 != null) {
								return new Draw(pFunc1, pFunc2, pFunc3, pFunc4);
							}
							pFunc1 = null;
							pFunc2 = null;
							pFunc3 = null;
							pFunc4 = null;
						} else if ("LoadImage2".equals(pTokenArray.GetArray().get(i).GetStr())) {
							H2F = H1L + 1; //int 
							H2L = pTokenArray.SearchCommma(H2F, corKakko); //int 
							int H3F = H2L + 1;
							int H3L = pTokenArray.SearchCommma(H3F, corKakko);
							int H4F = H3L + 1;
							int H4L = corKakko;
							pFunc1 = NewFunc(pBlock, pTokenArray, H1F, H1L); //Func 
							String str_ = pTokenArray.GetArray().get(H2L - 1).GetStr();
							int idx_ = str_.indexOf("\"", 1);
							String str = str_.substring(1, idx_ >= 0 ? idx_ : str_.length() - 1);
							Func pFunc3 = NewFunc(pBlock, pTokenArray, H3F, H3L);
							Func pFunc4 = NewFunc(pBlock, pTokenArray, H4F, H4L);
							if (pFunc1 != null && str != null && 
								pFunc3 != null && pFunc4 != null) {
								return new LoadImage2(pFunc1, str, pFunc3, pFunc4);
							}
							pFunc1 = null;
							pFunc3 = null;
							pFunc4 = null;
						}
					}
				}
				{
					//用向量传递变量给多参数函数
					int start = i + 2;
					int final_;
					List<Func> funcarray = new ArrayList<Func>();//ＮＥＷした関数リスト
					while ((final_ = pTokenArray.SearchCommma(start,corKakko)) > 0) {
						Func pFunc = NewFunc(pBlock, pTokenArray, start, final_);
						if (pFunc == null) {
							for (int ii = 0; ii < funcarray.size(); ++ii) {
								funcarray.set(ii, null);
							}
							return null;
						}
						funcarray.add(pFunc);
						start = final_ + 1;
					}
					funcarray.add(NewFunc(pBlock, pTokenArray, start, corKakko));
					//funcarray初期化完了
					if ("Shot".equals(pTokenArray.GetArray().get(i).GetStr())) {
						return new Shot(funcarray);
					} else if ("DrawImage2".equals(pTokenArray.GetArray().get(i).GetStr())) {
						return new Draw2(funcarray);
					} else if ("Explode".equals(pTokenArray.GetArray().get(i).GetStr())) {
						return new Explode(funcarray);
					}
				}
				MBF_ERR(pTokenArray.GetArray().get(i).GetStr() + 
						":不存在的函数。");
				return null;
			} else {
				MBF_ERR(pTokenArray.GetArray().get(i).GetStr() + 
					":函数没有正确的格式。");
				return null;
			}
		} else {
			MBF_ERR(pTokenArray.GetArray().get(i).GetStr() + 
					pTokenArray.GetArray().get(last).GetStr() + 
					":解释不正确。");
			return null;
		}
	}
	
	private static void MB_ERR(String str) {
		System.out.println(str);
	}
	
	private static void MBF_ERR(String str) {
		System.out.println(str);
	}
	
	private static float atof(String str) {
		try {
			return Float.parseFloat(str);
		} catch (NumberFormatException e) {
			e.printStackTrace();
			return 0;
		}
	}
}
