#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <sys/time.h> 
#include <sys/resource.h>  
#include <algorithm>
#include <iostream>
#include <vector>

#define VECTOR_SIZE 1048576
// #define VECTOR_SIZE 15
#define NUM_THREADS 9  // This is not accurate

using namespace std;

void show(float* v){
    for (long i = 0; i < VECTOR_SIZE; i++)
    {
        cout << v[i] << " ";
    }
    cout << "\n";
}

void swap(float* v, long a, long b){
    float temp = v[a];
    v[a] = v[b];
    v[b] = temp;
}

long partition (float* v, long low, long high) 
{ 
    long p = low;
    float pivot = v[low];
    for (long i = low + 1; i <= high; i++) 
    { 
        if (v[i] <= pivot) 
        { 
            p++;    
            swap(v, i, p); 
        } 
    } 
    swap(v, low, p); 
    return p; 
} 


void quickSort(float* v, long left, long right){
    if(left < right){
        long p = partition(v, left, right);
        quickSort(v, left, p -1);
        quickSort(v, p + 1, right);
    }
}

void parallelQuickSort(float* v, long left, long right){
    if(left < right){
        long p = partition(v, left, right);
        #pragma omp task
            parallelQuickSort(v, left, p -1);
            
        #pragma omp task
            parallelQuickSort(v, p + 1, right);
        
    }
}

void scalar(float* v1){
    quickSort(v1, 0, VECTOR_SIZE - 1);
}

void parallel(float* v,long left ,long right){
    #pragma omp parallel num_threads(4)
	{
		#pragma omp single
		{
            parallelQuickSort(v,left,right);
		}
	}
}

struct Arg_struct {
    float* v;
    long left;
    long right;
    int depth;
};

void *parallelQuickSortPOSIX(void *arg){
    Arg_struct *args = (Arg_struct*) arg;
    float *v = args->v;
    long left = args->left;
    long right = args->right;
    int depth = args->depth;
    
    if(left < right){
        long p = partition(v, left, right);
        if(right - left < VECTOR_SIZE / NUM_THREADS){  // Do it serial
            quickSort(v, left, p -1);
            quickSort(v, p + 1, right);
        }else{
            // printf("%ld-%ld->%d\n", left, right, depth);
            Arg_struct left_arg = {v, left, p - 1, depth + 1};
            pthread_t left_th;
            pthread_create(&left_th, NULL, parallelQuickSortPOSIX, &left_arg);

            Arg_struct right_arg = {v, p + 1, right, depth + 1};
            pthread_t right_th;
            pthread_create(&right_th, NULL, parallelQuickSortPOSIX, &right_arg);
            
            // parallelQuickSortPOSIX(&right_arg);

            pthread_join(left_th, NULL);
            pthread_join(right_th, NULL);
        }
    }
    pthread_exit(NULL);
}

void parallelPOSIX(float* v, long left, long right){
    Arg_struct arg = {v, left, right, 0};
    pthread_t th;
    pthread_create(&th, NULL, parallelQuickSortPOSIX, &arg);
    pthread_join(th, NULL);
}

void copyArray(float* a, float* b){
    for (long i = 0; i < VECTOR_SIZE; i++)
    {
        b[i] = a[i];
    }
}

int main(void){    
    float* v1 = new float[VECTOR_SIZE];
    float* v2 = new float[VECTOR_SIZE];
    float* v3 = new float[VECTOR_SIZE];
    for (long i = 0; i < VECTOR_SIZE; i++){
		float temp = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 100.0));
        v1[i] = temp;
        v2[i] = temp;
        v3[i] = temp;
    }


	struct timeval start, end;


    printf("Mohsen Fayyaz - Mahdi Hadiloo\n");
    printf("  810196650   -   810196587 \n");

    gettimeofday(&start, NULL);
	parallelPOSIX(v3, 0 , VECTOR_SIZE -1);
	gettimeofday(&end, NULL);
	// show(v3);
    long seconds3 = (end.tv_sec - start.tv_sec);
	long micros3 = ((seconds3 * 1000000) + end.tv_usec) - (start.tv_usec);

	gettimeofday(&start, NULL);
	scalar(v1);
	gettimeofday(&end, NULL);
	// show(v1);
    long seconds = (end.tv_sec - start.tv_sec);
	long micros1 = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);

	gettimeofday(&start, NULL);
	parallel(v2, 0 , VECTOR_SIZE -1);
	gettimeofday(&end, NULL);
	// show(v2);
    long seconds2 = (end.tv_sec - start.tv_sec);
	long micros2 = ((seconds2 * 1000000) + end.tv_usec) - (start.tv_usec);

	printf("Serial Run time = %ld \n", micros1);
	printf("Parallel_OMP Run time = %ld \n", micros2);
    printf("Parallel_POSIX Run time = %ld \n", micros3);
    printf("Speedup(Serial/POSIX) = %f\n\n", (float)(micros1) / (float)micros3);

}