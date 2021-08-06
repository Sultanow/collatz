#include "rules.h"

bool checkTwo(TYPE num)
{
	TYPE result = num>>1;
	unsigned int r = 0;
	unsigned int k = 0;

	while(result & 1)
	{
		result = result>>1;
		k+=1;
	}
	
	k = k&1;
	r = (result>>1)&1;

	return(k==r);
}

TYPE V(TYPE num)
{
	return(4*num+1);
}


TYPE S(TYPE num)
{
	if(checkTwo(num))
		return(2*num+1);

	return 0;
}

TYPE thirdRule(TYPE num)
{
	if(num%3==2)
	{
		return((2*num - 1)/3);
	}
	return 0;
}

TYPE thirdRuleBis(TYPE num, TYPE resultTR)
{
	if (resultTR)
	{
		if( ((num-1)/4) & 1)
		{
			return((resultTR-1)/2);
		}
	}

	return 0;
}
