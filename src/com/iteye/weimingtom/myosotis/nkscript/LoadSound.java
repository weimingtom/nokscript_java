package com.iteye.weimingtom.myosotis.nkscript;

public class LoadSound implements Func {
	private Func pFunc;
	private String str;
	
	public LoadSound(Func pFunc, String str) {
		this.pFunc = pFunc;
		this.str = str;
	}
	
	@Override
	public double Get() {
		// TODO Auto-generated method stub
		return 0;
	}

}
