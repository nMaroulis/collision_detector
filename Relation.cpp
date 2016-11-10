#include "Relation.hpp"

extern bool rel_hash;

Relation :: Relation(){ }

void Relation :: initRelation(int relId, int relCol){
    id = relId;
    columns = relCol;
    currentSize = 0;
    //binStart = 0;
    maxSize = REL_MAX_SIZE;
    transactions = (TransactionStruct *)malloc(sizeof(TransactionStruct)*maxSize);
    if(rel_hash)  //ama exeii parei to orisma tote xrhsimopoiei to hash
        transHash.init();
}

void Relation :: insertTransaction(Record * rec, uint64_t transactionId, uint32_t rCount){
    if(currentSize > 0){
        if(transactions[currentSize-1].getId() != transactionId){       //if insert comes first
            if(currentSize >= maxSize){   // In case the array is full , reallocate Memory
                maxSize = 2*maxSize;      // Double the size of the previous block
                transactions = (TransactionStruct *)realloc(transactions,maxSize*sizeof(TransactionStruct));
            }
            transactions[currentSize].setId(transactionId);   // Record Insertion
            transactions[currentSize].setRecord(rec);
            transactions[currentSize].setRowCount(rCount);
            transactions[currentSize].setDelSize();
            currentSize++;
            if(rel_hash)
                transHash.insertHash(transactionId, currentSize-1);
        }
        else{            //if delete comes first
            transactions[currentSize-1].setRecord(rec);
            transactions[currentSize-1].setRowCount(rCount);
        }
    }
    else{
        transactions[currentSize].setId(transactionId);   // Record Insertion
        transactions[currentSize].setRecord(rec);
        transactions[currentSize].setRowCount(rCount);
        transactions[currentSize].setDelSize();
        currentSize++;
        if(rel_hash)
            transHash.insertHash(transactionId, currentSize-1);
    }
}

void Relation :: traceRelationRecord(const uint64_t * k, Record ** recs_id,uint32_t rCount, uint64_t transId){
    if(currentSize >= maxSize){   // In case the array is full , reallocate Memory
            maxSize = 2*maxSize;      // Double the size of the previous block
            transactions = (TransactionStruct *)realloc(transactions,maxSize*sizeof(TransactionStruct));
    }
    int counter = 0;
    for(int i =0;i<rCount;i++){
        if(recs_id[i]!=NULL){ //forget ??
            counter++;
        }
    }
    transactions[currentSize].setDeleteRecord(counter);
    transactions[currentSize].setId(transId);
    transactions[currentSize].setRowCount(0);   //wste ama den er8ei eisagwgh na to gnwrizoume ka8ws 8a nai 0

    for(int i =0;i<rCount;i++){
        if(recs_id[i]!=NULL){
            transactions[currentSize].addDeleteRecord(recs_id[i]);
        }
    }
    currentSize++;
    if(rel_hash)
        transHash.insertHash(transId, currentSize-1);
    delete[] recs_id;
}

int Relation :: binarySearch(int64_t tid , int imin , int imax){
    if (imax < imin){
        //cout << "TID_NOT_FOUND" << endl;
        return -1;
    }
    else{
        int imid = (imin+imax)/2;
        if (transactions[imid].getId() > tid)
            return binarySearch(tid, imin, imid - 1);
        else if (transactions[imid].getId() < tid)
            return binarySearch(tid, imid + 1, imax);
        else
            return imid;
    }
}

int Relation :: binSearch(int64_t tid , int imin , int imax){
    if (imax < imin){
        //cout << "TID_NOT_FOUND" << endl;
        return imin;
    }
    else{
        int imid = (imin+imax)/2;
        if (transactions[imid].getId() > tid)
            return binSearch(tid, imin, imid - 1);
        else if (transactions[imid].getId() < tid)
            return binSearch(tid, imid + 1, imax);
        else
            return imid;
    }
}


void Relation :: printRelationValue(){
    cout<<"size : "<<currentSize<<endl;
    for(int i=0;i<currentSize;i++){
        cout<<"id : "<<transactions[i].getId()<<endl;
        cout<<"----------------"<<endl;
        for(int j=0;j<transactions[i].getRowCount();j++){
            for(int k=0;k<columns;k++){
                cout<<transactions[i].getRecord(j).getValue(k)<<" ";
            }
            cout<<endl;
        }
        for(int j=0;j<transactions[i].getDelSize();j++){
            for(int k=0;k<columns;k++){
                cout<< transactions[i].getDelRecord(j)->getValue(k)<<" ";
            }
            cout<< "(d)"<<endl;
        }
        cout<<endl;

    }
}

int Relation :: getCurrentSize(){
    return currentSize;
}

int Relation :: tSearch(uint64_t tid){
    return transHash.traceTransaction(tid);
}

void Relation :: freeRelationTrans(){
    for(int i=0;i<currentSize;i++){
        for(int j=0;j<transactions[i].getRowCount();j++)
            transactions[i].getRecord(j).freeRecord();
        if(transactions[i].getRowCount()>0)
            delete[] transactions[i].rec;
        if(transactions[i].getDelSize()>0)
            free(transactions[i].delRec);
    }
    free(transactions);
}


int Relation :: rsearch(uint64_t tid, uint64_t to){

   if(rel_hash){
        int res =tSearch(tid);
        while(res == -1){
            if(++tid <= to)
                res = tSearch(tid);
            else
                return -1;
        }
        return res;
   }
   else{
       return binSearch(tid,0,getCurrentSize()-1);
    }
}
