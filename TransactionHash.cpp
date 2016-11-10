#include "TransactionHash.hpp"

tBucket :: tBucket(int local):localDepth(local), currentSize(0){
    entries = (bucketEntry*)malloc(sizeof(bucketEntry)*BUCKET_SIZE);
}

int tBucket :: getLocalDepth(){

    return localDepth;
}

void tBucket :: setLocalDepth(){
    localDepth++;
}

int tBucket :: getCurrentSize(){
    return currentSize;
}

int tBucket :: getBucketKey(int i){
    return entries[i].tid;

}

bool tBucket :: isFull(){
    if(currentSize >= BUCKET_SIZE ){
        return true;
    }
    return false;

}

void tBucket :: insertEntry(uint64_t transId, int p){

    entries[currentSize].tid = transId;
    entries[currentSize].pos = p;
    currentSize++;

}

bucketEntry tBucket :: getEntry(int pos){
    return entries[pos];
}

void tBucket :: copyEntry(bucketEntry e){
    if(currentSize >= BUCKET_SIZE){
        cout << "TransactionHash:: Cannot copy entry bucket is full " << endl;
    }
    entries[currentSize] = e;
    currentSize++;
}

void tBucket :: deleteEntry(int pos){  //PREPEI NA MPEI BINARY ME TAKSINOMIMENA
    for(int i = pos;i<currentSize -1;i++)
        entries[i] = entries[i+1];
    currentSize--;
}

int tBucket :: findTrans(uint64_t t){

   int x = binarySearch(t,0,currentSize-1);
   if(x==-1)
       return -1;
   return entries[x].pos;
}

void TransactionHash :: init(){

    buckets = (tBucket**)malloc(sizeof(tBucket*)*2);
    buckets[0] = new tBucket(1);
    buckets[1] = new tBucket(1);
    globalDepth = 1;
    bucketCount = 2;

}

int TransactionHash :: hashFunction(uint64_t n , int d){

    int pos=2;
    pos <<=(d-1);
    return n%pos;
}

void TransactionHash :: dupliBuckets(){ //duplicate hash array

        bucketCount = bucketCount*2;
        buckets = (tBucket**)realloc(buckets,sizeof(tBucket*)*(bucketCount));
        for(int i=0;i<bucketCount/2;i++){
            buckets[i+bucketCount/2]=buckets[i];
            //buckets[i].addBucketPointer(i+bucketCount/2);
        }
}


void TransactionHash :: insertHash(uint64_t transId, int pos){

    int x = hashFunction(transId,globalDepth);

    if(buckets[x]->isFull()){
            if( buckets[x]->getLocalDepth() == globalDepth){
                dupliBuckets();
                globalDepth++;
            }
            tBucket * temp = new tBucket(buckets[x]->getLocalDepth()+1);
            if(x >= bucketCount/2){
                for(int i=0;i<bucketCount/2;i++){
                    if(buckets[i] == buckets[x]){
                        buckets[i] = temp;
                    }
                }
            }
            else{
                for(int i=bucketCount/2;i<bucketCount;i++){
                    if(buckets[i] == buckets[x]){
                        buckets[i] = temp;
                    }

                }
            }
            buckets[x]->setLocalDepth();

            for(int i=0;i<buckets[x]->getCurrentSize();i++){
                if(buckets[hashFunction(buckets[x]->getBucketKey(i), globalDepth)] == temp){
                        temp->copyEntry(buckets[x]->getEntry(i));
                        buckets[x]->deleteEntry(i);
                        i--;
                }
            }       //rearrange key
            insertHash(transId, pos);

    }
    else{
        buckets[x]->insertEntry(transId,pos);
    }

} 

int TransactionHash :: traceTransaction(uint64_t tid){
    int x = hashFunction(tid,globalDepth);
    return buckets[x]->findTrans(tid);
}


int tBucket :: binarySearch(int64_t tid , int imin , int imax){
   if (imax < imin){
       return -1;
   }
   else{
       int imid = (imin+imax)/2;
       if (entries[imid].tid > tid)
           return binarySearch(tid, imin, imid - 1);
       else if (entries[imid].tid < tid)
           return binarySearch(tid, imid + 1, imax);
       else
           return imid;
   }
}