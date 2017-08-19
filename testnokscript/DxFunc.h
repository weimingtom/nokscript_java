#pragma once

class CWaitTime : public CFunc
{
public:
	virtual double Get()
	{
		/*FIXME: */ 
		/*return g_pNkLib->WaitTime();*/ 
		return 0;
	}
};

class CUpdateInput : public CFunc
{
public:
	virtual double Get()
	{
		/*FIXME: */ 
		/*return g_pNkLib->UpdateInput();*/ 
		return 0;
	}
};

class CDrawList : public CFunc
{
public:
	virtual double Get()
	{
		/*FIXME: */ 
		/*return g_pNkLib->DrawList();*/ 
		return 0;
	}
};

class CShowFPS : public CFunc
{
public:
	virtual double Get()
	{
		/*FIXME: */ 
		/*return g_pNkLib->ShowFPS();*/ 
		return 0;
	}
};

class CFlip:public CFunc
{
public:
	virtual double Get()
	{
		/*FIXME: */ 
		/*return g_pNkLib->Flip();*/ 
		return 0;
	}
};

//入力系
class CGetInputState:public CFunc
{
public:
	CGetInputState(CFunc* pFunc)
	{
		m_pFunc = pFunc;
	}
	~CGetInputState()
	{
		SAFE_DELETE(m_pFunc);
	}
	virtual double Get()
	{ 
		/*FIXME: */ 
		/*return g_pNkLib->GetInputState2(m_pFunc->Get());*/ 
		return 0;
	}
private:
	CFunc *m_pFunc;
};

class CGetInputEvent : public CFunc
{
public:
	CGetInputEvent(CFunc *pFunc)
	{
		m_pFunc = pFunc;
	}
	~CGetInputEvent()
	{
		SAFE_DELETE(m_pFunc);
	}
	virtual double Get()
	{
		/*FIXME: */ 
		/*return g_pNkLib->GetInputEvent(m_pFunc->Get());*/ 
		return 0;
	}
private:
	CFunc *m_pFunc;
};

//グラフィック
#define SCRIPT_IMAGE_NUM 128

//extern CObjArray<CNkImage> g_image;
//ロード
class CLoadImage : public CFunc
{
public:
	CLoadImage(CFunc *pFunc, char *str)
	{
		m_pFunc = pFunc;
		m_str = str;
	}
	~CLoadImage()
	{
		SAFE_DELETE(m_pFunc);
		SAFE_DELETE_ARRAY(m_str);
	}
	virtual double Get()
	{
		/* FIXME: */
		/*
		int index=m_pFunc->Get();
		if(g_image[index]==NULL)
			g_image[index]=new CNkImage;
		return g_image[index]->Load(m_str);
		*/
		return 0;
	}
private:
	CFunc *m_pFunc;
	char *m_str;
};

//ロード
class CLoadImage2 : public CFunc
{
public:
	CLoadImage2(CFunc* pFunc, char *str, CFunc *pWidth, CFunc *pHeight)
	{
		m_pFunc = pFunc;
		m_str = str;
		m_pWidth = pWidth;
		m_pHeight = pHeight;
	}
	~CLoadImage2()
	{
		SAFE_DELETE(m_pFunc);
		SAFE_DELETE_ARRAY(m_str);
		SAFE_DELETE(m_pWidth);
		SAFE_DELETE(m_pHeight);
	}
	virtual double Get()
	{
		printf("[CLoadImage2]name:%s, width:%f, height:%f\n", 
			m_str, m_pWidth->Get(), m_pHeight->Get());
		/* FIXME: */
		/*
		int index=m_pFunc->Get();
		if(g_image[index]==NULL)
			g_image[index]=new CNkImage;
		return g_image[index]->Load(m_str,
			m_pWidth->Get(),
			m_pHeight->Get());
		*/
		return 0;
	}
private:
	CFunc *m_pFunc;
	char *m_str;
	CFunc *m_pWidth;
	CFunc *m_pHeight;
};

//描画
class CDraw : public CFunc
{
public:
	CDraw(CFunc* pIndex,CFunc* pLayer,CFunc* pFuncX,CFunc* pFuncY)
	{
		m_pIndex = pIndex;
		m_pLayer = pLayer;
		m_pFuncX = pFuncX;
		m_pFuncY = pFuncY;
	}
	~CDraw()
	{
		SAFE_DELETE(m_pIndex);
		SAFE_DELETE(m_pLayer);
		SAFE_DELETE(m_pFuncX);
		SAFE_DELETE(m_pFuncY);
	}
	virtual double Get()
	{
		/* FIXME: */ 
		/*return g_image[m_pIndex->Get()]->Draw2(
			m_pLayer->Get(),
			m_pFuncX->Get(),
			m_pFuncY->Get());
		*/ 
		return 0;
	}
private:
	CFunc *m_pIndex;
	CFunc *m_pLayer;
	CFunc *m_pFuncX;
	CFunc *m_pFuncY;
};

//描画
class CDraw2 : public CFunc
{
public:
	CDraw2(vector<CFunc*> pFuncArray)
	{
		m_pFuncArray = pFuncArray;
	}
	~CDraw2()
	{
		for (int i = 0; i < m_pFuncArray.size(); ++i)
		{
			SAFE_DELETE(m_pFuncArray[i]);
		}
	}
	virtual double Get()
	{
		/* FIXME: */
		/*
		return g_image[m_pFuncArray[0]->Get()]->Draw2(
			m_pFuncArray[1]->Get(),
			m_pFuncArray[2]->Get(),
			m_pFuncArray[3]->Get(),
			m_pFuncArray[4]->Get(),
			m_pFuncArray[5]->Get());
		*/
		return 0;
	}
private:
	vector<CFunc*> m_pFuncArray;
};

//サウンド
#define SCRIPT_SOUND_NUM 128
//extern CObjArray<CNkSound> g_sound;

class CLoadSound : public CFunc
{
public:
	CLoadSound(CFunc* pFunc,char* str)
	{
		m_pFunc = pFunc;
		m_str = str;
	}
	~CLoadSound()
	{
		SAFE_DELETE(m_pFunc);
		SAFE_DELETE_ARRAY(m_str);
	}
	double Get()
	{
		/* FIXME: */
		/*
		int index=m_pFunc->Get();
		if(g_sound[index]==NULL)
			g_sound[index]=new CNkSound;
		return g_sound[index]->Load(m_str);
		*/
		return 0;
	}
private:
	CFunc *m_pFunc;
	char *m_str;
};

class CPlaySound : public CFunc
{
public:
	CPlaySound(CFunc* pFunc1,CFunc* pFunc2)
	{
		m_pFunc1 = pFunc1;
		m_pFunc2 = pFunc2;
	}
	~CPlaySound()
	{
		SAFE_DELETE(m_pFunc1);
		SAFE_DELETE(m_pFunc2);
	}
	double Get()
	{
		/*return g_sound[m_pFunc1->Get()]->Play(m_pFunc2->Get());*/ 
		return 0;
	}
private:
	CFunc *m_pFunc1;
	CFunc *m_pFunc2;
};

//追加関数
//ショット発生
class CShot : public CFunc
{
public:
	CShot(vector<CFunc*> pFuncArray)
	{
		m_pFuncArray = pFuncArray;
	}
	~CShot()
	{
		for(int i = 0; i < m_pFuncArray.size(); ++i)
		{
			SAFE_DELETE(m_pFuncArray[i]);
		}
	}
	virtual double Get()
	{
		/* FIXME: */
		/*
		g_enemyShot.Add(new CEnemyShot(
			g_image[m_pFuncArray[0]->Get()],
			VECT(m_pFuncArray[1]->Get(),m_pFuncArray[2]->Get()),
			VECT(m_pFuncArray[3]->Get(),m_pFuncArray[4]->Get()),
			m_pFuncArray[5]->Get()));
		*/
		return 1;
	};
private:
	vector<CFunc*> m_pFuncArray;
};

//爆発発生
class CExplode : public CFunc
{
public:
	CExplode(vector<CFunc*> pFuncArray)
	{
		m_pFuncArray = pFuncArray;
	}
	~CExplode()
	{
		for (int i = 0; i < m_pFuncArray.size(); ++i)
		{
			SAFE_DELETE(m_pFuncArray[i]);
		}
	}
	virtual double Get()
	{
		/* FIXME: */
		/*
		g_effect.Add(new CEffect(
			g_image[m_pFuncArray[0]->Get()],
			VECT(m_pFuncArray[1]->Get(),m_pFuncArray[2]->Get()),
			m_pFuncArray[3]->Get(),m_pFuncArray[4]->Get()));
		*/
		return 1;
	};
private:
	vector<CFunc*> m_pFuncArray;
};

//全ての敵弾をアイテムに
class CShotToItem : public CFunc
{
public:
	CShotToItem()
	{

	}
	virtual double Get()
	{
		/* FIXME: */ 
		/*ShotToItem();*/
		return 0;
	}
};

//地形
class CIsTikei : public CFunc
{
public:
	CIsTikei(CFunc* pValue1,CFunc* pValue2)
	{
		m_pValue1 = pValue1;
		m_pValue2 = pValue2;
	}
	~CIsTikei()
	{
		SAFE_DELETE(m_pValue1);
		SAFE_DELETE(m_pValue2);
	}
	virtual double Get()
	{
		/* FIXME: */ 
		/*return g_pStage->CollTikei(m_pValue1->Get(),
			m_pValue2->Get());
		*/ 
		return 0;
	}
private:
	CFunc *m_pValue1;
	CFunc *m_pValue2;
};
