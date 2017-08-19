#pragma once

class CEqual : public CFunc
{
public:
	CEqual(CVar *pVariable, CFunc *pValue)
	{
		m_pVariable = pVariable;
		m_pValue = pValue;
	}
	~CEqual()
	{
		SAFE_DELETE(m_pVariable);
		SAFE_DELETE(m_pValue);
	}
	virtual double Get()
	{
		return m_pVariable->Set(m_pValue);
	}
private:
	CVar *m_pVariable;
	CFunc *m_pValue;
};

class CPlusEqual : public CFunc
{
public:
	CPlusEqual(CVar *pVariable,CFunc *pValue)
	{
		m_pVariable = pVariable;
		m_pValue = pValue;
	}
	~CPlusEqual()
	{
		SAFE_DELETE(m_pVariable);
		SAFE_DELETE(m_pValue);
	}
	virtual double Get()
	{
		return m_pVariable->Set(m_pVariable->Get() + m_pValue->Get());
	}
private:
	CVar* m_pVariable;
	CFunc* m_pValue;
};

class CMinusEqual : public CFunc
{
public:
	CMinusEqual(CVar *pVariable, CFunc *pValue)
	{
		m_pVariable = pVariable;
		m_pValue = pValue;
	}
	~CMinusEqual()
	{
		SAFE_DELETE(m_pVariable);
		SAFE_DELETE(m_pValue);
	}
	virtual double Get()
	{
		return m_pVariable->Set(m_pVariable->Get() - m_pValue->Get());
	}
private:
	CVar *m_pVariable;
	CFunc *m_pValue;
};

class CPlus : public CFunc
{
public:
	CPlus(CFunc *pValue1, CFunc *pValue2)
	{
		m_pValue1 = pValue1;
		m_pValue2 = pValue2;
	}
	~CPlus()
	{
		SAFE_DELETE(m_pValue1);
		SAFE_DELETE(m_pValue2);
	}
	virtual double Get()
	{
		return m_pValue1->Get() + m_pValue2->Get();
	}
private:
	CFunc *m_pValue1;
	CFunc *m_pValue2;
};

class CMinus : public CFunc
{
public:
	CMinus(CFunc* pValue1,CFunc* pValue2)
	{
		m_pValue1 = pValue1;
		m_pValue2 = pValue2;
	};
	~CMinus()
	{
		SAFE_DELETE(m_pValue1);
		SAFE_DELETE(m_pValue2);
	}
	virtual double Get()
	{
		return m_pValue1->Get() - m_pValue2->Get();
	}
private:
	CFunc *m_pValue1;
	CFunc *m_pValue2;
};

class CMultiply : public CFunc
{
public:
	CMultiply(CFunc *pValue1, CFunc *pValue2)
	{
		m_pValue1 = pValue1;
		m_pValue2 = pValue2;
	};
	~CMultiply()
	{
		SAFE_DELETE(m_pValue1);
		SAFE_DELETE(m_pValue2);
	}
	virtual double Get()
	{
		return m_pValue1->Get() * m_pValue2->Get();
	}
private:
	CFunc *m_pValue1;
	CFunc *m_pValue2;
};

class CDivide : public CFunc
{
public:
	CDivide(CFunc *pValue1,CFunc *pValue2)
	{
		m_pValue1 = pValue1;
		m_pValue2 = pValue2;
	}
	~CDivide()
	{
		SAFE_DELETE(m_pValue1);
		SAFE_DELETE(m_pValue2);
	};
	virtual double Get()
	{
		return m_pValue1->Get() / m_pValue2->Get();
	}
private:
	CFunc *m_pValue1;
	CFunc *m_pValue2;
};

class CSurplus : public CFunc
{
public:
	CSurplus(CFunc *pValue1, CFunc *pValue2)
	{
		m_pValue1 = pValue1;
		m_pValue2 = pValue2;
	}
	~CSurplus()
	{
		SAFE_DELETE(m_pValue1);
		SAFE_DELETE(m_pValue2);
	}
	virtual double Get()
	{
		return (int)m_pValue1->Get() % (int)m_pValue2->Get();
	}
private:
	CFunc *m_pValue1;
	CFunc *m_pValue2;
};

class CEqualEqual : public CFunc
{
public:
	CEqualEqual(CFunc *pValue1, CFunc *pValue2)
	{
		m_pValue1 = pValue1;
		m_pValue2 = pValue2;
	}
	~CEqualEqual()
	{
		SAFE_DELETE(m_pValue1);
		SAFE_DELETE(m_pValue2);
	}
	virtual double Get()
	{
		return m_pValue1->Get() == m_pValue2->Get();
	}
private:
	CFunc* m_pValue1;
	CFunc* m_pValue2;
};

class CNotEqual : public CFunc
{
public:
	CNotEqual(CFunc* pValue1,CFunc* pValue2)
	{
		m_pValue1 = pValue1;
		m_pValue2 = pValue2;
	}
	~CNotEqual()
	{
		SAFE_DELETE(m_pValue1);
		SAFE_DELETE(m_pValue2);
	}
	virtual double Get()
	{
		return m_pValue1->Get() != m_pValue2->Get();
	}
private:
	CFunc *m_pValue1;
	CFunc *m_pValue2;
};

class CIsSmaller : public CFunc
{
public:
	CIsSmaller(CFunc *pValue1, CFunc *pValue2)
	{
		m_pValue1 = pValue1;
		m_pValue2 = pValue2;
	}
	~CIsSmaller()
	{
		SAFE_DELETE(m_pValue1);
		SAFE_DELETE(m_pValue2);
	}
	virtual double Get()
	{
		return m_pValue1->Get() < m_pValue2->Get();
	}
private:
	CFunc *m_pValue1;
	CFunc *m_pValue2;
};

class CIsLarger : public CFunc
{
public:
	CIsLarger(CFunc *pValue1, CFunc *pValue2)
	{
		m_pValue1 = pValue1;
		m_pValue2 = pValue2;
	}
	~CIsLarger()
	{
		SAFE_DELETE(m_pValue1);
		SAFE_DELETE(m_pValue2);
	}
	virtual double Get()
	{
		return m_pValue1->Get() > m_pValue2->Get();
	}
private:
	CFunc *m_pValue1;
	CFunc *m_pValue2;
};

class CIsLargerEqual : public CFunc
{
public:
	CIsLargerEqual(CFunc *pValue1, CFunc *pValue2)
	{
		m_pValue1 = pValue1;
		m_pValue2 = pValue2;
	}
	~CIsLargerEqual()
	{
		SAFE_DELETE(m_pValue1);
		SAFE_DELETE(m_pValue2);
	}
	virtual double Get()
	{
		return m_pValue1->Get() >= m_pValue2->Get();
	}
private:
	CFunc *m_pValue1;
	CFunc *m_pValue2;
};

class CIsSmallerEqual : public CFunc
{
public:
	CIsSmallerEqual(CFunc *pValue1, CFunc *pValue2)
	{
		m_pValue1 = pValue1;
		m_pValue2 = pValue2;
	}
	~CIsSmallerEqual()
	{
		SAFE_DELETE(m_pValue1);
		SAFE_DELETE(m_pValue2);
	}
	virtual double Get()
	{
		return m_pValue1->Get() <= m_pValue2->Get();
	}
private:
	CFunc *m_pValue1;
	CFunc *m_pValue2;
};

class CAndAnd : public CFunc
{
public:
	CAndAnd(CFunc *pValue1, CFunc *pValue2)
	{
		m_pValue1 = pValue1;
		m_pValue2 = pValue2;
	}
	~CAndAnd()
	{
		SAFE_DELETE(m_pValue1);
		SAFE_DELETE(m_pValue2);
	}
	virtual double Get()
	{
		return m_pValue1->Get() && m_pValue2->Get();
	}
private:
	CFunc *m_pValue1;
	CFunc *m_pValue2;
};

class COrOr : public CFunc
{
public:
	COrOr(CFunc *pValue1,CFunc *pValue2)
	{
		m_pValue1 = pValue1;
		m_pValue2 = pValue2;
	}
	~COrOr()
	{
		SAFE_DELETE(m_pValue1);
		SAFE_DELETE(m_pValue2);
	}
	virtual double Get()
	{
		return m_pValue1->Get() || m_pValue2->Get();
	}
private:
	CFunc *m_pValue1;
	CFunc *m_pValue2;
};
