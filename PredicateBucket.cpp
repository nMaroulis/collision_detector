#include "PredicateBucket.hpp"

uint64_t same=0;


PredicateBucket :: PredicateBucket(int ld) : localDepth(ld),currentSize(0),bucketPC(0),maxBucketPC(4){

	//vPredicates = (ValidationPredicate **)malloc(sizeof(ValidationPredicate*)*B_SIZE);

	vPredicates = new ValidationPredicate*[B_SIZE];

	//for(int i=0;i<B_SIZE;i++){
    //    vPredicates[i] = new ValidationPredicate();
	//}
  bucketPointers = (uint64_t *)malloc(sizeof(uint64_t)*maxBucketPC);
}
bool PredicateBucket :: isFull(){
    if(currentSize >= B_SIZE ){
        return true;
    }
    return false;
}

ValidationPredicate * PredicateBucket :: insertToBucket(uint64_t from , uint64_t to , uint32_t col , char op , uint64_t val, uint32_t relid){
    if(currentSize >= B_SIZE){
        perror("Bucket Overflow");
        exit(1);
    }
    ValidationPredicate * temp = new ValidationPredicate();
    temp->setValPredicate(from,to,col,op,val,relid);
    vPredicates[currentSize] = temp;
    currentSize++;
    return vPredicates[currentSize-1];
}

int PredicateBucket :: getLocalDepth(){
    return localDepth;
}

void PredicateBucket :: setLocalDepth(){
	localDepth++;
}

int PredicateBucket :: getCurrentSize(){
    return currentSize;
}

uint64_t PredicateBucket :: getPredicateFrom(int i){
    return vPredicates[i]->getFrom();
}

uint64_t PredicateBucket :: getPredicateTo(int i){
    return vPredicates[i]->getTo();
}

uint32_t PredicateBucket :: getPredicateCol(int i){
    return vPredicates[i]->getCol();
}

char PredicateBucket :: getPredicateOp(int i){
    return vPredicates[i]->getOp();
}

uint64_t PredicateBucket :: getPredicateVal(int i){
    return vPredicates[i]->getVal();
}

ValidationPredicate * PredicateBucket :: getPredicate(int pos){
    return vPredicates[pos];
}

void PredicateBucket:: copyPredicate(ValidationPredicate * vp){
     if(currentSize >= B_SIZE){
        cout << "PredicateBucket :: Cannot copy entry bucket is full " << endl;
    }
    vPredicates[currentSize] = vp;
    currentSize++;
}

int PredicateBucket :: keyExists(uint64_t from , uint64_t to , uint32_t col , char op , uint64_t val){
   int i;
   for(i=0;i<currentSize;i++){
	    if(vPredicates[i]->getFrom()==from && vPredicates[i]->getTo()==to && vPredicates[i]->getCol()==col && vPredicates[i]->getOp()==op && vPredicates[i]->getVal()==val){
		 	  vPredicates[i]->addValPredicate();
        return i;
	  }
   }
   return -1;
}

void PredicateBucket:: deletePredicate(int pos){
    vPredicates[pos] = vPredicates[currentSize-1];
    currentSize--;
}

void PredicateBucket :: addBucketPointer(uint64_t p){
    if(bucketPC >= maxBucketPC){
        maxBucketPC = maxBucketPC*2;
        bucketPointers = (uint64_t *)realloc(bucketPointers,sizeof(uint64_t)*maxBucketPC);
    }
    bucketPointers[bucketPC] = p;
    bucketPC++;
}

uint64_t PredicateBucket :: getBucketPointer(uint64_t pos){
    return bucketPointers[pos];
}

void PredicateBucket :: deleteBucketPointer(uint64_t pos){
    bucketPointers[pos] = bucketPointers[bucketPC-1];
    bucketPC--;
}

uint64_t PredicateBucket :: getBucketPC(){
    return bucketPC;
}

void PredicateBucket :: deletePredicateByValue(uint64_t from , uint64_t to , uint32_t col , char op , uint64_t val){ //apo forget
   int i;
   for(i=0;i<currentSize;i++){
        if(vPredicates[i]->getFrom()==from && vPredicates[i]->getTo()==to && vPredicates[i]->getCol()==col && vPredicates[i]->getOp()==op && vPredicates[i]->getVal()==val){
            vPredicates[i]->freeConflict();                // apodesmeush mnhmhs tou predicate
            delete vPredicates[i];                         //
            vPredicates[i] = vPredicates[currentSize-1];   //
            vPredicates[currentSize-1] = NULL;             //
            currentSize--;                                 //
            break;
        }
    }
}