package com.iteye.weimingtom.myosotis.nkscript;

/**
 * struct CLASSDEFINE
 * @author Administrator
 *
 */
public class ClassStruct {
	public String name;
	public ClassDefine define = new ClassDefine();
	
	public ClassStruct(TokenArray pTA, int first) {
		this.name = pTA.GetArray().get(first).GetStr();
		this.define.Read(pTA, first + 1, this.name);
	}
}
