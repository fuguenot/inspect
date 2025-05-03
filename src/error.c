#include "error.h"

#include <stdlib.h>
#include <string.h>

#include "inspect.h"

#define MAX_ERR_SIZE 64

struct error_t error = {.code = E_OK, .details = NULL};
char *errmsg = NULL;

#define ERRMSG(code, str) case (code): strlcpy(errmsg, (str), MAX_ERR_SIZE + 1); break
#define ERRMSG_DETAILS(code, fmt) case (code): snprintf(errmsg, MAX_ERR_SIZE + 1, (fmt), error.details); break

char *get_errmsg() {
    if (errmsg == NULL) errmsg = calloc(MAX_ERR_SIZE + 1, sizeof(char));
    switch (error.code) {
        ERRMSG(E_OK, "");
        ERRMSG_DETAILS(E_OPEN_FILE, "failed to open file '%s'");
        ERRMSG_DETAILS(E_GET_FILE_SIZE, "failed to get file size for '%s'");
        ERRMSG_DETAILS(E_ALLOC_FILE_DATA, "failed to allocate space for contents of '%s'");
        ERRMSG_DETAILS(E_READ_FILE_DATA, "failed to read contents of '%s'");
        ERRMSG_DETAILS(E_ALLOC_BUF_LINES, "failed to allocate space for storing lines of '%s'");
        ERRMSG_DETAILS(E_ALLOC_BUF_LINE, "failed to allocate buffer for a line of '%s'");
        ERRMSG_DETAILS(E_ALLOC_BUF, "failed to allocate buffer '%s'");
ERRMSG(E_BUFS_FULL, "no buffers available");
        ERRMSG_DETAILS(E_ALLOC_FULLPATH, "failed to allocate space for the full path of '%s'");
        ERRMSG_DETAILS(E_FILE_NOT_FOUND, "file not found: '%s'");
        ERRMSG_DETAILS(E_ALLOC_BUF_FILENAME, "failed to allocate space for the filename of buffer '%s'");
        ERRMSG_DETAILS(E_ALLOC_HELP_PATH, "failed to allocate space for the help path including '%s'");
    }
    return errmsg;
}

void print_errmsg() {
    if (error.code != E_OK)
        printf("%s: %s\n", prog_name, get_errmsg());
}

void free_errmsg() {
    if (errmsg != NULL) free(errmsg);
}
