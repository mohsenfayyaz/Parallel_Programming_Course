#include "stdio.h"
// #include "ipp.h"
#include "x86intrin.h"
#include <sys/time.h>
#include <algorithm>
#include "util.cpp"
#define VECTOR_SIZE 1048576  // 2^20 = 1048576

long parallel_max(float* v1){
	__m128i inc4 = _mm_set1_epi32(4);
	__m128i max_index4 = _mm_setr_epi32(0, 1, 2, 3);
	__m128i cur_max_index4 = _mm_setr_epi32(0, 1, 2, 3);
	__m128 vec1, cmpeq, max_vec = _mm_load_ps(&v1[0]);

	for (long i = 4; i < VECTOR_SIZE; i += 4){
		// print_spfp_vector(max_vec);
		// print_int_vector(max_index4, '6');
		cur_max_index4 = _mm_add_epi32(cur_max_index4, inc4);

		vec1 = _mm_load_ps(&v1[i]);
		max_vec = _mm_max_ps(vec1, max_vec);
		cmpeq = _mm_cmpeq_ps(max_vec, vec1);
		// max_index4 = (__m128i) _mm_blendv_ps((__m128) max_index4, (__m128)cur_max_index4, _mm_shuffle_ps(cmpeq, cmpeq, _MM_SHUFFLE(3,2,1,0)));
		max_index4 = (__m128i) _mm_blendv_ps((__m128) max_index4, (__m128)cur_max_index4, cmpeq);
	}

	// print_spfp_vector(max_vec);
	// print_int_vector(max_index4, '6');
	floatVec m;
	intVec i;
	m.float128 = max_vec;
	i.int128 = max_index4;
	float max_m = m.m128_sp[0];
	int max_i4 = 0;
	for(int i = 1; i < 4; i++){
		if(m.m128_sp[i] >= max_m){
			max_m = m.m128_sp[i];
			max_i4 = i;
		}
	}
	return i.m128_i32[max_i4];
}

long scalar_max(float* v1) {
	float max = v1[0];
	long max_index = 0;
	for (long i = 0; i < VECTOR_SIZE; i++){
		// printf("%f, ", v1[i]);
		if(v1[i] >= max){
			max = v1[i];
			max_index = i;
		}
	}
	// printf("\n");
	return max_index;
}

int main(void)
{
	print_names();
	// Ipp64u start, end;
	// Ipp64u time1, time2;


	float fSTmpRes[4];
	float fSRes;
	float fVRes;

	__attribute__((aligned(16))) float v1 [VECTOR_SIZE]; // Aligned
	// v1 = new float[VECTOR_SIZE];

	if (!v1){
		printf("Memory allocation error!!\n");
		return 1;
	}

	// Initialize vectors with random numbers
	for (long i = 0; i < VECTOR_SIZE; i++)
	{
		v1[i] = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 100.0));
	}

	struct timeval start, end;

	gettimeofday(&start, NULL);
	int sres = scalar_max(v1);
	gettimeofday(&end, NULL);
	long seconds = (end.tv_sec - start.tv_sec);
	long micros1 = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);

	gettimeofday(&start, NULL);
	int pres = parallel_max(v1);
	gettimeofday(&end, NULL);
	long seconds2 = (end.tv_sec - start.tv_sec);
	long micros2 = ((seconds2 * 1000000) + end.tv_usec) - (start.tv_usec);

	printf("\nThe serial result is   = v[%d] = %f\n", sres, v1[sres]);
	printf("The parallel result is = v[%d] = %f\n", pres, v1[pres]);
	printf("Serial Run time = %ld \n", micros1);
	printf("Parallel Run time = %ld \n", micros2);
	printf("Speedup = %f\n\n", (float)(micros1) / (float)micros2);

	return 0;
}
