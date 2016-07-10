package com.iteye.weimingtom.myosotis.nkscript;

public class Sin implements Func {
	private Func pValue;

	public Sin(Func pValue) {
		this.pValue = pValue;
	}
	
	@Override
	public double Get() {
		return Math.sin(this.pValue.Get());
	}
}
