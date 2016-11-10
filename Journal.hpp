#ifndef JOURNAL_H
#define JOURNAL_H


#include "SchemaStruct.hpp"
#include "Relation.hpp"

class Journal{

    public:
        Journal();
        void initJournal(SchemaStruct *);
        void insertRelationRecord(uint64_t , Record *, uint32_t, uint32_t);
        void print();
        void freeRecords();
        void deleteRelationRecord(const uint64_t*,Record **,uint32_t,uint32_t,uint64_t);
        Relation * relations;

    private:
        int relationNum;  //total number of relations in the journal

};

#endif
