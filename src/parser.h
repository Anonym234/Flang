typedef char variable_t;
typedef char terminal_t;

typedef struct {
    enum {
        VARIABLE,
        TERMINAL
    } type;
    variable_t variable;
    terminal_t terminal;
} symbol_t;

typedef struct {
    symbol_t * symbols;
    int n;
} sentence_t;

typedef struct {
    variable_t * variables;
    int n;
    sentence_t * transitions;
    int m;
} grammar_t;

grammar_t grammar_parse(FILE *);
