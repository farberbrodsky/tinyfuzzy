#include <stdio.h>
#include "tinyfuzzy.h"

int main() {
    printf("Hello!\n");
    char *haystack = "AAAAATGTTACGG";
    char   *needle = "GGTTGACTA";
    struct TinyFuzzy_Result r = TinyFuzzy_search(haystack, needle);
    enum TinyFuzzy_Op *opp = r.ops;
    printf("Start at index %zu\n", r.match_start);
    while (*opp != TinyFuzzy_Op_END) {
        switch (*opp) {
            case TinyFuzzy_Op_KEEP:
                puts("Keep");
                break;
            case TinyFuzzy_Op_REPLACE:
                puts("Replace");
                break;
            case TinyFuzzy_Op_OMIT:
                puts("Omit");
                break;
            case TinyFuzzy_Op_INSERT:
                puts("Insert");
                break;
            default:
                puts("WTF!");
                printf("*opp %d\n", *opp);
                break;
        }
        ++opp;
    }
}
