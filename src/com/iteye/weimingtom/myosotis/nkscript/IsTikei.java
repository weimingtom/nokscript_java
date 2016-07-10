package com.iteye.weimingtom.myosotis.nkscript;

public class IsTikei implements Func {
	private Func pValue1;
	private Func pValue2;
	
	public IsTikei(Func pValue1, Func pValue2) {
		this.pValue1 = pValue1;
		this.pValue2 = pValue2;
	}
	
	@Override
	public double Get() {
		// TODO Auto-generated method stub
		return 0;
	}

}
