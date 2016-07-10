package com.iteye.weimingtom.myosotis.nkscript;

import java.util.ArrayList;
import java.util.List;

public class LogicalBlock implements Block {
	private static final int VARNUM = 256;
	private List<Func> lstpFunc = new ArrayList<Func>();
	private double[] pVar;	
	private int varnum;
	private String[] varname = new String[VARNUM];
	private Block pBlock;

	public LogicalBlock() {
		this.pVar = null;
		this.varnum = 0;
		this.pBlock = null;
	}
	
	public Var NewVariable(String str) {
		for (int i = 0; i < GetVarNum(); ++i) {
			if (str == GetVarNameArray()[i]) {
				return new Variable(this, i);
			}
		}
		if (this.GetBlock() != null) {
			return GetBlock().NewVariable(str);
		}
		return null;
	}
	
	@Override
	public double Get() {
		if (this.varnum != 0) {
			this.pVar = new double[this.varnum];
		}
		int itr;
		for (itr = 0; itr < this.lstpFunc.size(); ++itr) {
			if (this.lstpFunc.get(itr) != null) {
				this.lstpFunc.get(itr).Get();
			}
		}
		this.pVar = null;
		return 0;
	}
	
	@Override
	public double[] GetVarArray() {
		return this.pVar;
	}
	
	@Override
	public int GetVarNum() {
		return this.varnum;
	}
	
	public String[] GetVarNameArray() {
		return this.varname;
	}
	
	public Block GetBlock() {
		return this.pBlock;
	}
	
	public boolean Read(Block pBlock, TokenArray pTokenArray, int first) {
		this.pBlock = pBlock;
		this.pVar = null;
		this.varnum = 0;
		for (int i = 0; i < VARNUM; ++i) {
			this.varname[i] = null;
		}
		int last = pTokenArray.GetCorrespondence(first, "{", "}");
		if (last == -1) {
			MB_ERR("{ has no correspond }");
			return false;
		}
		for (int i = first + 1; i != last; ++i) {
			if ("double".equals(pTokenArray.GetArray().get(i).GetStr())  || 
				"var".equals(pTokenArray.GetArray().get(i).GetStr())) {
				++i;
				for(; i != last && pTokenArray.GetArray().get(i) != null; ++i) {
					if (Misc.IsVar(pTokenArray.GetArray().get(i).GetStr())) {
						this.varname[this.varnum] = pTokenArray.GetArray().get(i).GetStr();
						this.varnum++;
						i++;
						if (";".equals(pTokenArray.GetArray().get(i).GetStr())) {
							break;
						} else if(",".equals(pTokenArray.GetArray().get(i).GetStr())) {
							continue;
						} else {
							MBF_ERR(pTokenArray.GetArray().get(i).GetStr() +  " : variable's name is not correct");
							return false;
						}
					} else {
						MBF_ERR(pTokenArray.GetArray().get(i).GetStr() + " : variable's name is not correct");
						return false;
					}
				}
			} else {
				//int last2 = i;
				int[] last2 = new int[1];
				last2[0] = i;
				Func pFunc = MakeFunc.NewFunc2(this, pTokenArray, i, last2);
				if (pFunc != null) {
					this.lstpFunc.add(pFunc);
				}
				i = last2[0];
			}
		}
		for (int i = 0; i < VARNUM; ++i) {
			if (this.varname[i] != null) {
				//FIXME:
				this.varname[i] = null;
			}
		}
		return true;
	}
	
	private static void MB_ERR(String str) {
		System.out.println(str);
	}
	
	private static void MBF_ERR(String str) {
		System.out.println(str);
	}
}
