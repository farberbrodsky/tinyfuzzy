#include "tinyfuzzy.h"
#include <string.h>

struct TinyFuzzy_Result TinyFuzzy_search(char *haystack, char *needle) {
    struct TinyFuzzy_Result res;

    size_t haystack_len = strlen(haystack);
    size_t needle_len = strlen(needle);

    // construct the matrix
    ssize_t *scores = malloc((haystack_len + 1) * (needle_len + 1) * sizeof(ssize_t));
    // go by scores[i + (needle_len + 1) * j]
    memset(scores, 0, needle_len + 1);  // set first row to 0
    for (size_t j = 1; j < haystack_len + 1; j++) {  // set first column to 0
        scores[(needle_len + 1) * j] = 0;
    }

    for (size_t j = 1; j < haystack_len + 1; j++) {
        for (size_t i = 1; i < needle_len + 1; i++) {
            // calculate score for scores[i + (needle_len + 1) * j]
            ssize_t best_option = 0;

            // replace/same option, penalty of 3 or bonus of 3
            ssize_t some_option = scores[(i - 1) + (needle_len + 1) * (j - 1)];
            if (haystack[j - 1] == needle[i - 1]) {
                some_option += 3;
            } else {
                some_option -= 3;
            }
            if (some_option > best_option) {
                best_option  = some_option;
            }

            // insert/omit options, penalty of 2
            some_option = scores[i - 1 + (needle_len + 1) * j] - 2;
            if (some_option > best_option) {
                best_option  = some_option;
            }
            some_option = scores[i + (needle_len + 1) * (j - 1)] - 2;
            if (some_option > best_option) {
                best_option  = some_option;
            }

            scores[i + (needle_len + 1) * j] = best_option;
        }
    }

    size_t max_i = 0;
    size_t max_j = 0;
    size_t max_option = 0;

    for (size_t j = 0; j < haystack_len + 1; j++) {
        for (size_t i = 0; i < needle_len + 1; i++) {
            size_t option = scores[i + (needle_len + 1) * j];
            if (option > max_option) {
                max_option = option;
                max_i = i;
                max_j = j;
            }
        }
    }

    printf("Best is %zu\n", max_option);

    // allocate ops based on theoretical maximum size
    // we will memmove it later
    size_t max_i_j = (max_j > max_i) ? max_j : max_i;
    size_t ops_len = 1;
    res.ops = malloc(sizeof(enum TinyFuzzy_Op) * max_i_j);
    res.ops[--max_i_j] = TinyFuzzy_Op_END;

    // backtrack
    while (max_i > 0 && max_j > 0) {
        size_t target = scores[max_i + (needle_len + 1) * max_j];
        if (target == 0) {
            res.match_start = max_j - 1;
            break;
        }

        // replace/same option, penalty of 3 or bonus of 3
        ssize_t some_option = scores[(max_i - 1) + (needle_len + 1) * (max_j - 1)];
        if (haystack[max_j - 1] == needle[max_i - 1]) {
            some_option += 3;
        } else {
            some_option -= 3;
        }
        if (some_option == target) {
            max_i -= 1; max_j -= 1;
            ops_len++;
            res.ops[--max_i_j] = (haystack[max_j] == needle[max_i]) ?
                TinyFuzzy_Op_KEEP : TinyFuzzy_Op_REPLACE;
            continue;
        }

        some_option = scores[max_i - 1 + (needle_len + 1) * max_j] - 2;
        if (some_option == target) {
            ops_len++;
            res.ops[--max_i_j] = TinyFuzzy_Op_OMIT;
            max_i -= 1; continue;
        }
        some_option = scores[max_i + (needle_len + 1) * (max_j - 1)] - 2;
        if (some_option == target) {
            ops_len++;
            res.ops[--max_i_j] = TinyFuzzy_Op_INSERT;
            max_j -= 1; continue;
        }
    }
    memmove(res.ops, &res.ops[max_i_j], ops_len * sizeof(enum TinyFuzzy_Op));

    return res;
}
