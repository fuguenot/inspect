#include "inspect.h"

#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "error.h"

int load_file(const char *name, struct buffer_t *buf) {
    FILE *fp = fopen(name, "r");
    if (fp == NULL) {
        error.code = E_OPEN_FILE;
        error.details = name;
        return RET_ERR;
    }
    struct stat fbuf;
    if (fstat(fileno(fp), &fbuf) == -1) {
        fclose(fp);
        error.code = E_GET_FILE_SIZE;
        error.details = name;
        return RET_ERR;
    }
    int64_t size = fbuf.st_size;
    char *data = calloc(size + 1, sizeof(char));
    if (data == NULL) {
        fclose(fp);
        error.code = E_ALLOC_FILE_DATA;
        error.details = name;
        return RET_ERR;
    }
    if (fread(data, sizeof(char), size, fp) != size) {
        fclose(fp);
        free(data);
        error.code = E_READ_FILE_DATA;
        error.details = name;
        return RET_ERR;
    }
    fclose(fp);
    data[size] = '\0';
    buf->nlines = 1;
    for (int64_t i = 0; i < size; i++)
        if (data[i] == '\n') buf->nlines++;
    buf->lines = calloc(buf->nlines, sizeof(char *));
    if (buf->lines == NULL) {
        free(data);
        error.code = E_ALLOC_BUF_LINES;
        error.details = name;
        return RET_ERR;
    }
    char *ptr = NULL;
    char *start = data;
    int64_t line_size;
    for (int64_t i = 0; i < buf->nlines; i++) {
        ptr = strstr(start, "\n");
        line_size = ptr == NULL ? size - (start - data) : ptr - start;
        buf->lines[i] = calloc(line_size + 1, sizeof(char));
        if (buf->lines[i] == NULL) {
            free(data);
            error.code = E_ALLOC_BUF_LINE;
            error.details = name;
            return RET_ERR;
        }
        strlcpy(buf->lines[i], start, line_size + 1);
        if (ptr == NULL) break;
        start += line_size + 1;
    }
    free(data);
    return RET_OK;
}
