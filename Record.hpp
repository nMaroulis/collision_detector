#ifndef RECORD_H
#define RECORD_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <iostream>
using namespace std;

struct Record{
    private:
        uint64_t * value;   // Stores Record value
    public:
        Record(){};
        uint64_t getValue(int);//Returns Specified Value
        void insertValue(const uint64_t *,int,int);
        void freeRecord(void);
};

#endif
