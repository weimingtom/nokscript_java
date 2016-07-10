package com.iteye.weimingtom.myosotis.nkscript;

public class LoadImage2 implements Func {
	private Func pFunc;
	private String str;
	private Func pWidth;
	private Func pHeight;
	
	public LoadImage2(Func pFunc, String str, Func pWidth, Func pHeight) {
		this.pFunc = pFunc;
		this.str = str;
		this.pWidth = pWidth;
		this.pHeight = pHeight;
	}
	
	@Override
	public double Get() {
		// TODO Auto-generated method stub
		return 0;
	}

}
