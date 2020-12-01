//#include "filesystem.hpp"
//#include "streams.hpp"
//#include "mutex.hpp"
//#include "time.hpp"
#include "thread_pool.hpp"
#include <list>

int task(int a)
{
	std::this_thread::sleep_for(std::chrono::seconds(5));
	return 5;
}

int main(int argc,char* argv[])
 {
	//filesystemMain();
	//fix_out();
	//mixed_buffers("sandbox1/file.txt");
	//streamTests("sandbox1/file.txt");
	//main_m();
    //chrono_main();
	std::vector<int> v{1,2,3,4,43,43,4,234,23,4,234,23,4,234,23,5,234,3,4,235,35,234,23,4,35,2};
	int a {parallel_accumulate(v.begin(),v.end(),0)};
	//decltype(v)::difference_type a {std::distance(v.begin(),v.end())};
	std::cout << a << std::endl;

	return 0;
}
