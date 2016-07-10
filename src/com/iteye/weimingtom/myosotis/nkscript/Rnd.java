package com.iteye.weimingtom.myosotis.nkscript;

public class Rnd implements Func {

	@Override
	public double Get() {
		return Math.random();
	}
}
