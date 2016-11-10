#include "Hash.hpp"

Hash :: Hash(){}

void Hash :: initHash(uint32_t hCount){
    hashNum = hCount;
    hTables = new HashTable[hashNum];
    for(int i=0;i<hashNum;i++)
        hTables[i].insertHashId(i);
}


void Hash :: insertHashRecord(Record * hrec , uint32_t relId, uint32_t entries , uint64_t tid){
	for(int i=0;i<entries;i++){
		hTables[relId].insertHashTableRecord(hrec[i].getValue(0),tid,&hrec[i]);
	}
}

Record ** Hash :: traceRecord(uint32_t relid,uint32_t rcount,const uint64_t * k,uint64_t tid){
    Record  ** del_recs;
    del_recs = new Record*[rcount];
    for(int i=0;i<rcount;i++){
        del_recs[i] = hTables[relid].traceHashTableRecord(k[i],tid);
    }
    return del_recs;
}

void Hash :: print(){ //TEST_PRINT
    cout << ".: HASH :." << endl;
    //for(int i=0;i<hashNum;i++){
    //    cout << ">Im hash Table " << i << " with Buckets :" << endl;
        hTables[9].printHash();
   // }
}

void Hash :: freeHashTables(){
    for(int i=0;i<hashNum;i++){
        hTables[i].freeBuckets();
    }
    delete [] hTables;
}
