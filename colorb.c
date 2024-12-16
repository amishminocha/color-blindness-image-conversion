#include "bmp.h"
#include "io.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define OPTIONS "i:o:h"

int main(int argc, char *argv[]) {
    int opt = 0;
    int contains_i = 0;
    int contains_o = 0;
    int contains_h = 0;
    char *input_filename;
    char *output_filename;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'h': contains_h = 1; break;
        case 'i':
            contains_i = 1;
            input_filename = optarg;
            break;
        case 'o':
            contains_o = 1;
            output_filename = optarg;
            break;
        case '?': exit(0);
        }
    }

    if (contains_h) {
        printf("Usage:\t./colorb -i infile -o outfile\n\t./colorb -h\n");
    } else {
        if (contains_i == 0 || contains_o == 0) {
            printf("Error with inputting files\n");
            exit(0);
        }
        Buffer *buf = read_open(input_filename);
        BMP *bmp = bmp_create(buf);
        bmp_reduce_palette(bmp);
        read_close(&buf);
        Buffer *buf1 = write_open(output_filename);
        bmp_write(bmp, buf1);
        write_close(&buf1);
        bmp_free(&bmp);
    }

    return 0;
}
