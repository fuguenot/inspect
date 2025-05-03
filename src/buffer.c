#include "inspect.h"

#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "error.h"

#define FILENAME_MAX_SIZE 32

int open_buffer(const char *name, bool readonly, const char *path) {
    int i = 0;
    for (; i < NBUFS; i++)
        if (bufs[i] == NULL) break;
    if (i == NBUFS) {
        error.code = E_BUFS_FULL;
        return RET_ERR;
    }
    bufs[i] = malloc(sizeof(struct buffer_t));
    if (bufs[i] == NULL) {
        error.code = E_ALLOC_BUF;
        error.details = name;
        return RET_ERR;
    }
    int r = RET_NUL;
    if (path != NULL) {
        int64_t fpl = strlen(path) + strlen(name) + 1;
        char *fullpath = calloc(fpl + 1, sizeof(char));
        if (fullpath == NULL) {
            free(bufs[i]);
            bufs[i] = NULL;
            error.code = E_ALLOC_FULLPATH;
            error.details = name;
            return RET_ERR;
        }
        strlcat(fullpath, path, fpl + 1);
        strlcat(fullpath, "/", fpl + 1);
        strlcat(fullpath, name, fpl + 1);
        if (access(fullpath, F_OK) == 0) r = load_file(fullpath, bufs[i]);
        free(fullpath);
        fullpath = NULL;
    }
    if (r == RET_NUL && access(name, F_OK) == 0) r = load_file(name, bufs[i]);
    if (r == RET_NUL) {
        free(bufs[i]);
        bufs[i] = NULL;
        error.code = E_FILE_NOT_FOUND;
        error.details = name;
        return RET_ERR;
    }
    if (r == RET_ERR) {
        free(bufs[i]);
        bufs[i] = NULL;
        return RET_ERR;
    }
    bufs[i]->filename = calloc(strlen(name) + 1, sizeof(char));
    if (bufs[i]->filename == NULL) {
        free(bufs[i]);
        bufs[i] = NULL;
        error.code = E_ALLOC_BUF_FILENAME;
        error.details = name;
        return RET_ERR;
    }
    strlcpy(bufs[i]->filename, name, FILENAME_MAX_SIZE);
    bufs[i]->drow = 0;
    bufs[i]->dcol = 0;
    bufs[i]->row = 0;
    bufs[i]->col = 0;
    return i;
}

void close_buffer(int idx) {
    if (bufs[idx] == NULL) return;
    for (int64_t i = 0; i < bufs[idx]->nlines; i++)
        free(bufs[idx]->lines[i]);
    free(bufs[idx]->lines);
    free(bufs[idx]->filename);
    free(bufs[idx]);
    bufs[idx] = NULL;
}

void close_all_buffers() {
    for (int i = 0; i < NBUFS; i++)
        close_buffer(i);
}

int buffer_count() {
    int n = 0;
    for (int i = 0; i < NBUFS; i++)
        if (bufs[i] != NULL) n++;
    return n;
}
