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
	Catalog c(max,argv[2][0] == 'o');

	c.add(3);
	c.add(5);

	std::cout << "Row,Difference,Time,";
	for(TYPE i=1;i<=max+3;i++)
	{
		std::cout << i <<",";
	}
	std::cout << "Total Expense" << std::endl;

	while((num = c.getToProcess()) != 0)
	{
		worker(num,result);

		c.add(result[0]);
		c.check();
		c.add(result[1]);
		c.check();
		c.add(result[2]);
		c.check();
		c.add(result[3]);
		c.check();

		//std::cout << "\n";
		//c.printSet();
	}

	//c.printExpense();

	return 0;
}
