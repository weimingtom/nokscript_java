package com.iteye.weimingtom.myosotis.nkscript.test;

import com.iteye.weimingtom.myosotis.nkscript.NkScript;

public class Test001 {
	public static void main(String[] args) throws Exception {
		NkScript m_pScript = new NkScript("stage1.txt");
		int initfunc = m_pScript.SearchFunc("Init");
		if (initfunc < 0) {
			return;
		}
		m_pScript.GetFunc().get(initfunc).func.Get();
	}
}
