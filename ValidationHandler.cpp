#include "ValidationHandler.hpp"


HashRecord nullHash;
extern bool pre_hash; //ama einai true xrhsimopoiei to predicate hash
extern bool forget_f;
extern bool mutex;
extern bool rel_hash;
extern Journal journal;
extern Hash hash_t;
zNode * zombieHead;
zNode * zombieTail;
extern pthread_mutex_t rw_lock; //gia na mporei na 3ekleidwsei to mutex apo edw,afou to parei


ValidationHandler :: ValidationHandler(){}

void ValidationHandler :: processValidate(ValidationStruct* v,uint64_t f){
    ValidationStruct * temp = NULL;
    while(v!=NULL && f>=v->valId){
        cout<<checkValidation(v);
          //print(v);
        temp = v;
        v = v->next;
        temp->deleteValidation();
        delete temp;
    }
}

void ValidationHandler :: main_ProcessValidate(ValidationStruct* v){
    while(v!=NULL){
      v->setResult(checkValidation(v));
      v = v->next;
    }
}

void ValidationHandler :: t_ProcessValidate(ValidationStruct* v, uint32_t count){
    pthread_mutex_unlock(&rw_lock);
    //cout << "  Handler :: Got " << count <<" jobs, starting from validation " << v->valId << endl;
    for(int i=0;i<count;i++){
      v->setResult(checkValidation(v));
      v = v->next;
    }
}

bool ValidationHandler :: checkValidation(ValidationStruct* v){
    uint i=0;
    bool flag = false;
    bool help = false;
    if(pre_hash){
        while(i<v->queryCount && flag == false){
            if(mutex){
                if(v->queries[i].blocked == true)
                    help=true;
            }
            if(!help){
                if(calcPredicate(v->queries[i].relid,v->from,v->to,(QueryStruct)v->queries[i]))
                    flag = true;
            }

            i++;
            help = false;
        }
        return flag;
    }
    else{
        while(i<v->queryCount && flag == false){
            if(mutex){
                if(v->queries[i].blocked == true)
                    help = true;
            }
            if(!help){
                if(calcQueryValidation(v->queries[i].relid,v->from,v->to,(QueryStruct)v->queries[i]))
                    flag = true;
            }

            i++;
            help = false;
        }
        return flag;
    }
}

bool ValidationHandler :: calcQueryValidation(uint64_t relId ,uint64_t from , uint64_t to, QueryStruct q){
    uint64_t i=0;// =  binarySearch(from,binStart,currentSize-1);
    int j=0;
    HashRecord hashResult;
    bool flag =false; // flag-true otan dn exei mesa tou c0
    int cur_size = journal.relations[relId].getCurrentSize();
    if(cur_size == 0)
      return false;
    /* ---------- Keno Query ------------*/
    if(q.columnCount == 0){  
        j = journal.relations[relId].rsearch(from,to);
        if(rel_hash){        // se periptwsh rel hash
          if(j==-1)          // ama den vre8ei ka8olou to transaction
             return false;
        }
        if(journal.relations[relId].transactions[j].getId() < from || journal.relations[relId].transactions[j].getId() > to)
          return false;
        else 
          return true;
    }
    /* ------------- Hash ---------------*/
    if(q.columnCount>0){                                              //
        if(q.columns[0].col == 0 && q.columns[0].op == '0'){          // an to prwto einai c0 =
            hashResult = hashCheckRecords(relId,q.columns[0].value);  // pairnei ta antistoixa records ap to hash
            if(hashResult.getCurSize() == 0)                          // an den vrhke tipota
                return false;                                         // epistrefei false
            flag = true;                                              //
       }                                                              //
    }                                                                 //
    /* ---------- Kanonikh Anazhthsh ------------*/
    if(!flag){
        j = journal.relations[relId].rsearch(from,to);   // h binary search epistrefei to pio kontino megalytero apotelesma,an oxi to idio
        if(rel_hash){                                    // se periptwsh rel hash
          if(j==-1)                                      // ama den vre8ei ka8olou to transaction
             return false;                               //
        }                                                //
        if(journal.relations[relId].transactions[j].getId() < from || journal.relations[relId].transactions[j].getId() > to)
          return false;
        while(j < cur_size && journal.relations[relId].transactions[j].getId() <= to){
            if(validateRecords(journal.relations[relId].transactions[j],q)){
                return true;
            }
            j++;
        }
        return false;
    }
    else{ // psaxnei ap to hashrecord
        j=0;
        while( j < hashResult.getCurSize()){
            if(hashResult.getTransactionId(j) >=from){ //apo to from
                if(hashResult.getTransactionId(j) <= to){ //ama einai mikrotero tou to elegxei
                  if(validateHashRec(hashResult.getRec(j),q))
                    return true;
                }
                else   //ama exei 3eperasei to to vgainei
                   break;
            }
            j++; //epomeno transaction
        }
        return false;
    }
}

bool ValidationHandler :: validateRecords(TransactionStruct t, QueryStruct q){
  uint j=0,i=0;
  bool flag;
    for(j=0;j<t.getRowCount();j++){
        flag = true;
        for(i=0;i<q.columnCount;i++){
            switch (q.columns[i].op) {
             case '0':
                  if(t.rec[j].getValue(q.columns[i].col) == q.columns[i].value){
                  }
                  else{
                    flag = false;
                  }
                  break;
             case '1':
                  if(t.rec[j].getValue(q.columns[i].col) != q.columns[i].value){
                  }
                  else{
                    flag = false;
                  }
                  break;
             case '2':
                  if(t.rec[j].getValue(q.columns[i].col) < q.columns[i].value){
                  }
                  else{
                    flag = false;
                  }
                  break;
             case '3':
                  if(t.rec[j].getValue(q.columns[i].col) <= q.columns[i].value){
                  }
                  else{
                    flag = false;
                  }
                  break;
             case '4':
                  if(t.rec[j].getValue(q.columns[i].col) > q.columns[i].value){
                  }
                  else{
                    flag = false;
                  }
                  break;
             case '5':
                  if(t.rec[j].getValue(q.columns[i].col) >= q.columns[i].value){
                  }
                  else{
                    flag = false;
                  }
                  break;
             default:
                  perror("invalid operator given to TransactionStruct");
                  exit(1);
            }
            if(flag == false){
                break;
            }
            if(i == q.columnCount-1 && flag){
                return true;
            }
        }
      }
      for(j=0;j<t.getDelSize();j++){
          flag = true;
          for(i=0;i<q.columnCount;i++){
              switch (q.columns[i].op) {
               case '0':
                    if(t.delRec[j]->getValue(q.columns[i].col) == q.columns[i].value){
                    }
                    else{
                      flag = false;
                    }
                    break;
               case '1':
                    if(t.delRec[j]->getValue(q.columns[i].col) != q.columns[i].value){
                    }
                    else{
                      flag = false;
                    }
                    break;
               case '2':
                    if(t.delRec[j]->getValue(q.columns[i].col) < q.columns[i].value){
                    }
                    else{
                      flag = false;
                    }
                    break;
               case '3':
                    if(t.delRec[j]->getValue(q.columns[i].col) <= q.columns[i].value){
                    }
                    else{
                      flag = false;
                    }
                    break;
               case '4':
                    if(t.delRec[j]->getValue(q.columns[i].col) > q.columns[i].value){
                    }
                    else{
                      flag = false;
                    }
                    break;
               case '5':
                 if(t.delRec[j]->getValue(q.columns[i].col) >= q.columns[i].value){
                    }
                    else{
                      flag = false;
                    }
                    break;
               default:
                    perror("invalid operator given to TransactionStruct");
                    exit(1);
              }
              if(flag == false)
                  break;
              if(i == q.columnCount-1 && flag)
                  return true;
          }
      }
      return false;
}

bool ValidationHandler :: validateHashRec(Record * rec , QueryStruct q ){
    uint j=0,i=0;
    bool flag;
    flag = true;
    for(i=0;i<q.columnCount;i++){
        switch (q.columns[i].op) {
         case '0':
              if(rec->getValue(q.columns[i].col) == q.columns[i].value){
              }
              else{
                flag = false;
              }
              break;
         case '1':
              if(rec->getValue(q.columns[i].col) != q.columns[i].value){
              }
              else{
                flag = false;
              }
              break;
         case '2':
              if(rec->getValue(q.columns[i].col) < q.columns[i].value){
              }
              else{
                flag = false;
              }
              break;
         case '3':
              if(rec->getValue(q.columns[i].col) <= q.columns[i].value){
              }
              else{
                flag = false;
              }
              break;
         case '4':
              if(rec->getValue(q.columns[i].col) > q.columns[i].value){
              }
              else{
                flag = false;
              }
              break;
         case '5':
              if(rec->getValue(q.columns[i].col) >= q.columns[i].value){
              }
              else{
                flag = false;
              }
              break;
         default:
              perror("invalid operator given to ValidationHandler :: validateHashRec");
              exit(1);
        }
        if(flag == false){
            break;
        }
        if(i == q.columnCount-1 && flag){
            return true;
        }
    }
    return false;
}

HashRecord ValidationHandler :: hashCheckRecords(uint32_t relId, uint64_t val){
  uint64_t x = hash_t.hTables[relId].hashFunction(val,hash_t.hTables[relId].getGlobalDepth());
  int pos = hash_t.hTables[relId].buckets[x]->checkKey(val,'v');
    if( pos != -1){
        return hash_t.hTables[relId].buckets[x]->getRecordByPosition(pos);
    }
    else{
        nullHash.setCurSizeToZero();
        return nullHash;
    }
}

void ValidationHandler :: print(ValidationStruct * v){
  int i,j;
  //cout << pre_hash << endl;
  if(!pre_hash){
    cout<< ">Val: " << v->valId <<" "<< v->from<<","<< v->to <<  endl;
    for(i=0;i<v->queryCount;i++){
      cout << "[ " << v->queries[i].relid << " ";
      for(j=0;j<v->queries[i].columnCount;j++){
        cout << "c" << v->queries[i].columns[j].col << " " << v->queries[i].columns[j].op << " " <<  v->queries[i].columns[j].value<< " ";
      }
      cout<< "]" << endl;
      }
  }
  else{
    cout<< ">Val: " << v->valId <<" "<< v->from<<","<< v->to <<  endl;
    for(i=0;i<v->queryCount;i++){
      cout << "[ " << v->queries[i].relid << " ";
      for(j=0;j<v->queries[i].columnCount;j++){
        cout << "c" << v->queries[i].predicateTable[j]->getCol() << " " << v->queries[i].predicateTable[j]->getOp() << " " <<  v->queries[i].predicateTable[j]->getVal()<< " ";
      }
      cout<< "]" << endl;
    }
  }
}

void ValidationHandler :: insertToZombieList(ValidationPredicate * pr){
    if(zombieHead == NULL){
      zombieHead = (zNode*)malloc(sizeof(zNode*));
      zombieHead->p = pr;
      zombieHead->next=NULL;
      zombieTail = zombieHead;
    }
    else{
      zombieTail->next = (zNode*)malloc(sizeof(zNode*));
      (zombieTail->next)->p = pr;
      zombieTail = zombieTail->next;
      zombieTail->next=NULL;
    }
}

void ValidationHandler :: forget(uint64_t tid, PredicateHash ph,bool flag){
  zNode * temp = NULL;
  zNode * current = zombieHead;
  while(current != NULL){
    if(flag)
      ph.deletePredicate(current->p,tid);
    temp = current;
    current = current->next;
    free(temp);
  }
  zombieHead =NULL;
  zombieTail =NULL;
}

bool ValidationHandler :: calcPredicate(uint64_t relId ,uint64_t from , uint64_t to, QueryStruct q){

    uint64_t i=0;
    int j=0;
    uint k;
    bool flag; //ama ginei true epistrefei false dioti den 8a ypar3ei mellontikh sygkroush
    uint32_t pmax; //mege8os conflict bitmap
    if(q.columnCount == 0){  //KENO QUERY
        j = journal.relations[relId].rsearch(from,to);
        if(rel_hash){        // se periptwsh rel hash
          if(j==-1)          // ama den vre8ei ka8olou to transaction
             return false;
        }
        if(journal.relations[relId].transactions[j].getId() < from || journal.relations[relId].transactions[j].getId() > to)
          return false;
        else 
          return true;
    }
    bool * temp=NULL;  //krataei ta current conflicts
    for(i=0;i<q.columnCount;i++){
        flag = false;
        q.predicateTable[i]->removeValPredicate();    // counter--;
        if(q.predicateTable[i]->getCounter() <= 0)    //mpainei sthn zombie list
          insertToZombieList(q.predicateTable[i]);    //

        if(q.predicateTable[i]->isGhost()==true)
            return false;

        if(!q.predicateTable[i]->isCalculated()){
            setPredicateConflict(q.predicateTable[i]);
        }
        pmax = q.predicateTable[i]->getConflictSize();
        if(pmax == 0)
            return false;
        if(temp == NULL){
            temp = (bool*)malloc(sizeof(bool)*pmax);
            for(k=0;k<pmax;k++){
                temp[k] = q.predicateTable[i]->getConflict(k);
                if(q.predicateTable[i]->getConflict(k) == 1)
                    flag = true;
            }
            if(flag == false){
                free(temp);
                return false;
            }
        }
        else{
            for(k=0;k<pmax;k++){
                if(temp[k] == 1 && q.predicateTable[i]->getConflict(k) == 1){
                    temp[k] = 1;
                    flag = true;
                }
                else
                    temp[k] = 0;
            }
            if(flag == false){
                free(temp);
                return false;
            }
        }
    }
    free(temp);
    return true;
}

void ValidationHandler :: setPredicateConflict(ValidationPredicate * predicate){
  uint64_t i;
  int j;
  uint64_t temp = predicate->getFrom();
  uint32_t relId = predicate->getRelationId();
  int cur_size = journal.relations[relId].getCurrentSize();
  j = journal.relations[relId].rsearch(temp,predicate->getTo());
 // while(j==-1 && temp <= predicate->getTo()){
   //     temp++;
    //    j = journal.relations[relId].rsearch(temp,predicate->getTo());
  //}
  if(j==-1 || journal.relations[relId].transactions[j].getId() > predicate->getTo()){// ama den vre8ei ka8olou to transaction, einai ghost
      predicate->setGhostTransaction();
  }
  else{
    while(j < cur_size && journal.relations[relId].transactions[j].getId() <= predicate->getTo()){
          for(int k=0;k<journal.relations[relId].transactions[j].getRowCount();k++){
              switch (predicate->getOp() ) {
               case '0':
                    if(journal.relations[relId].transactions[j].rec[k].getValue(predicate->getCol()) == predicate->getVal())
                       predicate->setConflict(true);
                     else
                       predicate->setConflict(false);
                    break;
               case '1':
                     if(journal.relations[relId].transactions[j].rec[k].getValue(predicate->getCol()) != predicate->getVal())
                       predicate->setConflict(true);
                     else
                       predicate->setConflict(false);
                    break;
               case '2':
                     if(journal.relations[relId].transactions[j].rec[k].getValue(predicate->getCol()) < predicate->getVal())
                       predicate->setConflict(true);
                     else
                       predicate->setConflict(false);
                    break;
               case '3':
                    if(journal.relations[relId].transactions[j].rec[k].getValue(predicate->getCol()) <= predicate->getVal())
                       predicate->setConflict(true);
                    else
                       predicate->setConflict(false);
                    break;
               case '4':
                     if(journal.relations[relId].transactions[j].rec[k].getValue(predicate->getCol()) > predicate->getVal())
                       predicate->setConflict(true);
                     else
                       predicate->setConflict(false);
                    break;
               case '5':
                     if(journal.relations[relId].transactions[j].rec[k].getValue(predicate->getCol()) >= predicate->getVal())
                       predicate->setConflict(true);
                     else
                       predicate->setConflict(false);
                    break;
               default:
                    perror("invalid operator given to TransactionStruct");
                    exit(1);
              }
          }
          for(int k=0;k<journal.relations[relId].transactions[j].getDelSize();k++){
            switch (predicate->getOp()) {
               case '0':
                    if(journal.relations[relId].transactions[j].delRec[k]->getValue(predicate->getCol()) == predicate->getVal())
                       predicate->setConflict(true);
                     else
                       predicate->setConflict(false);
                    break;
               case '1':
                    if(journal.relations[relId].transactions[j].delRec[k]->getValue(predicate->getCol()) != predicate->getVal())
                       predicate->setConflict(true);
                    else
                       predicate->setConflict(false);
                    break;
               case '2':
                    if(journal.relations[relId].transactions[j].delRec[k]->getValue(predicate->getCol()) < predicate->getVal())
                       predicate->setConflict(true);
                    else
                       predicate->setConflict(false);
                    break;
               case '3':
                    if(journal.relations[relId].transactions[j].delRec[k]->getValue(predicate->getCol()) <= predicate->getVal())
                       predicate->setConflict(true);
                    else
                       predicate->setConflict(false);
                    break;
               case '4':
                    if(journal.relations[relId].transactions[j].delRec[k]->getValue(predicate->getCol()) > predicate->getVal())
                       predicate->setConflict(true);
                    else
                       predicate->setConflict(false);
                    break;
               case '5':
                    if(journal.relations[relId].transactions[j].delRec[k]->getValue(predicate->getCol()) >= predicate->getVal())
                       predicate->setConflict(true);
                    else
                       predicate->setConflict(false);
                    break;
               default:
                    perror("invalid operator given to TransactionStruct");
                    exit(1);
              }
          }
          j++;
    }
  }

}
