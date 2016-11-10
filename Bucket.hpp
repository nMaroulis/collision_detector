#ifndef BUCKET_H
#define BUCKET_H

#include "HashRecord.hpp"


#define MAX_SIZE 50

struct Bucket{
    private:
        int currentSize, localDepth;
        HashRecord * hRecords;
        uint64_t * bucketPointers;
        uint64_t bucketPC, maxBucketPC;

    public:
        Bucket(int i);
        bool isFull(void);
        void insertToBucket(uint64_t,uint64_t,Record *);
        int getLocalDepth(void);
        void setLocalDepth(void);
        void printMyKeys(void);
        int getCurrentSize(void);
        int getRecordKey(int);
        HashRecord getRecord(int);
        HashRecord getRecordByValue(uint64_t);
        HashRecord getRecordByPosition(int);
        void copyHashRecord(HashRecord);
        void deleteHashRecord(int);
        bool keyExists(uint64_t,uint64_t,bool,int,Record *);
        int checkKey(uint64_t,char);
        Record* getLastTransactionRecord(int);
        bool freeHashRecords(void);
        void deleteTrans(uint64_t, uint64_t);
        void addBucketPointer(uint64_t);

        uint64_t getBucketPointer(uint64_t);
        void deleteBucketPointer(uint64_t);
        uint64_t getBucketPC(void);
        void copyBucketPointer(Bucket *,uint64_t );


};

#endif
