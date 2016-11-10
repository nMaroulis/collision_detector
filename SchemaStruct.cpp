 #include "SchemaStruct.hpp"

 SchemaStruct :: SchemaStruct(uint32_t * col, int num):columns(col), relationNum(num){   //Returns the DefineSchema Attributes
};

int SchemaStruct :: getNum(){
    return relationNum;
}

int SchemaStruct :: getColumns(uint32_t i){
    return columns[i];
}
