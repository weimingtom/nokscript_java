package com.iteye.weimingtom.myosotis.nkscript;

public class MemVar implements Var {
	private MemFunc m_pMemFunc;
	private int m_index;
	
	public MemVar(MemFunc pMemFunc, int index) {
		m_pMemFunc = pMemFunc;
		m_index = index;
	}
	
	public double GetVar() {
		if (m_index >= 0) {
			return m_pMemFunc.GetInst().GetVarArray()[m_index];
		}
		/*FIXME: */
		/*
		switch(m_index)
		{
		case -1:
			return m_pMemFunc->GetInst()->GetEnemy()->GetX();
			break;
		case -2:
			return m_pMemFunc->GetInst()->GetEnemy()->GetY();
			break;
		case -3:
			return m_pMemFunc->GetInst()->GetEnemy()->GetImage();
			break;
		case -4:
			return m_pMemFunc->GetInst()->GetEnemy()->GetHP();
			break;
		case -5:
			return m_pMemFunc->GetInst()->GetEnemy()->GetAnm();
			break;
		case -10:
			return g_pPlayer->GetX();
			break;
		case -11:
			return g_pPlayer->GetY();
			break;
		}
		*/
		//FIXME:
		return m_pMemFunc.GetInst().GetVarArray()[m_index];
	}
	
	@Override
	public double Get() {
		return GetVar();
	}
	
	//FIXME:
	@Override
	public double Set(Func pValue) {
		return m_pMemFunc.GetInst().GetVarArray()[m_index] = pValue.Get();
	}
	
	public double Set(double value) {
		return m_pMemFunc.GetInst().GetVarArray()[m_index] = value;
	}
}
