#include <stdio.h>
#include <math.h>
#include <sys/time.h> 
#include <sys/resource.h>  
#include <omp.h>

double timeGetTime() 
{     
	struct timeval time;     
	struct timezone zone;     
	gettimeofday(&time, &zone);     
	return time.tv_sec + time.tv_usec*1e-6; 
}  


const long int VERYBIG = 100000;


int main( void )
{
  int i;
  long int j, k, sum;
  double sumx, sumy, total, z;
  double starttime, elapsedtime, start, average = 0;
  double threadTimes[4];

  for (int i = 0; i < 4; i++)
  {
    threadTimes[i] = 0;
  }
  
  // ---------------------------------------------------------------------
  // Output a start message
  printf( "OpenMP Parallel Timings for %ld iterations \n\n", VERYBIG );

  // repeat experiment several times
  for( i=0; i<6; i++ )
  {
    // get starting time
    starttime = timeGetTime();
    // reset check sum and total
    sum = 0;
    total = 0.0;
    
    // Work loop, do some work by looping VERYBIG times
    #pragma omp parallel for     \
      num_threads (4) \
      private( sumx, sumy, k )   \
      reduction( +: sum, total ) \
      schedule( static)
// schedule( dynamic, 2000 )
// schedule( dynamic, 1000 )

      for( int j=0; j<VERYBIG; j++ )
      {
        start = timeGetTime();
        // increment check sum
        sum += 1;
       
        // Calculate first arithmetic series
        sumx = 0.0;
        for( k=0; k<j; k++ )
         sumx = sumx + (double)k;

        // Calculate second arithmetic series
        sumy = 0.0;
        for( k=j; k>0; k-- )
         sumy = sumy + (double)k;

        if( sumx > 0.0 )total = total + 1.0 / sqrt( sumx );
        if( sumy > 0.0 )total = total + 1.0 / sqrt( sumy );

        threadTimes[omp_get_thread_num()] += timeGetTime() - start;
    }
    
    // get ending time and use it to determine elapsed time
    elapsedtime = timeGetTime() - starttime;
  
    for (int i = 0; i < 4; i++)
    {
      printf("thread %d take  %f   time\n",i, (threadTimes[i]));
      threadTimes[i] = 0;
    }
    
    average += elapsedtime;
  
    // report elapsed time
    printf("Time Elapsed %10d mSecs Total=%lf Check Sum = %ld\n",
                   (int)(elapsedtime * 1000), total, sum );
  }
  
  printf("average time was %f \n", average/6);
  
  // return integer as required by function header
  return 0;
}
// **********************************************************************
