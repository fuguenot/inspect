#include "inspect.h"

#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#define USAGE_STR "usage: %s [-h] <file>\n"
#define HELP_STR                                  \
    "inspect: simple tool for looking at files\n" \
    "options:\n"                                  \
    "  -h : prints this help string\n"            \
    "  file : the file to look at\n"

void usage(FILE *out) {
    fprintf(out, USAGE_STR, prog_name);
}

void help() {
    printf(HELP_STR);
}

void process_args(int argc, char *const *argv) {
    prog_name = argv[0];
    int o;
    while ((o = getopt(argc, argv, "h")) != -1) {
        switch (o) {
        case 'h':
            usage(stdout);
            help();
            exit(0);
            break;
        default:
            usage(stderr);
            exit(1);
            break;
        }
    }
    if (optind >= argc) {
        usage(stderr);
        exit(1);
    }
    file = argv[optind];
}

void load_file() {
    FILE *fp = fopen(file, "r");
    if (fp == NULL) {
        perror(prog_name);
        fprintf(stderr, "%s: failed to open file '%s'\n", prog_name, file);
        exit(1);
    }
    struct stat fbuf;
    if (fstat(fileno(fp), &fbuf) == -1) {
        perror(prog_name);
        fprintf(stderr,
                "%s: failed to determine file status for '%s'\n",
                prog_name,
                file);
        exit(1);
    }
    int64_t size = fbuf.st_size;
    buf = calloc(size + 1, sizeof(char));
    if (buf == NULL) {
        perror(prog_name);
        fprintf(stderr,
                "%s: failed to allocate buffer for file '%s'\n",
                prog_name,
                file);
        fclose(fp);
        exit(1);
    }
    if (fread(buf, sizeof(char), size, fp) != size) {
        perror(prog_name);
        fprintf(stderr, "%s: failed to read file '%s'\n", prog_name, file);
        fclose(fp);
        free(buf);
        exit(1);
    }
    fclose(fp);
    buf[size] = '\0';
}
