package com.iteye.weimingtom.myosotis.nkscript;

public class Cos implements Func {
	private Func pValue;

	public Cos(Func pValue) {
		this.pValue = pValue;
	}
	
	@Override
	public double Get() {
		return Math.cos(this.pValue.Get());
	}
}
