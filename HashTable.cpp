#include "HashTable.hpp"


HashTable::HashTable() : globalDepth(1), bucketCount(2){ //initialize with 2 buckets
	buckets = (Bucket**)malloc(sizeof(Bucket*)*bucketCount);
	buckets[0] = new Bucket(1);
	buckets[0]->addBucketPointer(0);
	buckets[1] = new Bucket(1);
	buckets[1]->addBucketPointer(1);
}

void HashTable::insertHashId(int hashId){   //TEST
	id = hashId;
}

void HashTable :: dupliBuckets(){ //duplicate hash array

        bucketCount = bucketCount*2;
        buckets = (Bucket**)realloc(buckets,sizeof(Bucket*)*(bucketCount));
        for(uint64_t i=0;i<bucketCount/2;i++){
            buckets[i+bucketCount/2]=buckets[i];
            buckets[i]->addBucketPointer(i+bucketCount/2);
        }
}

void HashTable::insertHashTableRecord(uint64_t hkey , uint64_t tid , Record * r){

    int x = hashFunction(hkey,globalDepth);
    int x1;
    if(!buckets[x]->keyExists(hkey,tid,0,-1,r)){  //if key exists save it and stop
        if(buckets[x]->isFull()){

            if( buckets[x]->getLocalDepth() == globalDepth){
                dupliBuckets();
                globalDepth++;
            }
            Bucket * temp = new Bucket(buckets[x]->getLocalDepth()+1);
          
           if(x >= bucketCount/2){
                for(uint64_t i=0;i<buckets[x]->getBucketPC()/2;i++){
                    temp->addBucketPointer(buckets[x]->getBucketPointer(i));
                    buckets[buckets[x]->getBucketPointer(i)] = temp;
                }
                uint64_t malakia = buckets[x]->getBucketPC()/2;
                for(int64_t i=malakia-1;i>=0;i--){
                    buckets[x]->deleteBucketPointer(i);
                }
            }
            else{
                for(uint64_t i=buckets[x]->getBucketPC()/2;i<buckets[x]->getBucketPC();i++){
                    temp->addBucketPointer(buckets[x]->getBucketPointer(i));
                    buckets[buckets[x]->getBucketPointer(i)] = temp;
                }
                uint64_t malakia =  buckets[x]->getBucketPC();
                for(uint64_t i=malakia/2;i<malakia;i++){
                    buckets[x]->deleteBucketPointer(i);
                }
            }

            buckets[x]->setLocalDepth();
            for(int i=0;i<buckets[x]->getCurrentSize();i++){
                if(buckets[hashFunction(buckets[x]->getRecordKey(i), globalDepth)] == temp){
                        temp->copyHashRecord(buckets[x]->getRecord(i));
                        buckets[x]->deleteHashRecord(i);
                        i--;
                }
            }       //rearrange key
            insertHashTableRecord(hkey,tid,r);
        }
        else{
            buckets[x]->insertToBucket(hkey,tid,r);
        }
    }
}


Record * HashTable:: traceHashTableRecord(const uint64_t k ,uint64_t tid){
    int x = hashFunction(k,globalDepth);
    int pos=buckets[x]->checkKey(k,'d');
    if(pos != -1){
          buckets[x]->keyExists(k,tid,1,pos,NULL);
          return buckets[x]->getLastTransactionRecord(pos);
    }
    else{
        return NULL;
    }
}

int HashTable::getGlobalDepth(){
	return globalDepth;
}

int HashTable::hashFunction(uint64_t n,int d){
    int pos=2;
    pos <<=(d-1);
    return n%pos;
}

void HashTable :: printHash(){   ///TEST_PRINT
    cout << "********************* " << endl;
    for(int i=0;i<bucketCount;i++){
       cout << "Im bucket " << i << " : " ;
        buckets[i]->printMyKeys();
        cout << endl;
    }
    cout<< "**********************" << endl;
}

void HashTable:: freeBuckets(){
    for(int i=0;i<bucketCount;i++){
        if(buckets[i]->freeHashRecords()){


            //delete buckets[i];
        }
    }
    free(buckets);
}

void HashTable :: hashForgetTrans(uint64_t key,uint64_t transId){
    int x = hashFunction(key, globalDepth);
    buckets[x]->deleteTrans(key,transId);
}








