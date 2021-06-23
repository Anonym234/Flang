// this translates a language grammar (contextfree) into a grammar representation

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#include "parser.h"

#define MAX_VARIABLES 69

char ** readRelevantLines(FILE *) ;
char * str_dup(const char *, int, int);
bool dfa(const char *, int *);
char hex(int);

grammar_t grammar_parse(FILE * fp) {
    char ** lines = readRelevantLines(fp);

    char * variables[MAX_VARIABLES+1];
    for (int i = 0; i < MAX_VARIABLES; ++i) {
        variables[i] = NULL;
    }

    for (int i = 0; lines[i] != NULL; ++i) {
        char * line = lines[i];
        printf("\"");
        for (int i = 0; line[i] != '\0'; ++i) {
            printf("%c", line[i]);
        }
        printf("\"\n<");
        int * matching = (int *) malloc(strlen(line) * sizeof(int));
        for (int i = 0; i < strlen(line); ++i) matching[i] = 0;
        bool match = dfa(line, matching);
        for (int i = 0; i < strlen(line); ++i) printf("%c", hex(matching[i]));
        printf(">\t%s\n", match ? "YESS" : "NOO");

        free(line);
        free(matching);
    }

    grammar_t grammar;
    return grammar;
}

char ** readRelevantLines(FILE * fp) {
    char ** arrOfStrings = (char **) malloc(sizeof(char *));
    int n = 0;
    arrOfStrings[n] = NULL;

    char * line;
    while (fscanf(fp, "%m[^\n]\n", &line) != EOF) {
        // if (line[0] == '%' || line[0] == '<') {
            arrOfStrings[n] = line;
            arrOfStrings = (char **) realloc(arrOfStrings, (++n + 1) * sizeof(char *));
            arrOfStrings[n] = NULL;
        // }
    }

    return arrOfStrings;
}

char * str_dup(const char * str, int from, int to) {
    int length = to - from;
    assert(length >= 0);
    char * dup = (char *) malloc((length + 1) * sizeof(char));
    for (int i = 0; i < length; ++i) {
        dup[i] = str[from + i];
        assert(str[from + i] != '\0');
    }
    return dup;
}

char hex(int number) {
    if (number >= 0 && number <= 9) {
        return '0' + number;
    } else if (number >= 10 && number <= 36) {
        return 'a' + number - 10;
    } else {
        return number;
    }
}

bool dfa(const char * str, int * target) {
    struct transition {
        int id;
        char symbol;
        int state;
    } states[] = {
        // start
        {id: 0, symbol: '/', state: 1},
        {id: 0, symbol: '<', state: 2},
        {id: 0, symbol: '\%', state: 10},

        // comment
        {id: 1, symbol: '\0', state: 1},

        // <variable> :=
        {id: 2, symbol: '\0', state: 2},
        {id: 2, symbol: '>', state: 3},
        {id: 3, symbol: ' ', state: 3},
        {id: 3, symbol: ':', state: 5},
        {id: 5, symbol: '=', state: 6},
        {id: 6, symbol: ' ', state: 6},
        {id: 6, symbol: '<', state: 7}, // beginning ot other variable reference
        {id: 6, symbol: '\"', state: 8}, // beginning of terminal symbol
        {id: 6, symbol: '%', state: 9}, // beginning of parser expression

        // <variable>
        {id: 7, symbol: '\0', state: 7},
        {id: 7, symbol: '>', state: 6},

        // "terminal symbols"
        {id: 8, symbol: '\0', state: 8},
        {id: 8, symbol: '"', state: 6},

        // %parser expression
        {id: 9, symbol: '\0', state: 9},
        {id: 9, symbol: ' ', state: 6},

        // %PARSER directives
        {id: 10, symbol: '\0', state: 10},
        {id: 10, symbol: ' ', state: 11},
        {id: 11, symbol: ' ', state: 11},
        {id: 11, symbol: '\0', state: 12},
        {id: 12, symbol: '\0', state: 12},
    };
    int accepting[] = {1, 6, 9, 12};

    int state = 0;

    int i = 0;
    char c;
    while ((c = str[i]) != '\0') {
        bool transitioned = false;
        for (int stateIndex = 0; stateIndex < sizeof(states) / sizeof(struct transition); ++stateIndex) {
            struct transition transition = states[stateIndex];
            if (transition.id != state) continue;
            if (c == transition.symbol) {
                // printf("in state %d, reading symbol '%c' and now transitioning to state %d\n", state, c, transition.state);
                state = transition.state;
                transitioned = true;
                break;
            }
        }
        if (!transitioned) for (int stateIndex = 0; stateIndex < sizeof(states) / sizeof(struct transition); ++stateIndex) {
            struct transition transition = states[stateIndex];
            if (transition.id != state) continue;
            if (transition.symbol == '\0') {
                // printf("in state %d, reading symbol '%c' and now conditionsless transitioning to state %d\n", state, c, transition.state);
                state = transition.state;
                transitioned = true;
                break;
            }
        }
        if (!transitioned) {
            // printf("NO TRANSITION in state %d for symbol '%c'.\n", state, c);
            return false;
        }
        target[i] = state;

        i++;
    }


    for (int i = 0; i < sizeof(accepting) / sizeof(int); ++i) {
        if (state == accepting[i]) {
            return true;
        }
    }
    return false;
}
