#ifndef __UTIL__
#define __UTIL__

#include "stdio.h"
#include "x86intrin.h"
#include <string>

typedef union
{
    __m128i int128;

    unsigned char m128_u8[16];
    signed char m128_i8[16];

    unsigned short m128_u16[8];
    signed short m128_i16[8];

    unsigned int m128_u32[4];
    signed int m128_i32[4];

    unsigned long m128_u64[2];
    signed long m128_i64[2];

} intVec;

typedef union
{
    __m128 float128;

    float m128_sp[4];
} floatVec;

void print_names()
{
    printf("Mohsen Fayyaz - Mahdi Hadiloo\n");
    printf("  810196650   -   810196587 \n");
}

template <typename IntType>
void print_int_vector_by_type(IntType var, int n, bool is_signed = false, bool is_long = false)
{
    const char* print_type;
    if(is_long)

        print_type = is_signed ? "%ld" : "%lX";
    else
        print_type = is_signed ? "%d" : "%X";
    
    printf("[");
    for (int i = n; i > 0; i--)
    {
        printf(print_type, var[i]);
        printf(", ");
    }
    printf(print_type, var[0]);
    printf("]\n");
}

void print_int_vector(__m128i a, unsigned char type)
{
    intVec tmp;
    tmp.int128 = a;
    switch (type)
    {
    case '1': // ‫‪16‬‬ ‫‪unsigned‬‬ ‫‪byte‬‬
        print_int_vector_by_type<unsigned char[16]>(tmp.m128_u8, 15);
        break;
    case '2':
        print_int_vector_by_type<signed char[16]>(tmp.m128_i8, 15, true);
        break;
    case '3':
        print_int_vector_by_type<unsigned short[8]>(tmp.m128_u16, 7);
        break;
    case '4':
        print_int_vector_by_type<signed short[8]>(tmp.m128_i16, 7, true);
        break;
    case '5':
        print_int_vector_by_type<unsigned int[4]>(tmp.m128_u32, 3);
        break;
    case '6':
        print_int_vector_by_type<signed int[4]>(tmp.m128_i32, 3, true);
        break;
    case '7':
        print_int_vector_by_type<unsigned long[2]>(tmp.m128_u64, 1, false, true);
        break;
    case '8':
        print_int_vector_by_type<signed long[2]>(tmp.m128_i64, 1, true, true);
        break;

    default:
        break;
    }
}

void print_int_test(__m128i a)
{
    printf("Unsigned byte:        ");
    print_int_vector(a, '1');
    printf("Signed byte:          ");
    print_int_vector(a, '2');
    printf("Unsigned word:        ");
    print_int_vector(a, '3');
    printf("Signed word:          ");
    print_int_vector(a, '4');
    printf("Unsigned double word: ");
    print_int_vector(a, '5');
    printf("Signed double word:   ");
    print_int_vector(a, '6');
    printf("Unsigned quad word:   ");
    print_int_vector(a, '7');
    printf("Signed quad word:     ");
    print_int_vector(a, '8');
}

void print_spfp_vector(__m128 a)
{
    floatVec tmp;
    tmp.float128 = a;
    printf("[");
    for (int i = 3; i > 0; i--)
    {
        printf("%f", tmp.m128_sp[i]);
        printf(", ");
    }
    printf("%f", tmp.m128_sp[0]);
    printf("]\n");
}

#endif