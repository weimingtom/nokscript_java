package com.iteye.weimingtom.myosotis.nkscript;

public class OrOr implements Func {
	private Func pValue1;
	private Func pValue2;
	
	public OrOr(Func pValue1, Func pValue2) {
		this.pValue1 = pValue1;
		this.pValue2 = pValue2;
	}
	
	@Override
	public double Get() {
		return (this.pValue1.Get() != 0 || this.pValue2.Get() != 0) ? 1 : 0;
	}
}
