#ifndef SCHEMASTRUCT_H_
#define SCHEMASTRUCT_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <iostream>

using namespace std;

struct SchemaStruct{
    private:
        int relationNum;
        uint32_t * columns;
    public:
        SchemaStruct(uint32_t *, int);
        int getNum(void);
        int getColumns(uint32_t);
};



/*    Pre Check {min / max}
.:main:.
167:  uint32_t col = sch.getColumns(o->relationId);
      uint64_t min[col] , max[col];
in for : 
	  if(j == 0){
          for(int k=0;k<col;k++){
            min[k]= o->values[k];
            max[k] = o->values[k];
          }
        }
        else{
          for(int k=0;k<col;k++){
            if(min[k] > o->values[col*j+k])
              min[k] = o->values[col*j+k];
            if(max[k] < o->values[col*j+k])
              max[k] = o->values[col*j+k];
          }
        }
    recBuf[j++].insertValue(max,col,0);
    recBuf[j].insertValue(min,col,0);

.:Handler:.
 bool check;
        while(j < cur_size && jo.relations[relId].transactions[j].getId() <= to){
            check = true;
            if(jo.relations[relId].transactions[j].getRowCount() > 0){
              for(int l=0;l<q.columnCount;l++){
                  switch (q.columns[l].op) {
                   case '0':
                        if(jo.relations[relId].transactions[j].rec[jo.relations[relId].transactions[j].getRowCount()].getValue(q.columns[l].col) < q.columns[l].value)
                          check = false;;
                        if(jo.relations[relId].transactions[j].rec[jo.relations[relId].transactions[j].getRowCount()+1].getValue(q.columns[l].col) > q.columns[l].value)
                          check = false;
                        break;
                   case '1':
                        break;
                   case '2': //<
                        if(jo.relations[relId].transactions[j].rec[jo.relations[relId].transactions[j].getRowCount()+1].getValue(q.columns[l].col) >= q.columns[l].value)
                          check = false;
                        break;
                   case '3':
                        if(jo.relations[relId].transactions[j].rec[jo.relations[relId].transactions[j].getRowCount()+1].getValue(q.columns[l].col) > q.columns[l].value)
                          check = false;
                        break;
                   case '4': //>
                        if(jo.relations[relId].transactions[j].rec[jo.relations[relId].transactions[j].getRowCount()].getValue(q.columns[l].col) <= q.columns[l].value)
                          check = false;
                        break;
                   case '5':
                        if(jo.relations[relId].transactions[j].rec[jo.relations[relId].transactions[j].getRowCount()].getValue(q.columns[l].col) < q.columns[l].value)
                          check = false;
                        break;
                   default:
                        perror("invalid operator given to TransactionStruct");
                        exit(1);
                  }
              }
            }
            if(check){
              if(validateRecords(jo.relations[relId].transactions[j],q))
                return true;
            }
*/

/*  Thread implementation alternatives

#include <sys/wait.h> 
#include <semaphore.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <cstdlib>
#include <errno.h>
#include <sys/shm.h>

int job_sem,rw_sem;   // semaforos twn jobs kai semaforos gia read and write
sem_t s1,s2;

//----------- Semaphore Functions -------------
  union semun{
    int val;
    struct semid_ds *buf;
    ushort *array;
  };
  int semUp(int semid,int semnum){ // kanei up tn shmaforo, to prwto orisma einai to id tou shmaforou, kai to deutero h 8esh tou, an einai block shmaforwn
      struct sembuf temp={(unsigned short)semnum, 1, 0};
      if (semop(semid,&temp,1) == -1){
          perror("SemUp");
          return -1;
      }
      return 0;
  }
  int semDown(int semid,int semnum){       // kanei down ton shmaforo
      struct sembuf temp={(unsigned short)semnum, -1, 0};
      if (semop(semid,&temp,1) == -1){
          perror("SemDown");
          return -1;
      }
      return 0;
  }
  int semInit(int Sid,int semnum,int val){  // arxikopoihsh shmaforwn
      union semun arg;
      arg.val = val;
      if (semctl(Sid,semnum,SETVAL,arg)== -1){
          perror("initializing semaphore");
          return -1;
      }
      return 0;
  }
//- - - - - - - - - - - - - - - - - - - - - - -
main()
  if((job_sem = semget(SEMKEY1,1,IPC_CREAT | 0660)) == -1){  // job semaphore creation
      perror("Job Semaphore Semget");                        //
      exit(1);                                               //
  }                                                          //                                                                      //
  if (semInit(job_sem,0,0)== -1){                            // job semaphore initialization
      perror("Job Semaphore init");                          //
      exit(1);                                               //
  }                                                          //
 if((rw_sem = semget(SEMKEY2,1,IPC_CREAT | 0660)) == -1){    // read/write semaphore creation
      perror("Job Semaphore Semget");                        //
      exit(1);                                               //
  }                                                          //                                                                      //
  if (semInit(rw_sem,0,0)== -1){                             // read/write semaphore initialization
      perror("Job Semaphore init");                          //
      exit(1);                                               //
  }                                                          //
 int shmid1 = shmget(SHMKEY1,sizeof(thread_data),IPC_CREAT|0660); // SH
    if(shmid1 < 0){                                                //
        perror("shmid1");                                          //
        exit(1);                                                   //
    }                                                              //
  // job_data = (thread_data *)shmat(shmid1,NULL,0);               //     
  // thread_data tmp;                                              //

  sem_init(&s1, 0, 0);
sem_init(&s2,0,1);
*/

#endif
