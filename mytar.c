#include "mytar.h"
FILE *out_file;

int main(int argc, char **argv){
    //if file already exists it needs to be wiped
    out_file = fopen("testout.tar","a");
    manage_file(argv[1]);
}

char mem_block[512];


