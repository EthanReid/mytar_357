#include "mytar.h"

int v_flag = 0;
int f_flag = 0;
FILE *out_file = NULL;

int main(int argc, char *argv[]) {
    // Declare and initialize variables
    int fd = -1;
    int flag = 0;
    int ct = 0;
    int c_flag = 0;
	int i;
    // Check if user passed in enough arguments
    if (argc < 2) {
        print_usage_message(argv[0]);
        return 1;
    }

    // Parse command-line arguments
    for (i = 0; i < strlen(argv[1]); i++) {
        char current_flag = argv[1][i];
        switch (current_flag) {
            case FLAG_T:
                ct++;
                if (!flag) {
                    flag = FLAG_T;
                }
                break;
            case FLAG_V:
                v_flag = 1;
                break;
            case FLAG_F:
                f_flag = 1;
                break;
            case FLAG_C:
                c_flag = 1;
                break;
            default:
                handle_invalid_flag_selection(argv[0], current_flag);
        }
    }   

    // Check if f flag was passed in
    if (!f_flag) {
        fprintf(stderr, "%s: give f option if passing in file\n", argv[0]);
        return 1;
    }else if (c_flag){
        out_file = fopen(argv[2],"wb");
        close(out_file);
        out_file = fopen(argv[2],"ab");
        int i;
        for (i = 3; i<argc; i++){
            manage_file(argv[i]);
        }
        write_block(FORCE);
        write_block(FORCE);
    }
    else {
        // Open the specified file
        fd = open(argv[2], O_RDONLY);
        if (fd == -1) {
            perror("open");
            return 1;
        }

        // Call function to process the archive
        print_archive(fd, argv, argc, flag);
        close(fd);
    }

    return 0;
}

