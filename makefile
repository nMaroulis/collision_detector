all: dbCollisionDetector 
	 @printf ".....................................................................\nNAME\n     ./dbCollisionDetector \n\nSYNOPSIS\n     ./dbCollisionDetector [OPTION] ... < [FILE.bin]\n\nDESCRIPTION\n     [-time]              Displays Runtime\n     [-sort]              Sort Queries\n     [-rel_hash]          Hash for Relations\n     [-pre_hash]          Hash for Predicates\n     [-forget]\t\t  Forget Predicates\n     [-mutex]\t\t  Μutual Εxclusion\n     [-threads]\t\t  Multi-threaded Execution\n\nAUTHOR\n     Written by Maroulis Nikolaos and Boras Odysseus.\n.....................................................................\n"

dbCollisionDetector: main.cpp Journal.cpp Relation.cpp Hash.cpp HashTable.cpp Bucket.cpp Record.cpp HashRecord.cpp TransactionStruct.cpp SchemaStruct.cpp ValidationStruct.cpp ValidationHandler.cpp TransactionHash.cpp PredicateHash.cpp PredicateHashTable.cpp PredicateBucket.cpp ValidationPredicate.cpp
	g++ -O3 -pthread -o dbCollisionDetector *.cpp

O1:main.cpp Journal.cpp Relation.cpp Hash.cpp HashTable.cpp Bucket.cpp Record.cpp HashRecord.cpp TransactionStruct.cpp SchemaStruct.cpp ValidationStruct.cpp ValidationHandler.cpp TransactionHash.cpp PredicateHash.cpp PredicateHashTable.cpp PredicateBucket.cpp ValidationPredicate.cpp
	g++ -O1 -pthread -o dbCollisionDetector main.cpp Journal.cpp Relation.cpp  Hash.cpp HashTable.cpp Bucket.cpp Record.cpp HashRecord.cpp TransactionStruct.cpp SchemaStruct.cpp ValidationStruct.cpp ValidationHandler.cpp TransactionHash.cpp PredicateHash.cpp PredicateHashTable.cpp PredicateBucket.cpp ValidationPredicate.cpp

O2: main.cpp Journal.cpp Relation.cpp Hash.cpp HashTable.cpp Bucket.cpp Record.cpp HashRecord.cpp TransactionStruct.cpp SchemaStruct.cpp ValidationStruct.cpp ValidationHandler.cpp TransactionHash.cpp PredicateHash.cpp PredicateHashTable.cpp PredicateBucket.cpp ValidationPredicate.cpp
	g++ -O2 -pthread -o dbCollisionDetector main.cpp Journal.cpp Relation.cpp  Hash.cpp HashTable.cpp Bucket.cpp Record.cpp HashRecord.cpp TransactionStruct.cpp SchemaStruct.cpp ValidationStruct.cpp ValidationHandler.cpp TransactionHash.cpp PredicateHash.cpp PredicateHashTable.cpp PredicateBucket.cpp ValidationPredicate.cpp

default:main.cpp Journal.cpp Relation.cpp Hash.cpp HashTable.cpp Bucket.cpp Record.cpp HashRecord.cpp TransactionStruct.cpp SchemaStruct.cpp ValidationStruct.cpp ValidationHandler.cpp TransactionHash.cpp PredicateHash.cpp PredicateHashTable.cpp PredicateBucket.cpp ValidationPredicate.cpp
	g++ -pthread -o dbCollisionDetector main.cpp Journal.cpp Relation.cpp  Hash.cpp HashTable.cpp Bucket.cpp Record.cpp HashRecord.cpp TransactionStruct.cpp SchemaStruct.cpp ValidationStruct.cpp ValidationHandler.cpp TransactionHash.cpp PredicateHash.cpp PredicateHashTable.cpp PredicateBucket.cpp ValidationPredicate.cpp

info: 
	@printf "> made by: \n\tOdysseas Boras 1115201000096\n\tNikos Maroulis 1115201000212\n"

debug: main.cpp Journal.cpp Relation.cpp Hash.cpp HashTable.cpp Bucket.cpp Record.cpp HashRecord.cpp TransactionStruct.cpp SchemaStruct.cpp ValidationStruct.cpp ValidationHandler.cpp TransactionHash.cpp PredicateHash.cpp PredicateHashTable.cpp PredicateBucket.cpp ValidationPredicate.cpp
	c++ -g -pthread -o debug main.cpp Journal.cpp Relation.cpp  Hash.cpp HashTable.cpp Bucket.cpp Record.cpp HashRecord.cpp TransactionStruct.cpp SchemaStruct.cpp ValidationStruct.cpp ValidationHandler.cpp TransactionHash.cpp PredicateHash.cpp PredicateHashTable.cpp PredicateBucket.cpp ValidationPredicate.cpp

clean:
	rm -rf *o dbCollisionDetector
