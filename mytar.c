#include "mytar.h"
FILE *out_file;

int main(int argc, char **argv){
    out_file = fopen("testout.tar","w");
    manage_file(argv[1]);
}

char mem_block[512];


