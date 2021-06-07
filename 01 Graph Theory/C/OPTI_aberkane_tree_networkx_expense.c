#include <stdlib.h>
#include <stdio.h>

/*
int V(int num)
{
	return(4*num+1);
}

int S(int num)
{
	return(2*num+1);
}

int fG(int num)
{
	return(2*num-1);
}
*/

unsigned char ruleTwo(unsigned long num)
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

int main()
{
	unsigned long tab[150];
	int result[150];

	for (unsigned long i=0; i<150; i++)
	{
		tab[i] = 2*i + 1;
	}

	for (int i=0; i<150; i++)
	{
		result[i] = ruleTwo(tab[i]);
	}

	for (int i=0; i<150; i++)
	{
		printf("%d ", result[i]);
	} 

	return 0;

}
