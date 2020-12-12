#include <stdio.h>
#include <math.h>
#include <sys/time.h> 
#include <sys/resource.h>  
#include <algorithm>

#define VECTOR_SIZE 1048576 

void parallel(float* v1){
    float max = 0, maxi = 0;
    long index = 0 , indexi = 0;
    #pragma omp parallel    \
      num_threads (4) \
      private( indexi, maxi ) 
      {
        indexi = 0;
        maxi = 0 ;
        #pragma omp for
            for (long i = 0; i < VECTOR_SIZE; i++){
                if(v1[i] > maxi){
                    maxi = v1[i];
                    indexi = i;
                }
            }

        #pragma omp critical
        {
            if(max < maxi){
                max = maxi;
                index = indexi;
            }
        }
      }
      
    printf("max = %f , index = %ld \n", max, index);

}


void scalar(float* v1){
    float max = 0;
    long index = 0;
    for(long i = 0; i < VECTOR_SIZE; i++){
		if(v1[i] > max){
            max = v1[i];
            index = i;
        }
	}
    printf("max = %f , index = %ld \n", max, index);

}



int main(void){
    float v1[VECTOR_SIZE];
    for (long i = 0; i < VECTOR_SIZE; i++){
		v1[i] = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 100.0));
	}

	struct timeval start, end;


    printf("Mohsen Fayyaz - Mahdi Hadiloo\n");
    printf("  810196650   -   810196587 \n");

	gettimeofday(&start, NULL);
	scalar(v1);
	gettimeofday(&end, NULL);
	long seconds = (end.tv_sec - start.tv_sec);
	long micros1 = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);

	gettimeofday(&start, NULL);
	parallel(v1);
	gettimeofday(&end, NULL);
	long seconds2 = (end.tv_sec - start.tv_sec);
	long micros2 = ((seconds2 * 1000000) + end.tv_usec) - (start.tv_usec);

	printf("Serial Run time = %ld \n", micros1);
	printf("Parallel Run time = %ld \n", micros2);
	printf("Speedup = %f\n\n", (float)(micros1) / (float)micros2);

}