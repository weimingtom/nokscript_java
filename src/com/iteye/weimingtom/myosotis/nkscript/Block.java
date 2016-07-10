package com.iteye.weimingtom.myosotis.nkscript;
import java.util.List;

public interface Block extends Func {
	public Var NewVariable(String str);
	
	public double[] GetVarArray(); //FIXME:
	
	public int GetVarNum();
}
