package com.iteye.weimingtom.myosotis.nkscript;

public class Const implements Func {
	private double value;

	public Const(double value) {
		this.value = value;
	}
	
	@Override
	public double Get() {
		return this.value;
	}
}
