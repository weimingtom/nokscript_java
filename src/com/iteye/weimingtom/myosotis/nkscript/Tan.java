package com.iteye.weimingtom.myosotis.nkscript;

public class Tan implements Func {
	private Func pValue;

	public Tan(Func pValue) {
		this.pValue = pValue;
	}
	
	@Override
	public double Get() {
		return Math.tan(this.pValue.Get());
	}
}
