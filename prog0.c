#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>
// Union to store 4 bytes as an array of bytes, an unsigned, signed, or float number
union value{
    unsigned uval;
    int sval;
    float fval;
    unsigned char bytes[4];
};

// reads 8 hex characters from string input and stores it in the union v
// returns -1 if the hexadecimal number is invalid, 0 otherwise
int read_hex(union value *v, char *input);

// converts the ASCII hex character c to binary
// returns the hex value of c if c is a valid hex digit, -1 otherwise
char hexDigit(char c);

// returns true if x has any even bit equal to 1, 0 otherwise
int any_even_one(unsigned x);
 
// returns a mask indicating the position of the left most one in x
int leftmost_one(unsigned x);

// returns x shifted n positions to the left with the n most significant bits of x 
// inserted at the right of x
unsigned rotate_left(unsigned x, int n);

// returns x shifted n positions to the right with the n least significant bits of x 
// inserted at the left of x
unsigned rotate_right(unsigned x, int n);

// returns x+y if no overflow occurs
// returns TMAX if a positive overflow occurs
// returns TMIN if a negative overflow occurs
unsigned saturating_add(int x, int y);

// multiplies the binary representation of a float number f by 2
unsigned float_twice(unsigned f);
 
// divides the binary representation of a float number f by 2
unsigned float_half(unsigned f);

int main(int argc, char** argv){
    if(argc != 3 && argc != 4){
        printf("Invalid number of arguments\n");
        exit(0);
    }

    union value value = {0};
    union value value2 = {0};

    if (strcmp(argv[1], "even") == 0){
            int valid = read_hex(&value, argv[2]);
            if (valid==-1){
                printf("Invalid hex value\n");
            }
            else{
                int result = any_even_one(value.uval);
                if (result == 1){
                    printf("True\n");
                }
                else{
                    printf("False\n");
                }
            }
    }
    else if (strcmp(argv[1], "left") == 0){
        int valid = read_hex(&value, argv[2]);
        if (valid==-1){
            printf("Invalid hex value\n");
        }
        else{
            int result = leftmost_one(value.uval);
            printf("%08x\n", result);
        }
    }
    else if (strcmp(argv[1], "lrotate")==0){
        int valid = read_hex(&value, argv[2]);
        if (valid==-1){
            printf("Invalid hex value\n");
        }
        else{
            int shift = atoi(argv[3]);
            if (shift < 0 || shift >= 32){
                printf("Invalid number of shift positions\n");
            }
            else{
                unsigned result = rotate_left(value.uval, shift);
                printf("%08x\n", result);
            }
        }
    }
    else if (strcmp(argv[1], "rrotate")==0){
        int valid = read_hex(&value, argv[2]);
        if (valid==-1){
            printf("Invalid hex value\n");
        }
        else{
            int shift = atoi(argv[3]);
            if (shift < 0 || shift >= 32){
                printf("Invalid number of shift positions\n");
            }
            else{
                unsigned result = rotate_right(value.uval, shift);
                printf("%08x\n", result);
            }
        }
    }
    else if (strcmp(argv[1], "saturate")==0){
        int valid1 = read_hex(&value, argv[2]);
        int valid2 = read_hex(&value2, argv[3]);
        if ((valid1==-1) || (valid2==-1)){
            printf("Invalid hex value\n");
        }
        else{
            unsigned sum = saturating_add(value.sval, value2.sval);
            printf("%08x %d\n", sum, (int)sum);
        }
    }
    else if (strcmp(argv[1], "twice")==0){
        int valid = read_hex(&value, argv[2]);
        if (valid==-1){
            printf("Invalid hex value\n");
        }
        else{
            unsigned result = float_twice(value.uval);
            value.uval = result;
            printf("%08x %e\n", value.uval, value.fval);
        }
    }
    else if (strcmp(argv[1], "half")==0){
        int valid = read_hex(&value, argv[2]);
        if (valid==-1){
            printf("Invalid hex value\n");
        }
        else{
            unsigned result = float_half(value.uval);
            value.uval = result;
            printf("%08x %e\n", value.uval, value.fval);
        }
    }
    else{
        printf("Invalid operation\n");
    }

    return 0;
}

int any_even_one(unsigned x){
    return (x & 0x55555555u) != 0;
}

int leftmost_one(unsigned x) {
    unsigned mask = 0x80000000u;
    while ((x & mask) == 0u && mask != 0u) {
        mask >>= 1;
    }
    return (int)mask;
}

unsigned rotate_left(unsigned x, int n) {
    if (n < 0) {
        n = 32 + (n % 32);
    }
    n %= 32;
    if (n == 0) {
        return x;
    }
    return (x << n) | (x >> (32 - n));
}

unsigned rotate_right(unsigned x, int n) {
    if (n < 0) {
        n = 32 + (n % 32);
    }
    n %= 32;
    if (n == 0) {
        return x;
    }
    return (x >> n) | (x << (32 - n));
}

unsigned saturating_add(int x, int y) {
    long long sum = (long long)x + y;
    if (sum > INT_MAX) {
        return (unsigned)INT_MAX;
    }
    if (sum < INT_MIN) {
        return (unsigned)INT_MIN;
    }
    return (unsigned)(int)sum;
}

unsigned float_twice(unsigned f) {
    union {
        unsigned u;
        float f;
    } v;
    v.u = f;

    if (isnan(v.f) || isinf(v.f)) {
        return f;
    }

    v.f = v.f * 2.0f;
    return v.u;
}

unsigned float_half(unsigned f) {
    union {
        unsigned u;
        float f;
    } v;
    v.u = f;

    if (isnan(v.f) || isinf(v.f)) {
        return f;
    }

    v.f = v.f * 0.5f;
    return v.u;
}

char hexDigit(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    } else if (c >= 'A' && c <= 'F') {
        return c - 'A' + 10;
    } else if (c >= 'a' && c <= 'f') {
        return c - 'a' + 10;
    } else {
        return -1;
    }
}

int read_hex(union value *v, char *input) {
    if (strlen(input) != 8) {
        return -1;
    }

    memset(v, 0, sizeof(*v));

    for (int i = 0; i < 4; i++) {
        char high = hexDigit(input[2 * i]);
        char low = hexDigit(input[2 * i + 1]);
        if (high == -1 || low == -1) {
            return -1;
        }
        v->bytes[3 - i] = (unsigned char)((high << 4) | low);
    }

    return 0;
}
