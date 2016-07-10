package com.iteye.weimingtom.myosotis.nkscript;

public class While implements Func {
	private Func pJoken;
	private Func pBlock;
	
	public While(Func pJoken, Func pBlock) {
		this.pJoken = pJoken;
		this.pBlock = pBlock;
	}
	
	@Override
	public double Get() {
		while (this.pJoken.Get() != 0) {
			try {
				this.pBlock.Get();
			} catch (LoopException e) {
				if ("__BREAK!".equals(e.getMessage())) {
					break;
				} else if ("__CONTINUE!".equals(e.getMessage())) {
					continue;
				} else {
					throw e;
				}
			}
		}
		return 0;
	}
}
