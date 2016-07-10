package com.iteye.weimingtom.myosotis.nkscript;

public class Equal implements Func {
	private Var pVariable;
	private Func pValue; 
	
	public Equal(Var pVariable, Func pValue) {
		this.pVariable = pVariable;
		this.pValue = pValue;
	}
	
	@Override
	public double Get() {
		return this.pVariable.Set(this.pValue);
	}
}
