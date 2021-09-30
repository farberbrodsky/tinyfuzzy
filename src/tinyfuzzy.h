#include <stdlib.h>
#include <stdio.h>

struct TinyFuzzy_Result {
    size_t match_start;

    enum TinyFuzzy_Op {
        TinyFuzzy_Op_END = 0,
        TinyFuzzy_Op_KEEP,
        TinyFuzzy_Op_REPLACE,
        TinyFuzzy_Op_OMIT,
        TinyFuzzy_Op_INSERT
    } *ops;
};

struct TinyFuzzy_Result TinyFuzzy_search(char *haystack, char *needle);
