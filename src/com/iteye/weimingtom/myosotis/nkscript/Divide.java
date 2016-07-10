package com.iteye.weimingtom.myosotis.nkscript;

public class Divide implements Func {
	private Func pValue1;
	private Func pValue2;
	
	public Divide(Func pValue1, Func pValue2) {
		this.pValue1 = pValue1;
		this.pValue2 = pValue2;
	}
	
	@Override
	public double Get() {
		return this.pValue1.Get() / this.pValue2.Get();
	}
}
