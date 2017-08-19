#pragma once

class CSin : public CFunc
{
public:
	CSin(CFunc* pValue)
	{
		m_pValue = pValue;
	}
	~CSin()
	{
		SAFE_DELETE(m_pValue);
	}
	virtual double Get()
	{
		return sin(m_pValue->Get());
	}
private:
	CFunc *m_pValue;
};

class CCos : public CFunc
{
public:
	CCos(CFunc* pValue)
	{
		m_pValue = pValue;
	}
	~CCos()
	{
		SAFE_DELETE(m_pValue);
	}
	virtual double Get()
	{
		return cos(m_pValue->Get());
	}
private:
	CFunc *m_pValue;
};

class CTan : public CFunc
{
public:
	CTan(CFunc* pValue)
	{
		m_pValue = pValue;
	}
	~CTan()
	{
		SAFE_DELETE(m_pValue);
	}
	virtual double Get()
	{
		return tan(m_pValue->Get());
	}
private:
	CFunc* m_pValue;
};

class CAtan : public CFunc
{
public:
	CAtan(CFunc* pValue)
	{
		m_pValue = pValue;
	}
	~CAtan()
	{
		SAFE_DELETE(m_pValue);
	}
	virtual double Get()
	{
		return atan(m_pValue->Get());
	}
private:
	CFunc* m_pValue;
};

class CSqrt : public CFunc
{
public:
	CSqrt(CFunc* pValue)
	{
		m_pValue = pValue;
	}
	~CSqrt()
	{
		SAFE_DELETE(m_pValue);
	}
	virtual double Get()
	{
		return sqrt(m_pValue->Get());
	}
private:
	CFunc *m_pValue;
};

class CAtan2 : public CFunc
{
public:
	CAtan2(CFunc* pValue1,CFunc* pValue2)
	{
		m_pValue1 = pValue1;
		m_pValue2 = pValue2;
	}
	~CAtan2()
	{
		SAFE_DELETE(m_pValue1);
		SAFE_DELETE(m_pValue2);
	}
	virtual double Get()
	{
		return atan2(m_pValue1->Get(), m_pValue2->Get());
	}
private:
	CFunc *m_pValue1;
	CFunc *m_pValue2;
};

class CAbs : public CFunc
{
public:
	CAbs(CFunc* pValue)
	{
		m_pValue = pValue;
	}
	~CAbs()
	{
		SAFE_DELETE(m_pValue);
	}
	virtual double Get()
	{
		//FIXME: abs -> fabs
		return fabs(m_pValue->Get());
	}
private:
	CFunc* m_pValue;
};

class CRnd : public CFunc
{
public:
	virtual double Get()
	{
		return (double)rand() / RAND_MAX;
	}
};

inline int Rand(int from, int to)
{
#ifdef _MSC_VER
	return (to - from) * rand() / (RAND_MAX + 1) + from;
#else
	return (to - from) * rand() / (RAND_MAX) + from;
#endif
}

class CRandom : public CFunc
{
public:
	CRandom(CFunc* pValue1,CFunc* pValue2)
	{
		m_pValue1 = pValue1;
		m_pValue2 = pValue2;
	}
	~CRandom()
	{
		SAFE_DELETE(m_pValue1);
		SAFE_DELETE(m_pValue2);
	}
	virtual double Get()
	{
		return Rand(m_pValue1->Get(), m_pValue2->Get());
	}
private:
	CFunc* m_pValue1;
	CFunc* m_pValue2;
};

extern int mod(int a, int b);

class CMod : public CFunc
{
public:
	CMod(CFunc* pValue1,CFunc* pValue2)
	{
		m_pValue1 = pValue1;
		m_pValue2 = pValue2;
	}
	~CMod()
	{
		SAFE_DELETE(m_pValue1);
		SAFE_DELETE(m_pValue2);
	}
	virtual double Get()
	{
		return mod(m_pValue1->Get(), m_pValue2->Get());
	}
private:
	CFunc *m_pValue1;
	CFunc *m_pValue2;
};
