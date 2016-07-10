package com.iteye.weimingtom.myosotis.nkscript;

public class Continue implements Func {

	@Override
	public double Get() {
		throw new LoopException("__CONTINUE!");
//		return 0;
	}
}
