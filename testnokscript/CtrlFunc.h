#pragma once

class CIf : public CFunc
{
public:
	CIf(CFunc* pJoken,CFunc* pBlock,CFunc* pElse){m_pJoken=pJoken;m_pBlock=pBlock;m_pElse=pElse;};
	~CIf(){SAFE_DELETE(m_pJoken);SAFE_DELETE(m_pBlock);SAFE_DELETE(m_pElse);};
	virtual double Get()
	{
		if(m_pJoken->Get())
		{
			m_pBlock->Get();
			return 1;
		}
		else if(m_pElse)
			m_pElse->Get();
		return 0;
	};
private:
	CFunc* m_pJoken;
	CFunc* m_pBlock;
	CFunc* m_pElse;
};

class CWhile : public CFunc
{
public:
	CWhile(CFunc* pJoken,CFunc* pBlock){m_pJoken=pJoken;m_pBlock=pBlock;};
	~CWhile(){SAFE_DELETE(m_pJoken);SAFE_DELETE(m_pBlock);};
	virtual double Get()
	{
		while(m_pJoken->Get())
		{
			try
			{
				m_pBlock->Get();
			}
			catch(const char* str)
			{
				if(strcmp(str,"__BREAK!")==0)
					break;
				else if(strcmp(str,"__CONTINUE!")==0)
					continue;
				else
					throw(str);
			}
		}
		return 0;
	};
private:
	CFunc* m_pJoken;
	CFunc* m_pBlock;
};


class CFor : public CFunc
{
public:
	CFor(CFunc* pFirst,CFunc* pJoken,CFunc* pStep,CFunc* pBlock){m_pFirst=pFirst;m_pJoken=pJoken;m_pStep=pStep;m_pBlock=pBlock;};
	~CFor(){SAFE_DELETE(m_pJoken);SAFE_DELETE(m_pBlock);SAFE_DELETE(m_pFirst);SAFE_DELETE(m_pStep);};
	virtual double Get()
	{
		for(m_pFirst->Get();m_pJoken->Get();m_pStep->Get())
		{
			try
			{
				m_pBlock->Get();
			}catch(const char* str)
			{
				if(strcmp(str,"__BREAK!")==0)
					break;
				else if(strcmp(str,"__CONTINUE!")==0)
					continue;
				else
					throw(str);
			}
		}
		return 0;
	};
private:
	CFunc* m_pFirst;	//初期化
	CFunc* m_pJoken;	//条件式
	CFunc* m_pStep;		//第三式
	CFunc* m_pBlock;	//本体
};

class CBreak : public CFunc
{
public:
	virtual double Get(){throw("__BREAK!");};	//for,whileブロックで例外をキャッチさせる
};

class CContinue : public CFunc
{
public:
	virtual double Get(){throw("__CONTINUE!");};	//for,whileブロックで例外をキャッチさせる
};
