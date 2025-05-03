#pragma once

#include <stdint.h>

#define RET_OK  0
#define RET_ERR -1
#define RET_NUL -2

enum error_type_e : uint8_t {
    E_OK = 0,

    E_OPEN_FILE,
    E_GET_FILE_SIZE,
    E_ALLOC_FILE_DATA,
    E_READ_FILE_DATA,
    E_ALLOC_BUF_LINES,
    E_ALLOC_BUF_LINE,
    E_ALLOC_BUF,

    E_BUFS_FULL,
    E_ALLOC_FULLPATH,
    E_FILE_NOT_FOUND,
    E_ALLOC_BUF_FILENAME,

    E_ALLOC_HELP_PATH,

    E_READONLY,
};

struct error_t {
    enum error_type_e code;
    const char *details;
};

extern struct error_t error;

char *get_errmsg();
void print_errmsg();
void free_errmsg();
