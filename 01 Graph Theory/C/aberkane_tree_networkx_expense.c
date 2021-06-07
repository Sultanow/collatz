#include <stdio.h>
#include <stdlib.h>
#include "lib.h"

int V(int num)
{
	return 4*num+1;
}

int S(int num)
{
	return 2*num+1;
}

int fG(int num)
{
	return 2*num-1;
}

int ruleTwo(int num)
{
	int result = (num-1)/2;
	int r = 0;
	int k = 0;

	while (result%2 != 0)
	{
		result = (result-1)/2;
		k+=1;
	}

	r = result/2;

	if(((k%2 == 0) && (r%2==0)) || ((k%2!=0) && (r%2!=0)))
		return(1);
	else
		return(0);
}

void includeNumber(int* numberArr, int number)
{
	if (int len = sizeof(numberArr)/sizeof(int) > 0)
	{
		int left = 0;
		int right = len-1;
		int i = 0;

		while left <= right
		{
			i = (left+right)>>1;
			if numberArr[i] < number
			{
				left += 1;
			}
			else
			{
				right -= 1;
			}
		}

		if numberArr[i] < number
		{
			insert(numberArr, len, i+2, number);
		}
		else
		{
			insert(numberArr, len, i+1, number);
		}
	}
	else
	{
		insert(numberArr, len, len+1, number);
	}

	return;
}

//Translated but not useful in C
//Could use the bsearch function from stdlib
/*
unsigned char isIncluded(int* numberArr, number)
{
	if (int len = sizeof(numberArr)/sizeof(int) > 0)
	{
		int left = 0;
		int right = len-1;
		int i = 0;

		while (left<=right)
		{
			i = (left+right)>>1;
			if (numberArr[i]==number)
				return 1;
			else if (numberArr[i]<number)
				left = i+1;
			else
				right = i-1;
		}

		return (numberArr[i] == number);
	}
	else
		return 0;
}
*/

void removeNumber(int* numberArr, int number)
{
	
}

int getType(int label)
{
	if ((label+1) % 3 == 0)
		return 1;
	else if (label % 3 == 0)
		return 2;
	else if ((label-1)%3 == 0)
		return 3;
	return -1;
}
