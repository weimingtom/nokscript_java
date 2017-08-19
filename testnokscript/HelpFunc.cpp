#include "Misc.h"
#include "NkScript.h"
#include "HelpFunc.h"

int mod(int a,int b)
{
	if (a >= 0)
	{
		return a % b;
	}
	else
	{
		return a % b + b;
	}
}
