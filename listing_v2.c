#include "mytar.h"
#include "listing.h"
#include "printarchive.h"
#include <string.h>

void list_archives(char *file_name, struct Header *header, int verbose_flag) {
    time_t m_time;
    m_time = strtol(header->mtime, NULL, 8);

    int file_size;
    file_size = strtol(header->size, NULL, 8);


    if (!(verbose_flag)){
        printf("%s\n", file_name);
    } else {
        char mode[10];
        get_file_mode(mode, header->typeflag, header->mode);

        char owner_name[17];
        get_owner_name(owner_name, header->uname, header->gname);

        char mtime_buffer[17];
        get_modification_time(mtime_buffer, mtime);

        printf("%10s %-17s %8i %16s %s\n", mode, owner_name, octal_to_string(header->size), mtime_buffer, file_name);
    }
}

void get_file_mode(char *mode, char typeflag, int permissions) {
    switch (typeflag) {
        case REGULAR_FILE:
            mode[0] = '-';
            break;
        case SYM_LINK:
            mode[0] = '1';
            break;
        case DIRECTORY:
            mode[0] = 'd';
            break;
        default:
            mode[0] = '?';
            break;
    }

    char *perm_chars = "rwxrwxrwx";
    for (int i = 0; i < 9; i++) {
        if (permissions & (1 << (8-i))) {
            mode[i+1] = perm_chars[i];
        } else {
            mode[i+1] = '-';
        }
    }
    mode[10] = '\0';
}

void get_owner_name(char *owner_name, char *uname, char *gname) {
    if (strlen(uname) <= OWNER_NAME_LENGTH) {
        strcpy(owner_name, uname);
        if (strlen(gname) > 0) {
            strcat(owner_name, "/");
            if (strlen(uname) + strlen(gname) < OWNER_NAME_LENGTH) {
                strcat(owner_name, gname);
            } else {
                strncat(owner_name, gname, OWNER_NAME_LENGTH - strlen(uname) - 1);
            }
        }
    } else {
        strncpy(owner_name, uname, OWNER_NAME_LENGTH);
        owner_name[OWNER_NAME_LENGTH - 1] = '\0';
    }
}

void get_modification_time(char *buffer, time_t mtime) {
    struct tm *time = localtime(&m_time);
    strftime(buffer, 17, "%Y-%m-%d %H:%M", time);
}

int print_archive(int argc, char **argv, int verbose_flag) {
    FILE *archive = fopen(argv[2], "rb");
    struct Header header;
    int result = 0;

    while (read_header(archive, &header) != 0) {
        char file_name[NAME_SIZE_BYTES + PREFIX_SIZE_BYTES + 2];
        create_name(file_name, header.prefix, header.name);

        if (argc < 4) {
            list_archives(file_name, &header, verbose_flag);
        } else {
            for (int i = 3; i < argc; i++) {
                if (check_file(file_name, argv[i]) == 0) {
                    list_archives(file_name, &header, verbose_flag);
                    break;
                }
            }
        }
    }

    fclose(archive);
    return result;
}