#include "inspect.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "error.h"

#define USAGE_STR "usage: %s [-h] <file1> [file2] [file3]...\n"
#define HELP_STR                                  \
    "inspect: simple tool for looking at files\n" \
    "options:\n"                                  \
    "  -h : prints this help string\n"            \
    "  files : the files to look at\n"

void usage(FILE *out) {
    fprintf(out, USAGE_STR, prog_name);
}

void help() {
    printf(HELP_STR);
}

int process_args(int argc, char *const *argv) {
    prog_name = argv[0];
    const char *dir = getenv("INSPECT_DIR");
    int64_t hpl = strlen(dir) + strlen("/help");
    help_path = calloc(hpl + 1, sizeof(char));
    if (help_path == NULL) {
        error.code = E_ALLOC_HELP_PATH;
        error.details = dir;
        return RET_ERR;
    }
    strlcat(help_path, dir, hpl + 1);
    strlcat(help_path, "/help", hpl + 1);
    int o;
    while ((o = getopt(argc, argv, "hR")) != -1) {
        switch (o) {
        case 'h':
            usage(stdout);
            help();
            exit(0);
            break;
        case 'R':
            glob_readonly = true;
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
    return optind;
}
