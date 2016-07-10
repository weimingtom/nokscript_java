package com.iteye.weimingtom.myosotis.nkscript;

public class Draw implements Func {
	private Func pIndex;
	private Func pLayer;
	private Func pFuncX;
	private Func pFuncY;
	
	public Draw(Func pIndex, Func pLayer, Func pFuncX, Func pFuncY) {
		this.pIndex = pIndex;
		this.pLayer = pLayer;
		this.pFuncX = pFuncX;
		this.pFuncY = pFuncY;
	}
	
	@Override
	public double Get() {
		// TODO Auto-generated method stub
		return 0;
	}

}
