#include "Main.h"
#include "NkScript.h"

int main(int argc, char *argv[])
{
	CNkScript *m_pScript = new CNkScript("stage1.txt");
	int initfunc = m_pScript->SearchFunc("Init");
	if (initfunc < 0)
	{
		return -1;
	}
	(*m_pScript->GetFunc())[initfunc]->m_func.Get();
	return 0;
}
