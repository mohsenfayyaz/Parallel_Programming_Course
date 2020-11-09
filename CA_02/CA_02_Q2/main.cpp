#include "stdio.h"
// #include "ipp.h"
#include "x86intrin.h"
#include <sys/time.h>
#include <algorithm>
#include "util.cpp"
#define VECTOR_SIZE 1048576  // 2^20 = 1048576

float parallel_mean(float* v1){
	__m128 sum = _mm_set1_ps(0.0);
	for (long i = 0; i < VECTOR_SIZE; i += 4){
		sum = _mm_add_ps(sum, _mm_load_ps(&(v1[i])));
	}
	// print_spfp_vector(sum);
	sum = _mm_hadd_ps(sum, sum);
	sum = _mm_hadd_ps(sum, sum);
	float final_sum = _mm_cvtss_f32(sum);
	// printf("%f\n", final_sum);
	return final_sum / VECTOR_SIZE;
}

float scalar_mean(float* v1) {
	float sum = 0;
	for (long i = 0; i < VECTOR_SIZE; i++){
		sum += v1[i];
	}
	// printf("%f\n", sum);
	return sum / VECTOR_SIZE;
}

int main(void)
{
	print_names();

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
	float sres = scalar_mean(v1);
	gettimeofday(&end, NULL);
	long seconds = (end.tv_sec - start.tv_sec);
	long micros1 = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);

	gettimeofday(&start, NULL);
	float pres = parallel_mean(v1);
	gettimeofday(&end, NULL);
	long seconds2 = (end.tv_sec - start.tv_sec);
	long micros2 = ((seconds2 * 1000000) + end.tv_usec) - (start.tv_usec);

	printf("\nThe serial result is  = %f\n", sres);
	printf("The parallel result is = %f\n", pres);
	printf("Serial Run time = %ld \n", micros1);
	printf("Parallel Run time = %ld \n", micros2);
	printf("Speedup = %f\n\n", (float)(micros1) / (float)micros2);

	return 0;
}
