#include "common.h"
#include "crossings.h"
#include "matrix_multiply.h"
#include "crossing_list.h"

#include <stdlib.h>
#include <stdio.h>

#include <gmp.h>

int
main(int argc, char **argv)
{
    struct crossing_list crossings;
    size_t i;
    mpz_t *in_vec, *out_vec, *tmp_vec;

    fprintf(stderr, "Computing crossings with %d pieces.\n", piece_data_count);

    crossing_list_init(&crossings);
    find_all_crossings(&crossings);

    fprintf(stderr, "Crossings: %lu\n", crossings.count);

    in_vec = malloc(crossings.count * sizeof (*in_vec));
    out_vec = malloc(crossings.count * sizeof (*out_vec));
    for (i = 0; i < crossings.count; ++i) {
        mpz_init(in_vec[i]);
        mpz_init(out_vec[i]);
    }
    mpz_set_ui(in_vec[0], 1);

    for (i = 0; i < BOARD_WIDTH; ++i) {
        adjacency_matrix_multiply(&crossings, in_vec, out_vec);

        fprintf(stderr, "Tilings for a %dx%d board: ", (int)i + 1,
                (int)BOARD_HEIGHT);
        mpz_out_str(stderr, 10, out_vec[0]);
        fprintf(stderr, "\n");

        tmp_vec = in_vec;
        in_vec = out_vec;
        out_vec = tmp_vec;
    }

    for (i = 0; i < crossings.count; ++i) {
        mpz_clear(in_vec[i]);
        mpz_clear(out_vec[i]);
    }

    crossing_list_destroy(&crossings);

    return 0;
}
