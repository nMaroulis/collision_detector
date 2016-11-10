#ifndef JOURNAL_H_
#define JOURNAL_H_

#include "TransactionStruct.hpp"
#include "TransactionHash.hpp"
#define REL_MAX_SIZE 5


class Relation{

    public:
        Relation();
        void initRelation(int, int);
        void insertTransaction(Record *, uint64_t, uint32_t);
        void traceRelationRecord(const uint64_t*,Record **,uint32_t,uint64_t);
        int rsearch(uint64_t,uint64_t);
        int binarySearch(int64_t,int,int);
        int binSearch(int64_t,int,int);
        void printRelationValue(void);
        void freeRelationTrans(void);
        int getCurrentSize(void);
        TransactionStruct * transactions;                           // Holds pointers to Records
        int tSearch(uint64_t);   //hash search
    private:
        int id, columns, recordNum, currentSize, maxSize;  // relationId,total num of columns plus one of the transId,curremt records size,records max size
        TransactionHash transHash;
};

#endif
