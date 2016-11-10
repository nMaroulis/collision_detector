#include "Bucket.hpp"


Bucket :: Bucket(int i): localDepth(i),currentSize(0),maxBucketPC(4),bucketPC(0){
    hRecords = (HashRecord *)malloc(sizeof(HashRecord)*MAX_SIZE);
    bucketPointers = (uint64_t *)malloc(sizeof(uint64_t)*maxBucketPC);

}
 
bool Bucket :: isFull(){
    if(currentSize >= MAX_SIZE ){
        return true;
    }
    return false;
}

void Bucket :: insertToBucket(uint64_t k, uint64_t t,Record * r){
    if(currentSize == MAX_SIZE){
        perror("Bucket Overflow");
        exit(1);
    }
    hRecords[currentSize].setHashRecord(t,k,r);
    currentSize++;
} 

int Bucket :: getLocalDepth(){
    return localDepth;
}
void Bucket :: setLocalDepth(){
	localDepth++;
}
void Bucket :: printMyKeys(){  ///TEST_PRINT
    int i,j;
    for(i=0;i<currentSize;i++){
        cout << hRecords[i].getKey() << "(";
       	for(j=0;j<hRecords[i].getCurSize();j++)
       		cout << hRecords[i].getTransactionId(j) << ",";
       	cout << ")";
    }
    cout << endl;
}
int Bucket :: getCurrentSize(){
    return currentSize;
}
int Bucket :: getRecordKey(int pos){
    return hRecords[pos].getKey();
}
HashRecord Bucket:: getRecord(int pos){
    return hRecords[pos];
}

HashRecord Bucket:: getRecordByValue(uint64_t val){
    for(uint i =0;i<=currentSize;i++){
        if(hRecords[i].getKey() == val)
            return hRecords[i];
    }
    perror("Hash Validation Check Error, key Not Found!");
    exit(1);
}

HashRecord Bucket :: getRecordByPosition(int pos ){
    return hRecords[pos];
}

void Bucket:: copyHashRecord(HashRecord rec){
    if(currentSize >= MAX_SIZE){
        cout << " Cannot copy entry bucket is full " << endl;
    }
    hRecords[currentSize] = rec;
    currentSize++;
}

bool Bucket:: keyExists(uint64_t k,uint64_t tid,bool s,int pos, Record * r){
	int i;
    if(pos == -1){
	    for(i=0;i<currentSize;i++){
		    if(hRecords[i].getKey() == k){
			    hRecords[i].insertTransactionId(tid);
			    hRecords[i].setStatus(s);
                hRecords[i].insertRec(r);
			    return true;
		  }
	   }
	   return false;
    }
    else{
        hRecords[pos].insertTransactionId(tid);
        hRecords[pos].setStatus(s);
        hRecords[pos].copyLastRec();
        return true;
    }
}

int Bucket:: checkKey(uint64_t k, char source){
    int i;
    if(source == 'd'){
        for(i=0;i<currentSize;i++){
            if(hRecords[i].getKey() == k && hRecords[i].getStatus() == 0){
                return i;
            }
        }
        return -1;
    }
    else{
        for(i=0;i<currentSize;i++){
            if(hRecords[i].getKey() == k){
                return i;
            }
        }
        return -1;
    }
}

Record* Bucket:: getLastTransactionRecord(int pos){

    return hRecords[pos].getRec(hRecords[pos].getRecCount()-1);//.getTransactionId(hRecords[pos].getCurSize()-1);
}

bool Bucket:: freeHashRecords(){

    if(currentSize == 0)
        return false;
    else{
        for(int i=0;i<currentSize;i++){
            hRecords[i].freeHashRec();
        }
        currentSize = 0;
        free(hRecords);
        return true;
    }
}

void Bucket:: deleteHashRecord(int pos){
    hRecords[pos] = hRecords[currentSize-1];
    currentSize--;
}

void Bucket :: deleteTrans(uint64_t key, uint64_t transId){

    for(int i=0;i<currentSize;i++){
        if(hRecords[i].getKey() == key){
            if(transId >= hRecords[i].getTransactionId(hRecords[i].getCurSize()-1)){
                deleteHashRecord(i);
            }
        }
    }
}
void Bucket :: addBucketPointer(uint64_t p){
    if(bucketPC >= maxBucketPC){
        maxBucketPC = maxBucketPC*2;
        bucketPointers = (uint64_t *)realloc(bucketPointers,sizeof(uint64_t)*maxBucketPC);
    }
    bucketPointers[bucketPC] = p;
    bucketPC++;


}

uint64_t Bucket :: getBucketPointer(uint64_t pos){

    return bucketPointers[pos];

}
void Bucket :: deleteBucketPointer(uint64_t pos){

    bucketPointers[pos] = bucketPointers[bucketPC-1];
    bucketPC--;
}
uint64_t Bucket :: getBucketPC(){
    return bucketPC;
}



void Bucket :: copyBucketPointer(Bucket * bucket, uint64_t pos){

   if(bucketPC >= maxBucketPC){
       maxBucketPC = maxBucketPC*2;
       bucketPointers = (uint64_t *)realloc(bucketPointers,sizeof(uint64_t)*maxBucketPC);
   }
   bucketPointers[bucketPC] = bucket->getBucketPointer(pos);
   bucketPC++;
   bucket->deleteBucketPointer(pos);
}