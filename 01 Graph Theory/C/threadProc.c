#include <stdio.h>
#include <stdlib.h>

#include "catalog.h"

unsigned char checkTwo(unsigned long num)
{
	unsigned long result = num>>1;
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

unsigned long V(unsigned long num)
{
	return(4*num+1);
}


unsigned long S(unsigned long num)
{
	if(checkTwo(num))
		return(2*num+1);

	return 0;
}

unsigned long thirdRule(unsigned long num)
{
	if(num%3==0)
	{
		return((2*num - 1)/3);
	}
	return 0;
}

unsigned long thirdRuleBis(unsigned long num, unsigned long resultTR)
{
	if (resultTR)
	{
		if( ((num-1)/4) && 1)
		{
			return((resultTR-1)/2);
		}
	}

	return 0;
}

void worker(unsigned long num, unsigned long *result)
{
	/*for (int i=0; i<4; i++)
	{
		result[i] = 0;
	}*/
	
	result[0] = V(num);
	result[1] = S(num);
	result[2] = thirdRule(num);
	result[3] = thirdRuleBis(num,result[2]);
	
	return;
}

int main(int argc, char* argv[])
{
	int max = 32;
	unsigned long result[4];
	Catalog c;

	worker(1,result);
	for(int i = 0; i < 4; i++)
		c.addToProve(result[i]);
	c.print();

	return 0;
}
