/*
 * Inspired from SIGMOD Programming Contest 2015.
 * http://db.in.tum.de/sigmod15contest/task.html
 * Simple requests parsing and reporting.
**/
#include "ValidationHandler.hpp"  //contains all system headers needed

using namespace std;

//--------------- GLOBAL DATA ----------------------
  #define THREAD_POOL_SIZE 7      // Number of threads to be created
  #define MAIN_JOB 0.9            // main takes whats left of totalCost depending on this var
  //#define SEMKEY1 (key_t) 1000  // semaphore #1 key
  //#define SEMKEY2 (key_t) 1001  // semaphore #2 key
  //#define SHMKEY1 (key_t) 1100  // shared memory key {in case of shared memory usage}
  bool rel_hash;   //ama einai true to relation xrhsimopoiei to hash ki oxi binarySearch
  bool pre_hash;   //ama einai true xrhsimopoiei to predicate hash
  bool sort_q;     //ama einai true kanei sort ta queries ap to mikrotero sto megalytero
  bool print_time; //ama einai true ektypwnei kai ton xrono sto telos
  bool forget_f;   //ama einai true kanei forget sto predicate hash
  bool mutex;      // amoivaios apokleismos
  bool threads;    // xrhsh threads h oxi
  SchemaStruct * sch;
  Journal journal;
  Hash hash_t;
  ValidationStruct * head = NULL;
  ValidationStruct * current = NULL;
  ValidationHandler handler;
  PredicateHash pHash;
  uint64_t totalCost;    //holds the number of the total validation cost on the current run
  /*------- Thread -------*/
    pthread_t tids[THREAD_POOL_SIZE];                       //dhlwsh THREAD_NUM threads
    pthread_mutex_t job_lock = PTHREAD_MUTEX_INITIALIZER;   //paei me to conditional job_cond gia ton sygxronismo twn threads
    pthread_mutex_t rw_lock = PTHREAD_MUTEX_INITIALIZER;    //xrhsimopoieitai sto diavasma/grapsimo sto thread_data wste na mn paei na to diavasei kapoio thread enw einai unitialized
    pthread_mutex_t print_lock = PTHREAD_MUTEX_INITIALIZER; //gia na ginetai safe grapsimo sthn metavlhth print_sem
    pthread_cond_t job_cond = PTHREAD_COND_INITIALIZER;     //molis ginei signal shmainei pws h main exei etoimh douleia kai kapoio thread thn pairnei
    typedef struct thread_data{        // domh gia inter-thread communication
      ValidationStruct * start;        // krataei mia 8esh ths listas
      uint32_t val_count;              // enan int me to poso na proxwrhsei sth lista
    }thread_data;                      //
    thread_data * job_data = NULL;     // dhlwsh tou deikth se ena thread_data
    bool alive_thread;            // while true threads keep running
    static uint32_t print_sem;    // molis ola ta threads exoun kanei th douleia tous, h main emfanizei ta apotelesmata
  /*- - - - - - - - - - - */
//- - - - - - - - - - - - - - - - - - - - - - - - -

void * threadController(void * arg){
  while(alive_thread){
    pthread_cond_wait(&job_cond, &job_lock);
    /*if(!alive_thread){
      cout << "EXIT" << endl;
      pthread_exit(NULL);
    }*/
    handler.t_ProcessValidate(job_data->start,job_data->val_count );
    /*to unlock tou rw_lock mutex ginetai mesa sth synarthsh tou handler*/
    pthread_mutex_lock(&print_lock);
    print_sem++;
    pthread_mutex_unlock(&print_lock);
  }
}

//-------------------  Parser Structs ------------------------
  /// Message types
  typedef enum { Done, DefineSchema, Transaction, ValidationQueries, Flush, Forget } Type_t;

  /// Support operations
  typedef enum { Equal, NotEqual, Less, LessOrEqual, Greater, GreaterOrEqual } Op_t;

  typedef struct MessageHead {
     /// Total message length, excluding this head
     uint32_t messageLen;
     /// The message type
     Type_t type;
  } MessageHead_t;


  typedef struct DefineSchema {
     /// Number of relations
     uint32_t relationCount;
     /// Column counts per relation, one count per relation. The first column is always the primary key
     uint32_t columnCounts[];
  } DefineSchema_t;

  typedef struct Transaction {
     /// The transaction id. Monotonic increasing
     uint64_t transactionId;
     /// The operation counts
     uint32_t deleteCount,insertCount;
     /// A sequence of transaction operations. Deletes first, total deleteCount+insertCount operations
     char operations[];
  } Transaction_t;


  typedef struct TransactionOperationDelete {
     /// The affected relation
     uint32_t relationId;
     /// The row count
     uint32_t rowCount;
     /// The deleted values, rowCount primary keyss
     uint64_t keys[];
  } TransactionOperationDelete_t;


  typedef struct TransactionOperationInsert {
     /// The affected relation
     uint32_t relationId;
     /// The row count
     uint32_t rowCount;
     /// The inserted values, rowCount*relation[relationId].columnCount values
     uint64_t values[];
  } TransactionOperationInsert_t;


  typedef struct ValidationQueries {
     /// The validation id. Monotonic increasing
     uint64_t validationId;
     /// The transaction range
     uint64_t from,to;
     /// The query count
     uint32_t queryCount;
     /// The queries
     char queries[];
  } ValidationQueries_t;

  typedef struct Column {
    /// The column id
    uint32_t column;
    /// The operations
    Op_t op;
    /// The constant
    uint64_t value;
  } Column_t;

  typedef struct Query {

     /// The relation
     uint32_t relationId;
     /// The number of bound columns
     uint32_t columnCount;
     /// The bindings
     Column_t columns[];
  } Query_t;

  typedef struct Flush {
     /// All validations to this id (including) must be answered
     uint64_t validationId;
  } Flush_t;

  typedef struct Forget {
     /// Transactions older than that (including) will not be tested for
     uint64_t transactionId;
  } Forget_t;
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//-------------------  Parser Functions -----------------------
  static uint32_t* schema = NULL;
  static void processDefineSchema(DefineSchema_t *s){
    int i;
    if (schema == NULL) free(schema);
    schema = (uint32_t *) malloc(sizeof(uint32_t)*s->relationCount);
    for(i = 0; i < s->relationCount; i++)
      schema[i] = s->columnCounts[i];
    sch = new SchemaStruct(schema,s->relationCount);     //define schema
    journal.initJournal(sch);                   //create journal
    hash_t.initHash(s->relationCount);                   //create hash
    if(pre_hash)
      pHash.initPredicateHash(s->relationCount);         //init predicate hash

    totalCost = 0;
  }

  static void processTransaction(Transaction_t *t){
    int i,j;
    const char* reader = t->operations;
    /* -----------------------  DELETE  ------------------------- */
    for(i=0; i < t->deleteCount; i++) {
      const TransactionOperationDelete_t* o = (TransactionOperationDelete_t*)reader;
    //-------------------------------------------------------------------------------------
      journal.deleteRelationRecord(o->keys,hash_t.traceRecord(o->relationId, o->rowCount,o->keys,t->transactionId),o->rowCount,o->relationId,t->transactionId);
    //-------------------------------------------------------------------------------------
      reader+=sizeof(TransactionOperationDelete_t)+(sizeof(uint64_t)*o->rowCount);
    }
    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

    /* -----------------------  INSERT  ------------------------- */
    for(i=0; i < t->insertCount; i++) {
      const TransactionOperationInsert_t* o = (TransactionOperationInsert_t*)reader;

    //-------used to temporarly store the parser records------------------------------
      Record * recBuf = new Record[o->rowCount];
      for(j=0;j<o->rowCount;j++){
            recBuf[j].insertValue(o->values,sch->getColumns(o->relationId),j*sch->getColumns(o->relationId));
      }
    //----------insert records-------------------------------------------------------
      journal.insertRelationRecord(t->transactionId, recBuf, o->relationId, o->rowCount);
      hash_t.insertHashRecord(recBuf, o->relationId,o->rowCount,t->transactionId);
      reader+=sizeof(TransactionOperationInsert_t)+(sizeof(uint64_t)*o->rowCount*schema[o->relationId]);
    }
    /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  }

  static void processValidationQueries(ValidationQueries_t *v){
    int i=0,j=0;
    //------------create validations-----------------------------------------------------
    if(head == NULL){
          head = new ValidationStruct(v->validationId, v->from,  v->to, v->queryCount);
          current = head;
    }
    else{
          current->next = new ValidationStruct(v->validationId, v->from,  v->to, v->queryCount);
          current = current->next;
    }
    const char* reader = v->queries;
    if(!pre_hash){
      for(i=0; i < v->queryCount; i++) {
          const Query_t* q = (Query_t*)reader;
          current->setQuery(q->relationId,q->columnCount);
          const Column_t* c = q->columns;
          for(j=0;j<q->columnCount;j++){
            switch (c[j].op) {
               case Equal:
                    //cout << "=";
                    current->queries[i].setColumn(c[j].column,'0',c[j].value);
                    break;
               case NotEqual:
                    // cout << "!=";
                    current->queries[i].setColumn(c[j].column,'1',c[j].value);
                    break;
               case Less:
                    //cout << "<";
                    current->queries[i].setColumn(c[j].column,'2',c[j].value);
                    break;
               case LessOrEqual:
                    // cout << "<=";
                    current->queries[i].setColumn(c[j].column,'3',c[j].value);
                    break;
               case Greater:
                    // cout << ">";
                    current->queries[i].setColumn(c[j].column,'4',c[j].value);
                    break;
               case GreaterOrEqual:
                    //  cout << ">=";
                    current->queries[i].setColumn(c[j].column,'5',c[j].value);
                    break;
               default:
                    perror("invalid operator ");
                    exit(1);
            }
          }
          reader+=sizeof(Query_t)+(sizeof(Column_t)*q->columnCount);
      }
    }
    else{ //predicate hash
        for(i=0; i < v->queryCount; i++) {
            const Query_t* q = (Query_t*)reader;
            current->setQuery(q->relationId,q->columnCount);
            const Column_t* c = q->columns;
            for(j=0;j<q->columnCount;j++){
              switch (c[j].op) {
                 case Equal:
                      current->queries[i].addPredicate(pHash.insertPredicate(q->relationId,v->from,v->to,c[j].column,'0',c[j].value));
                      break;
                 case NotEqual:
                      current->queries[i].addPredicate(pHash.insertPredicate(q->relationId,v->from,v->to,c[j].column,'1',c[j].value));
                      break;
                 case Less:
                      current->queries[i].addPredicate(pHash.insertPredicate(q->relationId,v->from,v->to,c[j].column,'2',c[j].value));
                      break;
                 case LessOrEqual:
                      current->queries[i].addPredicate(pHash.insertPredicate(q->relationId,v->from,v->to,c[j].column,'3',c[j].value));
                      break;
                 case Greater:
                      current->queries[i].addPredicate(pHash.insertPredicate(q->relationId,v->from,v->to,c[j].column,'4',c[j].value));
                      break;
                 case GreaterOrEqual:
                      current->queries[i].addPredicate(pHash.insertPredicate(q->relationId,v->from,v->to,c[j].column,'5',c[j].value));
                      break;
                 default:
                      perror("invalid operator ");
                      exit(1);
              }
            }
            reader+=sizeof(Query_t)+(sizeof(Column_t)*q->columnCount);
        }
    }
    totalCost += current->getCost();
    //-----------------------  SORT  -------------------------------
    if(sort_q && current->queryCount > 1) //&& current->queries[i].flag == false)
      current->sortQueries(0,current->queryCount-1);
  }

  static void processFlush(Flush_t *fl){
        if(threads){  //Using Threads
          //------------- var init -----------------
            uint32_t job_amount = (totalCost*MAIN_JOB) / THREAD_POOL_SIZE; // h douleia pou 8a parei to ka8e thread
            uint32_t cur_job,v_count;                                      // kratane proswrina times
            ValidationStruct * temp = head;                                // gia prospelash ths listas
            ValidationStruct * job_val = NULL;                             // to temp einai o komvos ths listas pou dinetai se ka8e thread
          //------------ job assignment -----------
            //float job_weight = 1.03;
            for(int i=0;i<THREAD_POOL_SIZE;i++){
               // job_amount = (uint32_t)(job_weight*(totalCost / THREAD_POOL_SIZE));
               // job_weight -= 0.01;
                cur_job = 0;
                v_count = 0;
                job_val = temp;
                while(temp!=NULL && cur_job <= job_amount){
                    cur_job += temp->getCost();
                    v_count++;
                    temp = temp->next;
                }
                pthread_mutex_lock(&rw_lock);
                pthread_mutex_unlock(&job_lock);
                job_data->val_count = v_count;
                job_data->start = job_val;
                pthread_cond_signal(&job_cond);
            }
          //---------- Main does some job ---------
            if(temp != NULL)                    //ama exei meinei tpt kalei thn synarthsh gia ypologismo
              handler.main_ProcessValidate(temp); //
          //------- wait for threads to end -------
            while(print_sem < THREAD_POOL_SIZE){
              nanosleep(0,0);
            }
          //------------ print results ------------
            pthread_mutex_lock(&print_lock);
            print_sem = 0;
            temp = head;
            while(head != NULL){
              temp = head;
              cout << head->getResult();
              head = head->next;
              delete temp;
            }
            cout << flush;
            pthread_mutex_unlock(&print_lock);
          //------------ clear vars ---------------
            head = NULL;
            current = NULL;
            totalCost = 0;  // sets the number of total cost to zero
        }
        else{  // without threads
            handler.processValidate(head,fl->validationId);
            head = NULL;
            current = NULL;
            cout<<flush;
        }
  }

  static void processForget(Forget_t *fo){
    //uint64_t fo->transactionId
    if(forget_f)
       handler.forget(fo->transactionId,pHash,true); //diagrafei ta predicates pou exei h zombie list ama plhroun tis syn8hkes
  }
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

void argumentHandler(char ** args, int argNum){
  pre_hash = false;
  rel_hash = false;
  sort_q = false;
  print_time = false;
  forget_f = false;
  mutex = false;
  threads = false;
  for(uint i=1;i<argNum;i++){
    if(!strcmp(args[i],"-sort"))
      sort_q = true;
    else if(!strcmp(args[i],"-rel_hash"))
      rel_hash = true;
    else if(!strcmp(args[i],"-pre_hash"))
      pre_hash = true;
    else if(!strcmp(args[i],"-time"))
      print_time = true;
    else if(!strcmp(args[i],"-forget"))
      forget_f = true;
    else if(!strcmp(args[i],"-mutex"))
      mutex = true;
    else if(!strcmp(args[i],"-threads"))
      threads = true;
    else{
      perror(">Uknown argument given, Please use the following\n\t (1) -rel_hash  (2) -pre_hash\n\t (3) -forget    (4) -sort\n\t (5) -time\t(6) -threads\n");
      exit(1);
    }
  }
}

void d_alloc(void){
    hash_t.freeHashTables();
    journal.freeRecords();
    if(forget_f)
       handler.forget(0,pHash,false);  //diagrafei thn zombie list
    delete sch;
    /*alive_thread = false;               // Threads finish their work and exit
    for(int i=0;i<THREAD_POOL_SIZE;i++){  //
         pthread_mutex_unlock(&job_lock); //
        pthread_cond_signal(&job_cond);   //
    }                                     //      */  
    pthread_mutex_destroy(&job_lock);     //
    pthread_mutex_destroy(&rw_lock);      //
    pthread_mutex_destroy(&print_lock);   //
    pthread_cond_destroy(&job_cond);      //
}


int main(int argc, char **argv) {
  //----------------------- init --------------------------------
    MessageHead_t head;
    void *body = NULL;
    uint32_t len;
    argumentHandler(argv,argc);
    const clock_t begin_time = clock();
    /*---- thread  ----*/
    if(threads){                                                       // Thread Creation
      alive_thread = true;                                             //
      int err;                                                         //
      for(int i=0;i<THREAD_POOL_SIZE;i++){                             //
         err = pthread_create(&tids[i],NULL,&threadController,NULL);   //
         if(err){                                                      //
             cout << "Error:unable to create thread " << err << endl;  //
             exit(-1);                                                 //
         }                                                             //
      }                                                                //
      job_data = new thread_data;                                      //inter-thread communication struct
      print_sem=0;                                                     //print sem inits with 0
    }                                                                  //
    /*- - - - - - - - */
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //--------------------- Parsing Part ----------------------------
      while(1){
        /* Retrieve the message head */
        if (read(0, &head, sizeof(head)) <= 0) { return -1; } // crude error handling, should never happen
        /*printf("HEAD LEN %u \t| HEAD TYPE %u \t| DESC ", head.messageLen, head.type); */
        if (body != NULL) free(body); // Retrieve the message body
        if (head.messageLen > 0 ){
          body = malloc(head.messageLen*sizeof(char));
          if (read(0, body, head.messageLen) <= 0) { return -1; } // crude error handling, should never happen
          len-=(sizeof(head) + head.messageLen);
        }
        switch (head.type) {        // And interpret it
           case Done:   // PROGRAMM ENDS HERE
                /*----------- test prints ---------------*/
                  if(print_time)
                    cout <<endl<< ">>Execution Time: "<< float((clock () - begin_time)/7) / CLOCKS_PER_SEC      <<endl; //PRINT_TIME
                    //journal->print();
                    //hash_t->print();
                /*--------- Deallocate Memory -----------*/
                  d_alloc();
                return 0; // Returns 0 and exits
           case DefineSchema:
                processDefineSchema((DefineSchema_t *)body);
                break;
           case Transaction:
                processTransaction((Transaction_t *)body);
                break;
           case ValidationQueries:
                processValidationQueries((ValidationQueries_t *)body);
                break;
           case Flush:
                processFlush((Flush_t *)body);
                break;
           case Forget:
                processForget((Forget_t *)body);
                break;
           default:
                return -1; // crude error handling, should never happen
        }
      }
      return 0;
}
