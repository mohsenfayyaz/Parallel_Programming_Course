#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <algorithm>

#define VECTOR_SIZE 1048576
#define NUM_THREADS 5

struct Find_max_struct {
    float *v;  // Find max in vector v
    long start_index;  // From where
    long end_index;  // To where
    int thread_index;
};

long max_index[NUM_THREADS];

void *parallel_posix_find_max_thread(void *arg){
    Find_max_struct *fms = (Find_max_struct*) arg;
    float local_max = (fms->v)[fms->start_index];
    long local_max_index = fms->start_index;
    // printf("%ld-%ld->%d\n", fms->start_index, fms->end_index, fms->thread_index);
    for(long i = fms->start_index; i <= fms->end_index; i++){
        if((fms->v)[i] >= local_max){
            local_max_index = i;
            local_max = (fms->v)[i];
        }
    }
    // Global Var
    max_index[fms->thread_index] = local_max_index;
    pthread_exit(NULL);
}

void parallel_posix(float *v1)
{
    pthread_t th[NUM_THREADS];
    long each_thread_len = VECTOR_SIZE / NUM_THREADS;
    for (int i = 0; i < NUM_THREADS; i++){
        Find_max_struct *fms = new Find_max_struct;
        if(i != NUM_THREADS - 1){
            *fms = {v1, i * each_thread_len, (i + 1) * each_thread_len - 1, i};
        }else{
            *fms = {v1, i * each_thread_len, VECTOR_SIZE - 1, i};
        }
        pthread_create(&th[i], NULL, parallel_posix_find_max_thread, fms); 
    }

    float max = v1[0];
    long max_i;
    for (int i = 0; i < NUM_THREADS; i++){
        pthread_join(th[i], NULL);
        if(v1[max_index[i]] >= max){
            max_i = max_index[i];
            max = v1[max_index[i]];
        }
    }

    printf("max = %f , index = %ld \n", max, max_i);
}

void parallel_omp(float *v1)
{
    float max = 0, maxi = 0;
    long index = 0, indexi = 0;
    #pragma omp parallel \
    num_threads(NUM_THREADS) private(indexi, maxi)
    {
        indexi = 0;
        maxi = 0;
        #pragma omp for
        for (long i = 0; i < VECTOR_SIZE; i++)
        {
            if (v1[i] > maxi)
            {
                maxi = v1[i];
                indexi = i;
            }
        }

    #pragma omp critical
        {
            if (max < maxi)
            {
                max = maxi;
                index = indexi;
            }
        }
    }

    printf("max = %f , index = %ld \n", max, index);
}

void scalar(float *v1)
{
    float max = 0;
    long index = 0;
    for (long i = 0; i < VECTOR_SIZE; i++)
    {
        if (v1[i] > max)
        {
            max = v1[i];
            index = i;
        }
    }
    printf("max = %f , index = %ld \n", max, index);
}

int main(void)
{
    float v1[VECTOR_SIZE];
    for (long i = 0; i < VECTOR_SIZE; i++)
    {
        v1[i] = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 100.0));
    }

    struct timeval start, end;

    printf("Mohsen Fayyaz - Mahdi Hadiloo\n");
    printf("  810196650   -   810196587 \n");

    gettimeofday(&start, NULL);
    parallel_posix(v1);
    gettimeofday(&end, NULL);
    long seconds3 = (end.tv_sec - start.tv_sec);
    long micros3 = ((seconds3 * 1000000) + end.tv_usec) - (start.tv_usec);

    gettimeofday(&start, NULL);
    scalar(v1);
    gettimeofday(&end, NULL);
    long seconds = (end.tv_sec - start.tv_sec);
    long micros1 = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);

    gettimeofday(&start, NULL);
    parallel_omp(v1);
    gettimeofday(&end, NULL);
    long seconds2 = (end.tv_sec - start.tv_sec);
    long micros2 = ((seconds2 * 1000000) + end.tv_usec) - (start.tv_usec);



    printf("Serial Run time = %ld \n", micros1);
    printf("Parallel_OMP Run time = %ld \n", micros2);
    printf("Parallel_POSIX Run time = %ld \n", micros3);
    printf("Speedup(Serial/POSIX) = %f\n\n", (float)(micros1) / (float)micros3);
}