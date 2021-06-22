#ifndef CATALOG_H
#define CATALOG_H

#define TYPE size_t

#include <vector>
#include <set>

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
		Catalog(TYPE);
		~Catalog();

		void addNumProven(TYPE num);
		void addTab4NumProven(TYPE tab[4]);
		TYPE getToProcess();

		void check();

		void print();
		void printSet();
		void printExpense();

	private:
		Row** eltProven;
		TYPE max;
		TYPE size;
		TYPE lastline;
		std::set<TYPE> numToProcess;
};

#endif
