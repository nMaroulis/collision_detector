#include "HashRecord.hpp"


void HashRecord:: setHashRecord(uint64_t tId,uint64_t k, Record * r){
        key = k;
        curSize=1;
        maxSize=1;
        transId = (uint64_t*)malloc(sizeof(uint64_t)*maxSize);
        transId[0] = tId;
        status = 0; // 0 == insert | 1 == delete
        rec = new Record*[20];
        recCount = 1;
        rec[0] = r;
}

void HashRecord:: insertTransactionId(uint64_t tId){
	if(curSize >= maxSize){
		maxSize = maxSize*2;
		transId = (uint64_t*)realloc(transId,sizeof(uint64_t)*maxSize);
	}
	transId[curSize] = tId;
	curSize++;
}
uint64_t HashRecord:: getKey(){
	return key;
}
int HashRecord:: getCurSize(){
	return curSize;
}
void HashRecord:: setCurSizeToZero(){   //case hashrecord in val is null
	curSize = 0;
}

uint64_t HashRecord:: getTransactionId(int pos){
	return transId[pos];
}
void HashRecord :: setStatus(bool s){
    status = s;
}

bool HashRecord :: getStatus(){
    return status;
}

void HashRecord :: freeHashRec(){
    free(transId);
}

Record * HashRecord :: getRec(int pos){
	return rec[pos];
}

void HashRecord :: insertRec(Record * r){
	rec[recCount] = r;
	recCount++;
}

int HashRecord :: getRecCount(){
	return recCount;
}

void HashRecord :: copyLastRec(){
	rec[recCount] = rec[recCount-1];
	recCount++;
}