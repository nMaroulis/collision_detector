#ifndef PREDICATEHASHTABLE_H
#define PREDICATEHASHTABLE_H

#include "PredicateBucket.hpp"

#include <math.h>
#include <stdlib.h>

class PredicateHashTable{
    private:
        PredicateBucket ** pBuckets;
        int bucketCount, globalDepth;
    public:
        PredicateHashTable();
        ValidationPredicate * insertPredicate(uint64_t,uint64_t, uint32_t,char,uint64_t,uint32_t);
        void dupliBuckets(void);
		int getGlobalDepth(void);
		int hashFunction(uint64_t, uint64_t, uint32_t, char, uint64_t,int);
		void deletePredicate(uint64_t,uint64_t, uint32_t,char,uint64_t);

};

#endif
