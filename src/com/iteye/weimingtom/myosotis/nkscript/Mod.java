package com.iteye.weimingtom.myosotis.nkscript;

public class Mod implements Func {
	private Func pValue1;
	private Func pValue2;
	
	public Mod(Func pValue1, Func pValue2) {
		this.pValue1 = pValue1;
		this.pValue2 = pValue2;
	}
	
	@Override
	public double Get() {
		return mod((int)this.pValue1.Get(), (int)this.pValue2.Get());
	}
	
	private int mod(int a, int b) {
		if (a >= 0) {
			return a % b;
		} else {
			return a % b + b;
		}
	}
}
