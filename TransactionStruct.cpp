#include "TransactionStruct.hpp"

uint64_t TransactionStruct:: getId(){
    return id;
}

Record TransactionStruct:: getRecord(int pos){
    return rec[pos];
}

uint32_t TransactionStruct:: getRowCount(){
    return rowCount;
}

void TransactionStruct:: setId(uint64_t tId){
    id = tId;
}

void TransactionStruct:: setRecord(Record * r){
    rec = r;
}

void TransactionStruct:: setRowCount(int c){
    rowCount = c;
}

void TransactionStruct :: setDeleteRecord(int c){
    if(c>0)
        delRec = (Record**)malloc(sizeof(Record*)*c);
    delSize = 0;
}

/*Record* TransactionStruct :: getRecordByValue(uint64_t key){

    return &rec[binarySearch(key,0,rowCount)];

}*/

uint32_t TransactionStruct :: binarySearch(uint64_t key, uint32_t imin, uint32_t imax){
    if (imax < imin){
        perror("Key Not Found in TransactionStruct");
        exit(1);
    }
    else{
        int imid = (imin+imax)/2;

        if(rec[imid].getValue(0) > key)
            return binarySearch(key, imin, imid - 1);
        else if(rec[imid].getValue(0) < key)
            return binarySearch(key, imid + 1, imax);
        else
            return imid;
    }
}


Record* TransactionStruct :: getDelRecord(int pos){
        return delRec[pos];
}

void TransactionStruct :: addDeleteRecord(Record * r){
    delRec[delSize] = r;
    delSize++;
}

int TransactionStruct :: getDelSize(){
    return delSize;
}

void TransactionStruct :: setDelSize(){
     delSize = 0;
}
