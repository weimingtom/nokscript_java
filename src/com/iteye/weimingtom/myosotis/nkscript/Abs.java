package com.iteye.weimingtom.myosotis.nkscript;

public class Abs implements Func {
	private Func pValue;

	public Abs(Func pValue) {
		this.pValue = pValue;
	}
	
	@Override
	public double Get() {
		return Math.abs(this.pValue.Get());
	}
}
