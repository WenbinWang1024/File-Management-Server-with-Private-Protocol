#ifndef FILE_INFO_H
#define FILE_INFO_H

#include "includings.h"

typedef struct {
    char f_type;
    size_t f_size;
    char f_name[MAX_NAME_LEN];
} file_t, * pFile_t;

void get_file_perm(mode_t mode, char * ret);

void get_file_type(mode_t mode, char * ret);

#endif // FILE_INFO_H
