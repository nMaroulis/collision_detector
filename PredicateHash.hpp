#ifndef PREDICATEHASH_H
#define PREDICATEHASH_H

#include "PredicateHashTable.hpp"


class PredicateHash{
    private:
        PredicateHashTable * pTables;
        int pHashNum;
    public:
        PredicateHash();
  		void initPredicateHash(int);
  		ValidationPredicate * insertPredicate(uint32_t,uint64_t,uint64_t,uint32_t,char,uint64_t);
  		bool deletePredicate(ValidationPredicate *,uint64_t);
};

#endif
