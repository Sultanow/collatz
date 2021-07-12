#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "catalog.h"
#include "rules.h"


void worker(TYPE num, TYPE *result)
{
	result[0] = V(num);
	result[1] = S(num);
	result[2] = thirdRule(num);
	result[3] = thirdRuleBis(num,result[2]);
	
	return;
}

int main(int argc, char* argv[])
{
	TYPE max = atoi(argv[1]);
	TYPE result[4];
	TYPE num;
	Catalog c(max);

	std::cout << "Row,Difference,Time,";
	for(TYPE i=1;i<=max+3;i++)
	{
		std::cout << i <<",";
	}
	std::cout << "Total Expense" << std::endl;

	while((num = c.getToProcess()) != 1)
	{
		worker(num,result);
		//std::cout << num << ": " << result[0] << ", " << result[1] << ", " << result[2] << ", " << result[3] << "\n" ;

		c.addNumProven(result[0]);
		c.check();
		c.addNumProven(result[1]);
		c.check();
		c.addNumProven(result[2]);
		c.check();
		c.addNumProven(result[3]);
		c.check();

		//c.printSet();
	}

	//c.printExpense();

	return 0;
}
