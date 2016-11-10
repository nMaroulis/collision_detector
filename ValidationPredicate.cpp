#include "ValidationPredicate.hpp"

ValidationPredicate :: ValidationPredicate(){
}

//ValidationPredicate :: ~ValidationPredicate(){

//}

void ValidationPredicate :: setValPredicate(uint64_t f ,uint64_t t,uint32_t c,char o,uint64_t v,uint32_t rel){
	from = f;
	to = t;
	col = c;
	op = o;
	val = v;
	relid = rel;
	calc = false;
	cCurrent = 0;
    cMax = 2;
	conflict = (bool*)malloc(sizeof(bool)*cMax);
	ghost=false;
    counter =1;
}

uint64_t ValidationPredicate :: getFrom(){
	return from;
}

uint64_t ValidationPredicate :: getTo(){
	return to;
}

uint32_t ValidationPredicate :: getCol(){
	return col;
}

char ValidationPredicate :: getOp(){
	return op;
}

uint64_t ValidationPredicate :: getVal(){
	return val;
}

void ValidationPredicate :: addValPredicate(){
 	counter++;
}

void ValidationPredicate :: removeValPredicate(){
	if(--counter<0){
		perror("ValidationPredicate :: Invalid Predicate Index Removal");
		exit(1);
	}
}

bool ValidationPredicate :: getConflict(uint pos){
	return conflict[pos];
}

void ValidationPredicate :: setConflict(bool b){
	if(cCurrent >= cMax){
		cMax = cMax*2;
		conflict = (bool*)realloc(conflict,cMax*sizeof(bool));
	}
	conflict[cCurrent] = b;
	cCurrent++;
	calc =true;
}

void ValidationPredicate :: freeConflict(){
	free(conflict);
}

uint32_t ValidationPredicate :: getConflictSize(){
	return cCurrent;
}

bool ValidationPredicate :: isCalculated(){
	return calc;
}

uint ValidationPredicate :: getCounter(){
	return counter;
}

uint32_t ValidationPredicate :: getRelationId(){
	return relid;
}

bool ValidationPredicate :: isGhost(){
	return ghost;
}

void ValidationPredicate :: setGhostTransaction(){

	ghost = true;
}
