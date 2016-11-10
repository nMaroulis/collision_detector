#include "PredicateHash.hpp"

PredicateHash :: PredicateHash(){}

void PredicateHash :: initPredicateHash(int hCount){
    pHashNum = hCount;
    pTables = new PredicateHashTable[pHashNum];
}

ValidationPredicate * PredicateHash :: insertPredicate(uint32_t relid , uint64_t from , uint64_t to , uint32_t col , char op , uint64_t val){
	return pTables[relid].insertPredicate(from,to,col,op,val,relid);
}

bool PredicateHash :: deletePredicate(ValidationPredicate * p,uint64_t tid){
	uint counter = p->getCounter();
	if(counter == 0 && p->getFrom() <= tid){
		pTables[p->getRelationId()].deletePredicate(p->getFrom(),p->getTo(),p->getCol(),p->getOp(),p->getVal());
		return true;
	}
	if(counter >0)
		return true;
	else           //exei counter 0 ara paramenei sthn zombie list
		return false;
}