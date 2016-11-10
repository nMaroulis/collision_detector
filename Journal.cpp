#include "Journal.hpp"



Journal :: Journal(){}

void Journal :: initJournal(SchemaStruct * schema){
    relationNum = schema->getNum();
    relations = new Relation[relationNum];
    for(int i=0;i<relationNum;i++){
        relations[i].initRelation(i,schema->getColumns(i));
    }

}
void Journal :: insertRelationRecord(uint64_t transactionId, Record * rec, uint32_t relationId, uint32_t rCount){   //Inserts Record in the Relation subJournal
        relations[relationId].insertTransaction(rec,transactionId, rCount);
}

void Journal :: deleteRelationRecord(const uint64_t* k,Record** recs_id , uint32_t rcount, uint32_t relid , uint64_t tid){
    relations[relid].traceRelationRecord(k,recs_id,rcount,tid);
}

void Journal :: print(){
    cout << ".: JOURNAL RECORDS :." << endl;
    for(int i=0;i<relationNum;i++){
        cout << "RELATION " << i << endl;
        relations[i].printRelationValue();
        cout << "****************************************" << endl;
    }
}

void Journal :: freeRecords(){
    for(int i=0;i<relationNum;i++){
        relations[i].freeRelationTrans();
    }
}
