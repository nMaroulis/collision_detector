#ifndef VALIDATIONSTRUCT_H_
#define VALIDATIONSTRUCT_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <iostream>
#include "ValidationPredicate.hpp"

extern bool pre_hash;
extern bool mutex;

using namespace std;

/*   char :: op
	------------
	  0  ::  =
	  1  ::  !=
	  2  ::  <
	  3  ::  <=
	  4  ::  >
	  5  ::  >=
*/

struct ColumnStruct{
	private:

	public:
		uint32_t col;
		uint64_t value;
		char op;
		ColumnStruct(uint32_t,char,uint64_t);
};

struct QueryStruct{
	private:
		uint32_t currentSize;

	public:
        bool blocked;
		uint32_t relid, columnCount;
		ColumnStruct * columns;
		void setColumn(uint32_t,char,uint64_t);
		QueryStruct();
		void setCurrentSize(uint32_t);
		void freeColumns(void);
		void freePredicates(void);
		uint32_t getCurrentSize(void);
		ValidationPredicate ** predicateTable;
       	void addPredicate(ValidationPredicate *);
       	void setMaxCount(uint32_t);
};


struct ValidationStruct{
    private:
        uint32_t currentQuery;
        uint32_t cost;  //kostos tou validation
        bool result;  //krataei to apotelesma
    public:
        ValidationStruct(uint64_t, uint64_t, uint64_t, uint32_t);
        uint64_t valId, from, to;
        uint32_t queryCount;
        QueryStruct * queries;
        ValidationStruct * next;
        void setQuery(uint32_t,uint32_t);
        void sortQueries(int,int);
        int pivot(int,int);
        void deleteValidation(void);
        void heuristicFunc(uint32_t);  //ypologizei to kostos tou validation mesw eurestikhs
        uint32_t getCost(void);
        void setResult(bool);
        bool getResult(void);
};

#endif
