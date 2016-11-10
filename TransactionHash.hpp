#ifndef TRANSACTIONHASH_H_
#define TRANSACTIONHASH_H_


#define BUCKET_SIZE 20

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <iostream>
#include <math.h>
using namespace std;

struct bucketEntry{
    uint64_t tid;
    int pos;
};

class tBucket{

    private:
        int currentSize;
        int localDepth;
        bucketEntry * entries;
    public:
        tBucket(int);
        void insertEntry(uint64_t, int);
        bool isFull(void);
        int getLocalDepth(void);
        void setLocalDepth(void);
        int getCurrentSize(void);
        int getBucketKey(int);
        bucketEntry getEntry(int);
        void copyEntry(bucketEntry);
        void deleteEntry(int);
        int findTrans(uint64_t);
        int binarySearch(int64_t, int,int);
};

class TransactionHash{

    private:
        tBucket ** buckets;
        int bucketCount;
        int globalDepth;
    public:
        void init(void);
        void insertHash(uint64_t,int);
        void dupliBuckets(void);
        int hashFunction(uint64_t, int);
        int traceTransaction(uint64_t);



};

#endif
