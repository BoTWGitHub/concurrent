#include <iostream>
#include <thread>
#include <vector>
#include <ctime>

#define NUMS_SIZE 500000000

using namespace std;

void sumFunc(vector<int>& nums, int index, int len, long long& sum){
	sum = 0;
	//cout << "    from " << index << " to " << min(index+len, NUMS_SIZE) << endl;
	for(int i=index;i<min(index+len, NUMS_SIZE);i++) sum+=nums[i];
}

int main()
{
	struct timespec begin_time, end_time;
	clock_gettime(CLOCK_MONOTONIC, &begin_time);
	cout << "generate array" << endl;
	vector<int> nums(NUMS_SIZE);
	for(int i=0;i<NUMS_SIZE/100;i++) nums[i] = rand()%11;
	for(int i=NUMS_SIZE/100;i<NUMS_SIZE;i++) nums[i] = nums[i%100];
	clock_gettime(CLOCK_MONOTONIC, &end_time);
	double t = (end_time.tv_sec-begin_time.tv_sec) + (end_time.tv_nsec - begin_time.tv_nsec)/1000000000.0;
	cout << "time " << t << " s" << endl;

	cout << "simple sum" << endl;
	clock_gettime(CLOCK_MONOTONIC, &begin_time);
	long long sum;
	sumFunc(nums, 0, NUMS_SIZE, sum);
	clock_gettime(CLOCK_MONOTONIC, &end_time);
	t = (end_time.tv_sec-begin_time.tv_sec) + (end_time.tv_nsec - begin_time.tv_nsec)/1000000000.0;
	cout << "sum " << sum << " time " << t << " s" << endl;

	int num_of_threads = thread::hardware_concurrency();
	if(num_of_threads==0) num_of_threads = 2;
	vector<thread> threads(num_of_threads-1);
	vector<long long> sums(threads.size());
	long long len_per_threads = NUMS_SIZE/threads.size();
	cout << "threads " << sums.size() << " len " << len_per_threads << endl;

	clock_gettime(CLOCK_MONOTONIC, &begin_time);
	for(int i=0;i<threads.size();i++){
		threads[i] = thread(sumFunc, ref(nums), i*len_per_threads, len_per_threads, ref(sums[i]));
	}
	sumFunc(nums, threads.size()*len_per_threads, len_per_threads, sum);
	for(auto& entry : threads) entry.join();
	for(long long ret : sums) sum+=ret;
	clock_gettime(CLOCK_MONOTONIC, &end_time);
	t = (end_time.tv_sec-begin_time.tv_sec) + (end_time.tv_nsec - begin_time.tv_nsec)/1000000000.0;
	cout << "sum " << sum << " time " << t << " s" << endl;
	return 0;
}
