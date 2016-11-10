#ifndef TRANSACTIONSTRUCT_H_
#define TRANSACTIONSTRUCT_H_

#include "Record.hpp"
#include "ValidationStruct.hpp"

struct TransactionStruct{

    private:
        uint64_t id;
        int delSize;
        uint32_t rowCount;
    public:
        Record * rec;
        Record ** delRec;
        TransactionStruct(){}
        uint64_t getId(void);
        Record getRecord(int);
        uint32_t getRowCount(void);
        void setId(uint64_t);
        void setRecord(Record *);
        void setRowCount(int);
        void setDeleteRecord(int);
        Record* getRecordByValue(uint64_t);
        void addDeleteRecord(Record *);
        void freeDeleteRecord(void);
        int getDelSize(void);
        void setDelSize(void);
        Record * getDelRecord(int);
        uint32_t binarySearch(uint64_t,uint32_t,uint32_t);


};

#endif
