package com.iteye.weimingtom.myosotis.nkscript;

public class MemFunc implements Block {
	private ClassDefine m_pClassDef;
	private ClassInstance m_pInstance;
	private LogicalBlock m_pBlock;
	
	public ClassInstance GetInst(){
		return m_pInstance;
	};
	
	public MemFunc() {
		m_pClassDef = null;
		m_pInstance = null;
		m_pBlock = null;
	}
	
	public double Get() {
		return 0;
	}
	
	public void Run(ClassInstance pInstance) {
		m_pInstance = pInstance;
		m_pBlock.Get();
	}
	
	public Var NewVariable(String str) {
		if ("x".equals(str)) {
			return new MemVar(this, -1);
		} else if ("y".equals(str)) {
			return new MemVar(this, -2);
		} else if ("img".equals(str)) {
			return new MemVar(this, -3);
		} else if ("hp".equals(str)) {
			return new MemVar(this, -4);
		} else if ("anm".equals(str)) {
			return new MemVar(this, -5);
		} else if ("px".equals(str)) {
			return new MemVar(this,-10);
		} else if ("py".equals(str)) {
			return new MemVar(this, -11);
		}
		for (int i = 0; i < GetVarNum(); ++i) {
			if (str.equals(GetVarNameArray()[i])) {
				return new MemVar(this, i);
			}
		}
		return null;
	}
	
	public String[] GetVarNameArray() {
		return m_pClassDef.GetVarNameArray();
	}
	
	public int GetVarNum() {
		return m_pClassDef.GetVarNum();
	}
	
	public double[] GetVarArray() {
		return m_pInstance.GetVarArray();
	}
	
	public boolean Read(ClassDefine pClassDef, 
		TokenArray pTokenArray, int first) {
		//FIXME:
		//Outputf("%s\n",(*pTokenArray)[first]->GetStr());
		m_pClassDef = pClassDef;
		m_pBlock = null;
		m_pBlock = new LogicalBlock();
		return m_pBlock.Read(this, pTokenArray, first);
	}
}
