#ifndef HASHTABLE_H
#define HASHTABLE_H


#include "Bucket.hpp"
#include <math.h>
#include <stdlib.h>

class HashTable{
    private:
        int id;
        int globalDepth;
        uint64_t bucketCount;
    public:
        Bucket ** buckets;
        HashTable();
        void insertHashTableRecord(uint64_t,uint64_t,Record *);
        Record* traceHashTableRecord(const uint64_t,uint64_t);
        int getGlobalDepth(void);
        int hashFunction(uint64_t,int);
        void insertHashId(int);
        void dupliBuckets(void);
        void printHash(void);
        void freeBuckets(void);
        void hashForgetTrans(uint64_t,uint64_t);
};

#endif
