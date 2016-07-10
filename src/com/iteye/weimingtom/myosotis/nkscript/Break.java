package com.iteye.weimingtom.myosotis.nkscript;

public class Break implements Func {

	@Override
	public double Get() {
		throw new LoopException("__BREAK!");
	}
}
