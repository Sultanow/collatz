#ifndef CATALOG_H
#define CATALOG_H

#include <vector>
#include "set.h"
#include <ctime>

class Row
{
	public:
		Row(TYPE nb);
		~Row();

		bool get(TYPE);
		void set(TYPE);

		TYPE size;
		TYPE nbProven;
		std::vector<bool> Proven;
};

class Catalog
{
	public:
		Catalog(TYPE, bool old);
		~Catalog();

		void addNumProven(TYPE num);
		void addTab4NumProven(TYPE tab[4]);
		TYPE getToProcess();

		void check();

		void print();
		void printExpense();

	private:
		Row** eltProven;
		TYPE max;
		TYPE size;
		TYPE lastline;
		Set* numToProcess;
		time_t beginning;
};

#endif
