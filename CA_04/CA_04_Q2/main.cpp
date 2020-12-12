#include <stdio.h>
#include <math.h>
#include <sys/time.h> 
#include <sys/resource.h>  
#include <algorithm>
#include <iostream>
#include <vector>

 

#define VECTOR_SIZE 1048576
// #define VECTOR_SIZE 524288



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
    quickSort(v1,0,VECTOR_SIZE - 1);

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

void copyArray(float* a, float* b){
    for (long i = 0; i < VECTOR_SIZE; i++)
    {
        b[i] = a[i];
    }
    
}

int main(void){    
    float* v1 = new float[VECTOR_SIZE];
    float* v2 = new float[VECTOR_SIZE];
    for (long i = 0; i < VECTOR_SIZE; i++){
		float temp = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 100.0));
        v1[i] = temp;
        v2[i] = temp;
    }


	struct timeval start, end;


    printf("Mohsen Fayyaz - Mahdi Hadiloo\n");
    printf("  810196650   -   810196587 \n");

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
	printf("Parallel Run time = %ld \n", micros2);
	printf("Speedup = %f\n\n", (float)(micros1) / (float)micros2);

}