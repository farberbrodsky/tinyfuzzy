#include <stdio.h>
#include "tinyfuzzy.h"

int main(int argc, char **argv) {
    // char *haystack = "ABCDEFGHIJ";
    // char   *needle = "AAACCDFFGHHIJ";
    if (argc < 3) {
        printf("No!\n");
        return 1;
    }
    char *haystack = argv[1];
    char   *needle = argv[2];
    struct TinyFuzzy_Result r = TinyFuzzy_search(haystack, needle);
    enum TinyFuzzy_Op *opp = r.ops;

    printf("Start at index %zu, %zu\n", r.haystack_match_start, r.needle_match_start);
    while (*opp != TinyFuzzy_Op_END) {
        switch (*opp) {
            case TinyFuzzy_Op_KEEP:
                printf("   Keep %c  %c\n", needle[r.needle_match_start++], haystack[r.haystack_match_start++]);
                break;
            case TinyFuzzy_Op_REPLACE:
                printf("Replace %c->%c\n", needle[r.needle_match_start++], haystack[r.haystack_match_start++]);
                break;
            case TinyFuzzy_Op_OMIT:
                printf("   Omit %c\n", needle[r.needle_match_start++]);
                break;
            case TinyFuzzy_Op_INSERT:
                printf(" Insert %c\n", needle[r.needle_match_start++]);
                break;
            default:
                break;
        }
        ++opp;
    }
    return 0;
}
