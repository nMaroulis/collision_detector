#ifndef HASH_H
#define HASH_h


#include "HashTable.hpp"

class Hash{
    private:
       	uint32_t hashNum;
    public:
        Hash();
        void initHash(uint32_t);
        void insertHashRecord(Record * ,uint32_t,uint32_t,uint64_t);
        Record ** traceRecord(uint32_t,uint32_t,const uint64_t *,uint64_t);
        void print(void);
        void freeHashTables(void);
		HashTable * hTables;
};

#endif
