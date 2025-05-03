#include "inspect.h"

#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define FILENAME_MAX_SIZE 32

int load_file(const char *name, struct buffer_t *buf) {
    FILE *fp = fopen(name, "r");
    if (fp == NULL) return -(E_BUFFER | E_IO);
    struct stat fbuf;
    if (fstat(fileno(fp), &fbuf) == -1) {
        fclose(fp);
        return -(E_BUFFER | E_IO);
    }
    int64_t size = fbuf.st_size;
    char *data = calloc(size + 1, sizeof(char));
    if (data == NULL) {
        fclose(fp);
        return -(E_BUFFER | E_ALLOC);
    }
    if (fread(data, sizeof(char), size, fp) != size) {
        fclose(fp);
        free(data);
        return -(E_BUFFER | E_IO);
    }
    fclose(fp);
    data[size] = '\0';
    buf->nlines = 1;
    for (int64_t i = 0; i < size; i++)
        if (data[i] == '\n') buf->nlines++;
    buf->lines = calloc(buf->nlines, sizeof(char *));
    if (buf->lines == NULL) {
        free(data);
        return -(E_BUFFER | E_ALLOC);
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
            return -(E_BUFFER | E_ALLOC);
        }
        strlcpy(buf->lines[i], start, line_size + 1);
        if (ptr == NULL) break;
        start += line_size + 1;
    }
    free(data);
    return 0;
}

int open_buffer(const char *name) {
    int i = 0;
    for (; i < NBUFS; i++)
        if (bufs[i] == NULL) break;
    if (i == NBUFS) return -(E_BUFFER | E_FULL);
    bufs[i] = malloc(sizeof(struct buffer_t));
    if (bufs[i] == NULL) return -(E_BUFFER | E_ALLOC);
    int r = load_file(name, bufs[i]);
    bufs[i]->filename = calloc(strlen(name) + 1, sizeof(char));
    strlcpy(bufs[i]->filename, name, FILENAME_MAX_SIZE);
    bufs[i]->drow = 0;
    bufs[i]->dcol = 0;
    bufs[i]->row = 0;
    bufs[i]->col = 0;
    return r < 0 ? r : i;
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
