package com.iteye.weimingtom.myosotis.nkscript;

public class Sqrt implements Func {
	private Func pValue;

	public Sqrt(Func pValue) {
		this.pValue = pValue;
	}
	
	@Override
	public double Get() {
		return Math.sqrt(this.pValue.Get());
	}
}
