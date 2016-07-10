package com.iteye.weimingtom.myosotis.nkscript;

public class ClassDefine {
	private static final int VARNUM = 256;
	
	private MemFunc pMemFunc;
	private MemFunc pConstructer; 
	private int varnum; 
	private String[] varname = new String[VARNUM];
	
	public ClassDefine() {
		this.pMemFunc = null;
		this.varnum = 0;
	}
	
	public int GetVarNum() {
		return this.varnum;
	}
	
	public String[] GetVarNameArray() {
		return this.varname;
	}
	
	public MemFunc GetMemFunc() {
		return this.pMemFunc;
	}
	
	public MemFunc GetConstructer() {
		return this.pConstructer;
	}
	
	public boolean Read(TokenArray pTokenArray, int first, String str) {
		this.varnum = 0;
		for (int i = 0;i < VARNUM; ++i) {
			this.varname[i] = null;
		}
		int last = pTokenArray.GetCorrespondence(first, "{", "}");
		if (last == -1) {
			MB_ERR("{ is not match }");
			return false;
		}
		for (int i = first + 1; i != last; ++i) {
			if ("double".equals(pTokenArray.GetArray().get(i).GetStr())  || 
				"var".equals(pTokenArray.GetArray().get(i).GetStr())) {
				++i;
				for (; i != last && pTokenArray.GetArray().get(i) != null; ++i) {
					if (Misc.IsVar(pTokenArray.GetArray().get(i).GetStr())) {
						this.varname[this.varnum] = pTokenArray.GetArray().get(i).GetStr();
						this.varnum++;
						i++;
						if (";".equals((pTokenArray.GetArray().get(i)).GetStr())) {
							break;
						} else if(",".equals(pTokenArray.GetArray().get(i).GetStr())) {
							continue;
						} else {
							MBF_ERR(pTokenArray.GetArray().get(i).GetStr() + ":variable symbol is not correct");
							return false;
						}
					} else {
						MBF_ERR(pTokenArray.GetArray().get(i).GetStr() + ":variable name is not correct");
						return false;
					}
				}
			} else if (Misc.IsVar(pTokenArray.GetArray().get(i).GetStr()) && 
					"(".equals(pTokenArray.GetArray().get(i + 1).GetStr())) {
				int kakko = pTokenArray.GetCorrespondence(i + 1, "(", ")");
				if (kakko < 0) {
					MBF_ERR(pTokenArray.GetArray().get(i).GetStr() + 
						":成员函数没有)");
					return false;
				}
				int tyukakko = pTokenArray.GetCorrespondence(kakko + 1, "{", "}");
				if (tyukakko < 0) {
					MBF_ERR(pTokenArray.GetArray().get(i).GetStr() + ":member function has no {}");
					return false;
				}
				i = tyukakko;
			} else {
				MBF_ERR(pTokenArray.GetArray().get(i).GetStr() + ":code is not recognized");
				return false;
			}
		}
		int funcname = pTokenArray.Search(first, str);
		if (funcname < 0 || funcname >= last)
		{
			this.pConstructer = null;
		} else {
			this.pConstructer = new MemFunc();
			if (!this.pConstructer.Read(this, pTokenArray, funcname + 3))
			{
				this.pConstructer = null;
			}
		}
		funcname = pTokenArray.Search(first, "main");
		if (funcname < 0 || funcname >= last) {
			MBF_ERR("have no main function");
			return false;
		}
		this.pMemFunc = new MemFunc();
		return this.pMemFunc.Read(this, pTokenArray, funcname + 3);
	}
	
	private static void MB_ERR(String str) {
		System.out.println(str);
	}
	
	private static void MBF_ERR(String str) {
		System.out.println(str);
	}
}
