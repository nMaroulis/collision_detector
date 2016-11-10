#ifndef VALIDATION_HANDLER_H
#define VALIDATION_HANDLER_H
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <iostream>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include "Journal.hpp"
#include "Hash.hpp"
#include "PredicateHash.hpp"
#include <pthread.h>   // thread related headers

typedef struct ZombieListNode{
  ValidationPredicate * p;
  struct ZombieListNode * next; 
}zNode;


using namespace std;

class ValidationHandler{

    private:
    
    public:
      ValidationHandler();
      void processValidate(ValidationStruct*,uint64_t);
      bool checkValidation(ValidationStruct*);
      bool calcQueryValidation(uint64_t,uint64_t,uint64_t,QueryStruct);
      bool validateRecords(TransactionStruct, QueryStruct);
      bool validateHashRec(Record *,QueryStruct);
      HashRecord hashCheckRecords(uint32_t,uint64_t);
      void print(ValidationStruct*);
      /*------ predicates ------*/
      bool calcPredicate(uint64_t,uint64_t,uint64_t,QueryStruct);
      void setPredicateConflict(ValidationPredicate *);
      void insertToZombieList(ValidationPredicate *);
      void forget(uint64_t,PredicateHash,bool);
      /*-------- threads -------*/
      void t_ProcessValidate(ValidationStruct*, uint32_t);
      void main_ProcessValidate(ValidationStruct* );
};

#endif
