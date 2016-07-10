package com.iteye.weimingtom.myosotis.nkscript;

public class If implements Func {
	private Func pJoken;
	private Func pBlock;
	private Func pElse;
	
	public If(Func pJoken, Func pBlock, Func pElse) {
		this.pJoken = pJoken;
		this.pBlock = pBlock;
		this.pElse = pElse;
	}
	
	@Override
	public double Get() {
		if (this.pJoken.Get() != 0) {
			this.pBlock.Get();
			return 1;
		} else if (this.pElse != null) {
			this.pElse.Get();
		}
		return 0;
	}

}
