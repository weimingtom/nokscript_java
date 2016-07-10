package com.iteye.weimingtom.myosotis.nkscript;

public class For implements Func {
	private Func pFirst;
	private Func pJoken;
	private Func pStep;
	private Func pBlock;

	public For(Func pFirst, Func pJoken, Func pStep, Func pBlock) {
		this.pFirst = pFirst;
		this.pJoken = pJoken;
		this.pStep = pStep;
		this.pBlock = pBlock;
	}
	
	@Override
	public double Get() {
		for (this.pFirst.Get(); this.pJoken.Get() != 0 ? true: false ; this.pStep.Get()) {
			try {
				this.pBlock.Get();
			} catch(LoopException e) {
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
