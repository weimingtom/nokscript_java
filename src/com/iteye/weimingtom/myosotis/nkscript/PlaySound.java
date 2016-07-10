package com.iteye.weimingtom.myosotis.nkscript;

public class PlaySound implements Func {
	private Func pFunc1;
	private Func pFunc2;
	
	public PlaySound(Func pFunc1, Func pFunc2) {
		this.pFunc1 = pFunc1;
		this.pFunc2 = pFunc2;
	}
	
	@Override
	public double Get() {
		// TODO Auto-generated method stub
		return 0;
	}

}
