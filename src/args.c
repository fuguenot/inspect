#include "inspect.h"

#include <stdlib.h>
#include <string.h>
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

char *process_args(int argc, char *const *argv) {
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
    return argv[optind];
}
