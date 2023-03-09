#include "blockBuffer.h"
char block[BLOCK_SIZE];

void write_block(int value){
    /*takes int, it force, it writes the block out as it and resets it to 0
    push, checks index in block, if nothing has been added to just returns,
    push cannot write an empty block, only force can*/
    if (value == FORCE){
        fwrite(block, 1, BLOCK_SIZE, out_file);
        memset(block, 0, BLOCK_SIZE);
    }else if (value == PUSH){
        ;
    }
    else{
        ;
    }
}
