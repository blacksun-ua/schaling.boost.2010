/*
Refactor the following program to calculate the total using two threads. 
Since many processors nowadays have two cores, the execution time should decrease by utilizing threads.

#include <boost/date_time/posix_time/posix_time.hpp> 
#include <boost/cstdint.hpp> 
#include <iostream> 

int main() 
{ 
  boost::posix_time::ptime start = boost::posix_time::microsec_clock::local_time(); 

  boost::uint64_t sum = 0; 
  for (int i = 0; i < 1000000000; ++i) 
    sum += i; 

  boost::posix_time::ptime end = boost::posix_time::microsec_clock::local_time(); 
  std::cout << end - start << std::endl; 

  std::cout << sum << std::endl; 
} 

Solve it by using pthreads native library
To compile it do "g++ pthread -o 1a 1a.cpp" in command line
*/


#include <pthread.h>
#include <stdio.h>
#include <time.h>

const unsigned long limit = 1000000000;

struct worker_t
{
	unsigned long sum;
	unsigned long lower_bound;
	unsigned long upper_bound;
	char unused[53];
};

void* worker(void* data)
{
	worker_t* wrk = (worker_t*)data;
	for(long i = wrk->lower_bound; i < wrk->upper_bound; ++i)
		wrk->sum += i;
}

void refactored_func()
{
	pthread_t threads[2];
	worker_t w1, w2;
   
	w1.sum = 0, w1.lower_bound = 0, w1.upper_bound = limit/2;
	w2.sum = 0, w2.lower_bound = limit/2, w2.upper_bound = limit; 
	
	(void) pthread_create(&threads[0], NULL, worker, &w1);
	(void) pthread_create(&threads[1], NULL, worker, &w2);
	
	(void) pthread_join(threads[0], NULL);
	(void) pthread_join(threads[1], NULL);
   
	printf("%lu\n", w1.sum + w2.sum);
}

void original_func()
{
	unsigned long sum = 0;
	for(unsigned long i = 0; i < limit; ++i)
		sum += i;
	
	printf("%lu\n", sum);
}

int main (int argc, char *argv[])
{
	printf("CPS = %d\n", CLOCKS_PER_SEC);
	clock_t begin, end;
	begin= clock();
	original_func();
	end = clock();
	printf("Execution time = %d ms\n", (end - begin)*1000/CLOCKS_PER_SEC);
	
	begin= clock();
	refactored_func();
	end = clock();
	printf("Execution time = %d ms\n", (end - begin)*1000/CLOCKS_PER_SEC/2);	// divide on 2 because 2 cores used
	
	return 0;
}
