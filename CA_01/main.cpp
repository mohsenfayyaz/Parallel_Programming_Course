#include "stdio.h"
#include "x86intrin.h"

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

    printf("Mohsen Fayyaz - Mahdi Hadiloo\n");
    printf("  810196650   -   810196587 \n");
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

typedef union {
	__m128i 			int128;
	
	unsigned char		m128_u8[16];
	signed char			m128_i8[16];

	unsigned short		m128_u16[8];
	signed short		m128_i16[8];

    float               m128_sp[4];
    double              m128_dp[2];
} intVec;

void print_u8 (__m128i a)
{
	intVec tmp;
	tmp.int128 = a;
	printf ("[");
	for (int i=15; i>0; i--) {
		printf ("%X, ", tmp.m128_u8[i]);
	}
	printf ("%X]\n\n", tmp.m128_u8[0]);
}

void print_u16 (__m128i a)
{
	intVec tmp;
	tmp.int128 = a;
	printf ("[");
	for (int i=7; i>0; i--) {
		printf ("%X, ", tmp.m128_u16[i]);
	}
	printf ("%X]\n\n", tmp.m128_u16[0]);
}

void print_int_vector(__m128i a, unsigned char type)
{
    
}

void print_spfp_vector(__m128 a)
{
    
}

int main(void)
{
    print_cpu_info();

    unsigned char intArray[16] = {0X00, 0X11, 0X22, 0X33, 0X44, 0X55, 0X66, 0X77,
                                  0X88, 0X99, 0XAA, 0XBB, 0XCC, 0XDD, 0XEE, 0XFF};

    __m128i a;
	a = _mm_load_si128((const __m128i*)intArray);

	printf ("Unsigned byte: ");
	print_u8 (a);

	printf ("Unsigned word: ");
	print_u16 (a);

	return 0;
}
