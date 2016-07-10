package com.iteye.weimingtom.myosotis.nkscript;

public class Atan implements Func {
	private Func pValue;

	public Atan(Func pValue) {
		this.pValue = pValue;
	}
	
	@Override
	public double Get() {
		return Math.atan(this.pValue.Get());
	}
}
