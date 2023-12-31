#include <stdlib.h>
#include <time.h>

#include "uuid.h"

#define RAND_LENGTH 4



const unsigned char hex[16]={'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                             'a', 'b', 'c', 'd', 'e', 'f'};

void uuid_generate_random(uuid_t out) {
    int i, j, rnd;
    static int seeded = 0;

    if (!seeded) {
        srand(time(NULL));
        seeded=1;
    }

    for (i=0;i<(16/RAND_LENGTH);i++) {
        rnd = rand();

        for (j=0;j<RAND_LENGTH;j++) {
            out[i*RAND_LENGTH+j] = (0xff & rnd >> (8*j));
        }
    }

    // set the version to 4
    out[6] = (out[6] & 0x0f) | 0x40;

    // set the variant to 1 (a)
    out[8] = (out[8] & 0x0f) | 0xa0;
}

int _hex2dec(char c) {
    int i;

    for (i=0;i<16;i++) {
        if (hex[i] == c)
            return i;
    }

    return -1;
}

void uuid_parse(const char *in, uuid_t uuid) {
    int i, j;

    i=j=0;

    do {
        switch (in[i]) {
            case '-':
                break;
            default:
                uuid[j++]=(_hex2dec(in[i++]) << 4) | _hex2dec(in[i]);
        }

        i++;
    } while (j < 16 && i < 36);
}

void uuid_unparse(const uuid_t uuid, char *out) {
    int i, j;

    i=j=0;

    do {
        switch (j) {
            case 4:
            case 6:
            case 8:
            case 10:
                out[i++]='-';
        }

        out[i++]=hex[(uuid[j] >> 4)];
        out[i++]=hex[(0xf & uuid[j])];

        j++;
    } while (j < 16);

    out[36]=0;
}

void uuid_copy(uuid_t dst, const uuid_t src) {
    int i;
    for (i=0;i<sizeof(uuid_t);i++) {
        dst[i]=src[i];
    }
}
