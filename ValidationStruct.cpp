 #include "ValidationStruct.hpp"

ValidationStruct::ValidationStruct(uint64_t id, uint64_t f, uint64_t t, uint32_t q):valId(id), from(f), to(t), queryCount(q){   //Returns the DefineSchema Attributes
	queries = (QueryStruct*)malloc(sizeof(QueryStruct)*queryCount);
    next = NULL;
    currentQuery = 0;
    result = false; // init result
    cost = 0;
    heuristicFunc(0);
};

void ValidationStruct :: heuristicFunc(uint32_t c){
   
      cost = (to-from)*queryCount;
}

uint32_t ValidationStruct :: getCost(){
  return cost;
}

void ValidationStruct :: setResult(bool r){
  result = r;
}

bool ValidationStruct :: getResult(){
  return result;
}
void ValidationStruct :: setQuery( uint32_t rid , uint32_t c){

    queries[currentQuery].relid = rid;
    queries[currentQuery].columnCount = c;
    queries[currentQuery].setCurrentSize(0);
    
    
    if(mutex)
        queries[currentQuery].blocked = false;
    if(!pre_hash){ //normal
      if(c > 0)
        queries[currentQuery].columns = (ColumnStruct*)malloc(sizeof(ColumnStruct)*c);
    }
    else{
      if(c > 0)
        queries[currentQuery].predicateTable = (ValidationPredicate **)malloc(sizeof(ValidationPredicate*)*c);
    }
    currentQuery++;
  
}


void ValidationStruct :: sortQueries(int first , int last){ //using QuickSort algorithm O(n*logn)
    int pivotElement;
    if(first < last)
    {
        pivotElement = pivot(first, last);
        sortQueries(first, pivotElement-1);
        sortQueries(pivotElement+1, last);
    }
}

int ValidationStruct :: pivot(int first, int last) {
    int  p = first;
    int pivotElement = queries[first].columnCount;
    QueryStruct temp;
    for(int i = first+1 ; i <= last ; i++)
    {
        /* If you want to sort the list in the other order, change "<=" to ">" */
        if(queries[i].columnCount <= pivotElement)
        {
            p++;
            temp = queries[i];
            queries[i] = queries[p];
            queries[p] = temp;
        }
    }
    temp = queries[p];
    queries[p] = queries[first];
    queries[first] = temp;
    return p;
}

void ValidationStruct :: deleteValidation(){
    if(!pre_hash){
      for(int i=0;i<queryCount;i++)
        queries[i].freeColumns();
    }
   else{
      for(int i=0;i<queryCount;i++){
        if(queries[i].columnCount > 0)
          delete [] queries[i].predicateTable;
      }
   }

    free(queries);
}

//----------------------------------------------------------
QueryStruct :: QueryStruct(){};

void QueryStruct :: setCurrentSize(uint32_t val){
    currentSize = val;
}

void QueryStruct :: setColumn(uint32_t c , char o , uint64_t v){
    uint64_t i =0 ;
    if(mutex){
       while(i<currentSize){
            if(columns[i].col == c){
                switch (columns[i].op) {
                 case '0':
                       switch (o) {
                             case '0':
                                  if(columns[i].value != v){
                                    blocked = true;
                                  }
                                  break;
                             case '1':
                                  if(columns[i].value == v){
                                    blocked = true;
                                  }
                                  break;
                             case '2':
                                  if(columns[i].value > v){
                                    blocked = true;
                                  }
                                  break;
                             case '3':
                                  if(columns[i].value > v){
                                    blocked = true;
                                  }
                                  break;
                             case '4':
                                  if(columns[i].value < v){
                                        blocked = true;
                                  }
                                  break;
                             case '5':
                                  if(columns[i].value < v){
                                    blocked = true;
                                  }
                                  break;
                            }
                     break;
                 case '1':
                            switch (o) {
                             case '0':
                                  if(columns[i].value == v){
                                    blocked = true;
                                  }
                                  break;
                             }
                      break;
                 case '2':
                            switch (o) {
                             case '0':
                                  if(columns[i].value < v){
                                    blocked = true;
                                  }
                                  break;
                             case '4':
                                  if(columns[i].value < v){
                                    blocked = true;
                                  }
                                  break;
                             case '5':
                                  if(columns[i].value < v){
                                    blocked = true;
                                  }
                                  break;
                             }
                      break;
                 case '3':
                            switch (o) {
                             case '0':
                                  if(columns[i].value < v){
                                    blocked = true;
                                  }
                                  break;
                             case '4':
                                  if(columns[i].value < v){
                                    blocked = true;
                                  }
                                  break;
                             case '5':
                                  if(columns[i].value < v){
                                    blocked = true;
                                  }
                                  break;
                             }
                      break;
                 case '4':
                            switch (o) {
                             case '0':
                                  if(columns[i].value > v){
                                    blocked = true;
                                  }
                                  break;
                             case '2':
                                  if(columns[i].value > v){
                                    blocked = true;
                                  }
                                  break;
                             case '3':
                                  if(columns[i].value > v){
                                    blocked = true;
                                  }
                                  break;
                             }
                      break;
                 case '5':
                            switch (o) {
                             case '0':
                                  if(columns[i].value > v){
                                    blocked = true;
                                  }
                                  break;
                             case '2':
                                  if(columns[i].value > v){
                                    blocked = true;
                                  }
                                  break;
                             case '3':
                                  if(columns[i].value > v){
                                    blocked = true;
                                  }
                                  break;
                             }
                      break;
                 default:
                      perror("invalid operator ");
                      exit(1);
                }
            }
        i++;
        }
    }


    i = 0;
    if(currentSize >0){
      while(i<currentSize && columns[i].col == 0 && columns[i].op == '0')
        i++;
      columns[currentSize] = columns[i];
      columns[i].col = c;
      columns[i].op = o;
      columns[i].value = v;
    }
    else{
        columns[currentSize].col = c;
        columns[currentSize].op = o;
        columns[currentSize].value = v;
    }
    currentSize++;
}

void QueryStruct :: freeColumns(){
    if(columnCount>0)
      free(columns);
}

void QueryStruct :: freePredicates(){
    if(columnCount>0)
      free(predicateTable);
}

uint32_t QueryStruct :: getCurrentSize(){
    return currentSize;
}

void QueryStruct :: addPredicate(ValidationPredicate * predicate){

  uint64_t i=0;
  if(mutex){
       while(i<currentSize){
            if(predicateTable[i]->getCol() == predicate->getCol()){
                switch (predicateTable[i]->getOp()) {
                 case '0':
                       switch (predicate->getOp()) {
                             case '0':
                                  if(predicateTable[i]->getVal() != predicate->getVal()){
                                    blocked = true;

                                  }
                                  break;
                             case '1':
                                  if(predicateTable[i]->getVal() == predicate->getVal()){
                                    blocked = true;

                                  }
                                  break;
                             case '2':
                                  if(predicateTable[i]->getVal() > predicate->getVal()){
                                    blocked = true;

                                  }
                                  break;
                             case '3':
                                  if(predicateTable[i]->getVal() > predicate->getVal()){
                                    blocked = true;

                                  }
                                  break;
                             case '4':
                                  if(predicateTable[i]->getVal() < predicate->getVal()){
                                        blocked = true;
                                  }
                                  break;
                             case '5':
                                  if(predicateTable[i]->getVal() < predicate->getVal()){
                                    blocked = true;

                                  }
                                  break;
                            }
                     break;
                 case '1':
                            switch (predicate->getOp()) {
                             case '0':
                                  if(predicateTable[i]->getVal() == predicate->getVal()){

                                    blocked = true;
                                  }
                                  break;
                             }
                      break;
                 case '2':
                            switch (predicateTable[i]->getOp()) {
                             case '0':
                                  if(predicateTable[i]->getVal() < predicate->getVal()){
                                    blocked = true;

                                  }
                                  break;
                             case '4':
                                  if(predicateTable[i]->getVal() < predicate->getVal()){
                                    blocked = true;

                                  }
                                  break;
                             case '5':
                                  if(predicateTable[i]->getVal() < predicate->getVal()){
                                    blocked = true;

                                  }
                                  break;
                             }
                      break;
                 case '3':
                            switch (predicateTable[i]->getOp()) {
                             case '0':
                                  if(predicateTable[i]->getVal() < predicate->getVal()){
                                    blocked = true;

                                  }
                                  break;
                             case '4':
                                  if(predicateTable[i]->getVal() < predicate->getVal()){
                                    blocked = true;

                                  }
                                  break;
                             case '5':
                                  if(predicateTable[i]->getVal() < predicate->getVal()){
                                    blocked = true;

                                  }
                                  break;
                             }
                      break;
                 case '4':
                            switch (predicateTable[i]->getOp()) {
                             case '0':
                                  if(predicateTable[i]->getVal() > predicate->getVal()){
                                    blocked = true;

                                  }
                                  break;
                             case '2':
                                  if(predicateTable[i]->getVal() > predicate->getVal()){
                                    blocked = true;

                                  }
                                  break;
                             case '3':
                                  if(predicateTable[i]->getVal() > predicate->getVal()){
                                    blocked = true;

                                  }
                                  break;
                             }
                      break;
                 case '5':
                            switch (predicateTable[i]->getOp()) {
                             case '0':
                                  if(predicateTable[i]->getVal() > predicate->getVal()){
                                    blocked = true;

                                  }
                                  break;
                             case '2':
                                  if(predicateTable[i]->getVal() > predicate->getVal()){
                                    blocked = true;

                                  }
                                  break;
                             case '3':
                                  if(predicateTable[i]->getVal() > predicate->getVal()){
                                    blocked = true;

                                  }
                                  break;
                             }
                      break;
                 default:
                      perror("invalid operator ");
                      exit(1);
                }
            }
        i++;
        }
    }
    i = 0;

    if(currentSize > 0){
        uint32_t i = 0;
        while(i<currentSize && predicateTable[i]->getCol() == 0 && predicateTable[i]->getOp()=='0'){
            i++;
        }
        predicateTable[currentSize] = predicateTable[i];
        predicateTable[i] = predicate;
    }
    else{
   //predicateTable[currentSize] = predicate;
        predicateTable[0] = predicate;
    }
    currentSize++;
}

