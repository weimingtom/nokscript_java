package com.iteye.weimingtom.myosotis.nkscript;

public class Atan2 implements Func {
	private Func pValue1;
	private Func pValue2;
	
	public Atan2(Func pValue1, Func pValue2) {
		this.pValue1 = pValue1;
		this.pValue2 = pValue2;
	}
	
	@Override
	public double Get() {
		return Math.atan2(this.pValue1.Get(), this.pValue2.Get());
	}
}
