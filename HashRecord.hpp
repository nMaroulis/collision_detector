#ifndef HASHRECORD_H
#define HASHRECORD_H


#include "Record.hpp"

struct HashRecord{
    private:
       uint64_t key ;
       int curSize , maxSize;
       uint64_t * transId;
       bool status;
       Record ** rec;
       int recCount;
    public:
        HashRecord(){}
        void setHashRecord(uint64_t,uint64_t,Record *);
        uint64_t getKey(void);
        void  insertTransactionId(uint64_t);
        uint64_t getTransactionId(int);
        int getCurSize(void);
        void setStatus(bool);
        bool getStatus(void);
        void setCurSizeToZero(void);
        Record * getRec(int);
        void insertRec(Record *);
        void freeHashRec(void);
        int getRecCount(void);
        void copyLastRec(void);
};

#endif
