#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "parser.h"

int main(int argc, char ** argv) {
    const char * filename = (argc >= 2) ? argv[1] : "grammar.grm";
    printf("opening file %s\n", filename);
    FILE * fp = fopen(filename, "r");
    assert(fp);

    grammar_t grammar = grammar_parse(fp);

    fclose(fp);
    fp = NULL;
}
