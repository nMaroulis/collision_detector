#include "PredicateHashTable.hpp"


PredicateHashTable::PredicateHashTable() : globalDepth(1), bucketCount(2){      //initialize with 2 buckets
	pBuckets = (PredicateBucket**)malloc(sizeof(PredicateBucket*)*bucketCount);
	pBuckets[0] = new PredicateBucket(1);
	pBuckets[0]->addBucketPointer(0);
	pBuckets[1] = new PredicateBucket(1);
	pBuckets[1]->addBucketPointer(1);
}

void PredicateHashTable :: dupliBuckets(){ //duplicate hash array

        bucketCount = bucketCount*2;
        pBuckets = (PredicateBucket**)realloc(pBuckets,sizeof(PredicateBucket*)*(bucketCount));
        for(uint64_t i=0;i<bucketCount/2;i++){
            pBuckets[i+bucketCount/2]=pBuckets[i];
            pBuckets[i]->addBucketPointer(i+bucketCount/2);
        }
}


ValidationPredicate * PredicateHashTable :: insertPredicate (uint64_t from , uint64_t to , uint32_t col , char op , uint64_t val,uint32_t relid){
	int x = hashFunction(from,to,col,op,val,globalDepth);
	int pos=pBuckets[x]->keyExists(from,to,col,op,val);
    if(pos==-1){          //if key exists save it and stop
        if(pBuckets[x]->isFull()){

            if( pBuckets[x]->getLocalDepth() == globalDepth){
                dupliBuckets();
                globalDepth++;
            }
            PredicateBucket * temp = new PredicateBucket(pBuckets[x]->getLocalDepth()+1);

           if(x >= bucketCount/2){
                for(uint64_t i=0;i<pBuckets[x]->getBucketPC()/2;i++){
                    temp->addBucketPointer(pBuckets[x]->getBucketPointer(i));
                    pBuckets[pBuckets[x]->getBucketPointer(i)] = temp;
                }
                uint64_t malakia = pBuckets[x]->getBucketPC()/2;
                for(int64_t i=malakia-1;i>=0;i--){
                    pBuckets[x]->deleteBucketPointer(i);
                }
            }
            else{
                for(uint64_t i=pBuckets[x]->getBucketPC()/2;i<pBuckets[x]->getBucketPC();i++){
                    temp->addBucketPointer(pBuckets[x]->getBucketPointer(i));
                    pBuckets[pBuckets[x]->getBucketPointer(i)] = temp;
                }
                uint64_t malakia =  pBuckets[x]->getBucketPC();
                for(uint64_t i=malakia/2;i<malakia;i++){
                    pBuckets[x]->deleteBucketPointer(i);
                }
            }
            pBuckets[x]->setLocalDepth();
            for(int i=0;i<pBuckets[x]->getCurrentSize();i++){
                if(pBuckets[hashFunction(pBuckets[x]->getPredicateFrom(i),pBuckets[x]->getPredicateTo(i),pBuckets[x]->getPredicateCol(i),pBuckets[x]->getPredicateOp(i),pBuckets[x]->getPredicateVal(i),globalDepth)] == temp){
                        temp->copyPredicate(pBuckets[x]->getPredicate(i));
                        pBuckets[x]->deletePredicate(i);
                        i--;
                }
            }       //rearrange key

            insertPredicate(from,to,col,op,val,relid);
        }
        else{
            return pBuckets[x]->insertToBucket(from,to,col,op,val,relid);
        }
    }
    else{
        return pBuckets[x]->getPredicate(pos);
    }
}



int PredicateHashTable::getGlobalDepth(){
	return globalDepth;
}


int PredicateHashTable::hashFunction(uint64_t from , uint64_t to , uint32_t col , char op , uint64_t val, int d){
    uint64_t n = from+to+col+val;
    return n%((int)pow(2, d));
}

void PredicateHashTable :: deletePredicate(uint64_t from , uint64_t to , uint32_t col , char op , uint64_t val){
    int x = hashFunction(from,to,col,op,val,globalDepth);
    pBuckets[x]->deletePredicateByValue(from,to,col,op,val);
}