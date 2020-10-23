#include "stdio.h"
#include "x86intrin.h"
#include <string>

#ifdef _WIN32 //  Windows
#define cpuid __cpuid
#else //  Linux
void cpuid(int CPUInfo[4], int InfoType)
{
    __asm__ __volatile__(
        "cpuid"
        : "=a"(CPUInfo[0]),
          "=b"(CPUInfo[1]),
          "=c"(CPUInfo[2]),
          "=d"(CPUInfo[3])
        : "a"(InfoType));
}
#endif

void print_names()
{
    printf("Mohsen Fayyaz - Mahdi Hadiloo\n");
    printf("  810196650   -   810196587 \n");
}

void print_cpu_info()
{
    int info[4];
    bool MMX = false;
    bool SSE = false;
    bool SSE2 = false;
    bool SSE3 = false;
    bool AES = false;
    bool SSE41 = false;
    bool SSE42 = false;
    bool AVX = false;
    bool AVX2 = false;
    bool SHA = false;

    cpuid(info, 0x00000001);

    MMX = (info[3] & ((int)1 << 23)) != 0;

    SSE = (info[3] & ((int)1 << 25)) != 0;
    SSE2 = (info[3] & ((int)1 << 26)) != 0;
    SSE3 = (info[2] & ((int)1 << 0)) != 0;
    AES = (info[2] & ((int)1 << 25)) != 0;
    SSE41 = (info[2] & ((int)1 << 19)) != 0;
    SSE42 = (info[2] & ((int)1 << 20)) != 0;

    AVX = (info[2] & ((int)1 << 28)) != 0;

    cpuid(info, 0x80000000);
    if (info[0] >= 0x00000007)
    {
        cpuid(info, 0x00000007);
        AVX2 = (info[1] & ((int)1 << 5)) != 0;
        SHA = (info[1] & ((int)1 << 29)) != 0;
    }

    print_names();
    printf("%s\n", MMX ? "MMX   Supported" : "MMX   NOT Supported");
    printf("%s\n", SSE ? "SSE   Supported" : "SSE   NOT Supported");
    printf("%s\n", SSE2 ? "SSE2  Supported" : "SSE2  NOT Supported");
    printf("%s\n", SSE3 ? "SSE3  Supported" : "SSE3  NOT Supported");
    printf("%s\n", SSE41 ? "SSE41 Supported" : "SSE41 NOT Supported");
    printf("%s\n", SSE42 ? "SSE42 Supported" : "SSE42 NOT Supported");
    printf("%s\n", AES ? "AES   Supported" : "AES   NOT Supported");
    printf("%s\n", SHA ? "SHA   Supported" : "SHA   NOT Supported");
    printf("%s\n", AVX ? "AVX   Supported" : "AVX   NOT Supported");
    printf("%s\n", AVX2 ? "AVX2  Supported" : "AVX2  NOT Supported");
}

// Q2

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
        print_int_vector_by_type<uint64_t[2]>(tmp.m128_u64, 1, false, true);
        break;
    case '8':
        print_int_vector_by_type<int64_t[2]>(tmp.m128_i64, 1, true, true);
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
}



int main(void)
{
    print_names();
    // ------Q1------
    // print_cpu_info();

    // -----Q2.a-----
    unsigned char int_array[16] = {0X00, 0X11, 0X22, 0X33, 0X44, 0X55, 0X66, 0X77,
                                   0X88, 0X99, 0XAA, 0XBB, 0XCC, 0XDD, 0XEE, 0XFF};
    __m128i a;
    a = _mm_load_si128((const __m128i *)int_array);
    print_int_test(a);

    // -----Q2.b-----
    float float_array[2] = {1.1, 2.2};

    return 0;
}
