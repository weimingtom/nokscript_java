package com.iteye.weimingtom.myosotis.nkscript;

public class PlusEqual implements Func {
	private Var pVariable;
	private Func pValue;
	
	public PlusEqual(Var pVariable, Func pValue) {
		this.pVariable = pVariable;
		this.pValue = pValue;
	}
	
	@Override
	public double Get() {
		return this.pVariable.Set(this.pVariable.Get() + this.pValue.Get());
	}
}
