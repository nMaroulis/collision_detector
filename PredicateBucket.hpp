#ifndef PREDICATEBUCKET_H
#define PREDICATEBUCKET_H

#include "ValidationPredicate.hpp"


#define B_SIZE 250


class PredicateBucket{
    private:
        //ValidationPredicate ** vPredicates;
        uint64_t * bucketPointers;
        uint64_t bucketPC, maxBucketPC;
        int localDepth;
        //int currentSize;
    public:

        PredicateBucket(int);

        ValidationPredicate ** vPredicates;
        int currentSize;

        bool isFull(void);
		ValidationPredicate * insertToBucket(uint64_t,uint64_t,uint32_t,char,uint64_t,uint32_t);
		int getLocalDepth(void);
		void setLocalDepth(void);
		int getCurrentSize(void);
		uint64_t getPredicateFrom(int );
		uint64_t getPredicateTo(int);
		uint32_t getPredicateCol(int);
		char getPredicateOp(int);
		uint64_t getPredicateVal(int);
		ValidationPredicate * getPredicate(int);
		void copyPredicate(ValidationPredicate *);
		int keyExists(uint64_t,uint64_t,uint32_t,char,uint64_t);
		void deletePredicate(int);
		void addBucketPointer(uint64_t);
		uint64_t getBucketPointer(uint64_t );
		void deleteBucketPointer(uint64_t);
		uint64_t getBucketPC(void);
		void deletePredicateByValue(uint64_t,uint64_t,uint32_t,char,uint64_t);
};

#endif
