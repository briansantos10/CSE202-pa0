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
int saturating_add(int x, int y);

// multiplies the binary representation of a float number f by 2
unsigned float_twice(unsigned f);
 
// divides the binary representation of a float number f by 2
unsigned float_half(unsigned f);

int main(int argc, char** argv){
    if(argc != 3 && argc != 4){
        printf("Invalid number of arguments");
        exit(0);
    }

    union value value;
    union value value2;

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
            unsigned result = rotate_left(value.uval, shift);
            if (shift>32){
                printf("Invalid number of shift positions\n");
            }
            else{
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
            if (shift>=32){
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
            int sum = saturating_add(value.sval, value2.sval);
            printf("%08x %i\n",sum, sum);
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
//  Solutions

int any_even_one(unsigned x){
    return (x & 0x55555555) != 0;
}

int leftmost_one(unsigned x) {
    int count = 0;
    while (x != 0) {
        x = x >> 1;
        count++;    
    }
    if (count == 0) {
        return 0; // No bits are set
    }
    return 1 << (count - 1); 
}

unsigned rotate_right(unsigned x, int n) {
    unsigned int y = x & ( ~ (0xFFFFFFFF << n) );
    return (x << n) | (y >> (32 - n));
}

unsigned rotate_left(unsigned x, int n) {
    unsigned int y = x >> (32 - n);
    return (x << n) | y;
}

unsigned saturating_add(int x, int y) {
    int sum = x + y;
    if (x > 0 && y > 0 && sum < 0) {
        return INT_MAX; 
    } else if (x < 0 && y < 0 && sum > 0) {
        return INT_MIN;
    }

    return (unsigned) sum;
}

unsigned float_twice(unsigned f) {
    unsigned exp = (f >> 23) & 0xFF;
    if (exp == 0xff) {
        return f;
    }

    return f + (1 << 23);
}   

unsigned float_half(unsigned f) {
    unsigned exp = (f >> 23) & 0xFF;
    if (exp == 0xff) {
        return f;
    }

    return f - (1 << 23);
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

    for (int i = 0; i < 8; i++) {
        char digit = hexDigit(input[i]);
        if (digit == -1) {
            return -1;
        }
        v->bytes[i / 2] |= (digit << ((1 - (i % 2)) * 4));
    }

    return 0;
}
