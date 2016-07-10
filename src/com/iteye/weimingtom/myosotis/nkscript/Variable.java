package com.iteye.weimingtom.myosotis.nkscript;

public class Variable implements Var {
	private Block pBlock;
	private int index;
	
	public Variable(Block pBlock, int index) {
		this.pBlock = pBlock;
		this.index = index;
	}
	
	@Override
	public double Get() {
		return this.pBlock.GetVarArray()[this.index];
	}

	@Override
	public double Set(Func pValue) {
		this.pBlock.GetVarArray()[this.index] = pValue.Get();
		return Get();
	}

	@Override
	public double Set(double value) {
		this.pBlock.GetVarArray()[this.index] = value;
		return this.Get();
	}
}
