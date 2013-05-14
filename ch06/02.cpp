/* Generalize the program from exercise 1 by utilizing as many threads as the processor 
 * can physically execute at the same time. For example, if the processor contains four 
 * cores, a total of four threads should be utilized.
 * 
 * Compile with "g++ -pthread -o 02 ./02.cpp /usr/lib64/libboost_thread.so"
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
	char unused[53];	// NOTE!!! This need for cache line data align. Without this code doesn't work properly. 
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
	boost::posix_time::ptime start = boost::posix_time::microsec_clock::local_time();
	
	int num_of_processors = boost::thread::hardware_concurrency();
	std::vector<worker_data_t> data(num_of_processors);
	boost::thread_group tg;
	
	for(int i = 0; i < num_of_processors; ++i) {
		worker_data_t w = {0, limit/num_of_processors*i, limit/num_of_processors*(i+1)};
		data[i] = w;
		tg.create_thread(Adapter<wrk, worker_data_t>(worker, data[i]));
	}
	tg.join_all();

	boost::posix_time::ptime end = boost::posix_time::microsec_clock::local_time();
	std::cout << end - start << std::endl;

	long sum = 0;
	for(int i = 0; i < num_of_processors; ++i)
		sum += data[i].sum;
	std::cout << sum << std::endl;
}

int main(int argc, char* argv[])
{ 
	refactored_func();
	origin_func();
} 
