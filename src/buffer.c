#include "inspect.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define FILENAME_MAX_SIZE 32

int load_file(const char *name, struct buffer_t *buffer) {
    FILE *fp = fopen(name, "r");
    if (fp == NULL) return -(E_BUFFER | E_IO);
    struct stat fbuf;
    if (fstat(fileno(fp), &fbuf) == -1) {
        fclose(fp);
        return -(E_BUFFER | E_IO);
    }
    int64_t size = fbuf.st_size;
    char *buf = calloc(size + 1, sizeof(char));
    if (buf == NULL) {
        fclose(fp);
        return -(E_BUFFER | E_ALLOC);
    }
    if (fread(buf, sizeof(char), size, fp) != size) {
        fclose(fp);
        free(buf);
        return -(E_BUFFER | E_IO);
    }
    fclose(fp);
    buf[size] = '\0';
    buffer->clines = 1;
    for (int64_t i = 0; i < size; i++)
        if (buf[i] == '\n') buffer->clines++;
    buffer->lines = calloc(buffer->clines, sizeof(char *));
    if (buffer->lines == NULL) {
        free(buf);
        return -(E_BUFFER | E_ALLOC);
    }
    char *ptr = NULL;
    char *start = buf;
    int64_t line_size;
    for (int64_t i = 0; i < buffer->clines; i++) {
        ptr = strstr(start, "\n");
        line_size = ptr == NULL ? size - (start - buf) : ptr - start;
        buffer->lines[i] = calloc(line_size + 1, sizeof(char));
        if (buffer->lines[i] == NULL) {
            free(buf);
            return -(E_BUFFER | E_ALLOC);
        }
        strlcpy(buffer->lines[i], start, line_size + 1);
        if (ptr == NULL) break;
        start += line_size + 1;
    }
    free(buf);
    return 0;
}

int open_buffer(const char *name) {
    int i = 0;
    for (; i < NBUFS; i++)
        if (buffers[i] == NULL) break;
    if (i == NBUFS) return -(E_BUFFER | E_FULL);
    buffers[i] = malloc(sizeof(struct buffer_t));
    if (buffers[i] == NULL) return -(E_BUFFER | E_ALLOC);
    int r = load_file(name, buffers[i]);
    buffers[i]->filename = calloc(strlen(name) + 1, sizeof(char));
    strlcpy(buffers[i]->filename, name, FILENAME_MAX_SIZE);
    return r < 0 ? r : i;
}

void close_buffer(int idx) {
    if (buffers[idx] == NULL) return;
    for (int64_t i = 0; i < buffers[idx]->clines; i++)
        free(buffers[idx]->lines[i]);
    free(buffers[idx]->lines);
    free(buffers[idx]->filename);
    free(buffers[idx]);
    buffers[idx] = NULL;
}

void close_all_buffers() {
    for (int i = 0; i < NBUFS; i++)
        close_buffer(i);
}
