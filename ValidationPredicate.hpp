#ifndef VALIDATIONPREDICATE_H
#define VALIDATIONPREDICATE_H


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <iostream>

using namespace std;

/*************KEY***************
*  from | to | col | op | val  *
*******************************/
class ValidationPredicate{
    private:
    	uint64_t from,to,val;  //key
    	uint32_t col;          //
    	char op;               //
        bool * conflict;
        uint32_t relid,cMax,cCurrent;
        uint counter;
        bool calc, ghost;
    public:
        ValidationPredicate(void);
       // ~ValidationPredicate(void);
        void setValPredicate(uint64_t,uint64_t,uint32_t,char,uint64_t,uint32_t);
		uint64_t getFrom(void);
		uint64_t getTo(void);
		uint32_t getCol(void);
		char getOp(void);
		uint64_t getVal(void);
		void addValPredicate(void);
        void removeValPredicate(void);
        void setConflict(bool);
        bool getConflict(uint);
        void freeConflict(void);
        bool isCalculated(void);
        uint getCounter(void);
        uint32_t getRelationId(void);
        uint32_t getConflictSize(void);
        void setGhostTransaction(void);
        bool isGhost(void);
};

#endif
