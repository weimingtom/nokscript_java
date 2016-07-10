package com.iteye.weimingtom.myosotis.nkscript;

public class Random implements Func {
	private Func pValue1;
	private Func pValue2;
	
	public Random(Func pValue1, Func pValue2) {
		this.pValue1 = pValue1;
		this.pValue2 = pValue2;
	}
	
	@Override
	public double Get() {
		return Rand((int)this.pValue1.Get(), (int)this.pValue2.Get());
	}
	
	public int Rand(int from, int to) {
		return (int)((to - from) * Math.random() + from);
	}
}
