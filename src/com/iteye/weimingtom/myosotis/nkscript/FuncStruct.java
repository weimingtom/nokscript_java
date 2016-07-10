package com.iteye.weimingtom.myosotis.nkscript;

/**
 * struct FUNCDEFINE
 * @author Administrator
 *
 */
public class FuncStruct {
	public LogicalBlock func;
	public String name;
	
	public FuncStruct(TokenArray pTA, int first){
		this.name = pTA.GetArray().get(first).GetStr();
		if ("(".equals(pTA.GetArray().get(first + 1).GetStr())) {
			MBF_ERR(pTA.GetArray().get(first).GetStr() + ":function does not exist\n");
		}
		int i = pTA.Search(first, ")");
		if (i >= 0) {
			this.func.Read(null, pTA, i + 1);
		}
	}
	
	private static void MBF_ERR(String str) {
		System.out.println(str);
	}
}
