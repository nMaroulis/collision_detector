#include "Record.hpp"

uint64_t Record :: getValue(int pos){   //Returns Specified Value
    return value[pos];
}

void Record :: insertValue(const uint64_t * val, int i, int m){
	value = (uint64_t *)malloc(sizeof(uint64_t)*(i));
    for(int j=0;j<i;j++){
        value[j] = val[j+m];
    }
}

void Record:: freeRecord(){
    free(value);
}
