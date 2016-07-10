package com.iteye.weimingtom.myosotis.nkscript;

public class ClassInstance {
	private ClassDefine pDefine;
	private double[] vararray;
	
	public double[] GetVarArray() {
		return this.vararray;
	}
	
	public ClassInstance(ClassDefine pDefine) {
		this.pDefine = pDefine;
		this.vararray = new double[pDefine.GetVarNum()];
		for (int i = 0; i < pDefine.GetVarNum(); ++i) {
			this.vararray[i] = 0;
		}
		if (pDefine.GetConstructer() != null) {
			pDefine.GetConstructer().Run(this);
		}
	}
	
	public void Run() {
		this.pDefine.GetMemFunc().Run(this);
	}
}
