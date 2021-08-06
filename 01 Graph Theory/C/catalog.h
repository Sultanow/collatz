#ifndef CATALOG_H
#define CATALOG_H

#include <vector>
#include "set.h"
#include <ctime>

class Catalog
{
	public:
		Catalog(TYPE, bool old);
		~Catalog();

		void add(TYPE num);
		TYPE getToProcess();

		void check();

		void printExpense();

	private:
		std::vector<bool> eltProven;
		std::vector<TYPE> nbProven;
		TYPE max;
		int size;
		int lastLine;
		TYPE nbLine;
		Set* numToProcess;
		struct timespec beginning;
};

#endif
