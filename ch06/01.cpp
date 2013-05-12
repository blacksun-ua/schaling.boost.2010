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

Compile with "g++ -pthread -o 01 ./01.cpp /usr/lib64/libboost_thread.so"
*/

#include <boost/date_time/posix_time/posix_time.hpp> 
#include <boost/thread.hpp>
#include <boost/cstdint.hpp> 
#include <boost/lexical_cast.hpp>
#include <iostream> 

static const int limit = 1000000000;

void origin_func()
{
	boost::posix_time::ptime start = boost::posix_time::microsec_clock::local_time(); 

	boost::uint64_t sum = 0; 
	for (int i = 0; i < limit; ++i) 
		sum += i; 

	boost::posix_time::ptime end = boost::posix_time::microsec_clock::local_time(); 
	std::cout << end - start << std::endl; 

	std::cout << sum << std::endl; 
}

struct worker_data_t
{
	boost::int64_t sum;
	int lower_bound;
	int upper_bound;
	long reserved[6];	// NOTE!!! This need for cache line data align. Without this code doesn't work properly. 
	// Data size shoul be larger than cache line size (in my case it's 64 bytes)
};

void worker(worker_data_t& data)
{
	for(int i = data.lower_bound; i < data.upper_bound; ++i)
		data.sum += i;
}

typedef void (*wrk)(worker_data_t&);

template <typename FP, typename D> class Adapter
{
public:
	Adapter(FP fp, D& d) : fp_(fp), d_(d) {}
	void operator()()
	{
		fp_(d_);
	}
	
private:
	FP fp_;
	D& d_;
};

void refactored_func()
{
	worker_data_t d1, d2;
	d1.sum = 0, d2.sum = 0;
	d1.lower_bound = 0, d2.lower_bound = limit/2;
	d1.upper_bound = limit/2, d2.upper_bound = limit;
	
	boost::posix_time::ptime start = boost::posix_time::microsec_clock::local_time();

	boost::thread_group tgr;
	tgr.create_thread(Adapter<wrk, worker_data_t>(worker, d1));
	tgr.create_thread(Adapter<wrk, worker_data_t>(worker, d2));
	tgr.join_all();
	
	boost::posix_time::ptime end = boost::posix_time::microsec_clock::local_time();
	std::cout << end - start << std::endl;

	std::cout << d1.sum + d2.sum << std::endl;
}

int main(int argc, char* argv[])
{ 
	refactored_func();
	origin_func();
} 
