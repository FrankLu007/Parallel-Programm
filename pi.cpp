#include <cstdlib>
#include <pthread.h>
#include <random>
#include <cstdio>
#include <cstring>
#include <sys/time.h>
int core = 4;
long long total = 1000000, num, mod, **record;
pthread_t *thread;
void* count(void *argv)
{
	int s = (int)(long long)argv;
	long long cnt = -num, ans = 0;
	float x, y;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(0.0, 1.0);
	if(mod > s) cnt--;
	while(cnt++)
	{
		x = dis(gen);
		y = dis(gen);
		if(x*x + y*y <= 1.0f) ans++;
	}
	record[s][0] = ans;
}
int main(int args, char* argv[])
{
	struct timeval start, end;
	gettimeofday(&start, 0);
	if(args > 1) core = std::atoi(argv[1]);
	if(args > 2) total = std::atoll(argv[2]);
	thread = new pthread_t[core-1];
	record = new long long*[core-1];
	num = total/core;
	mod = total%core;
	for(int i = 0 ; i < core-1 ; i++) record[i] = new long long[1];
	for(int i = 0 ; i < core-1 ; i++) pthread_create(&thread[i], 0, count, (void*)i);
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(0.0, 1.0);
	long long ans = 0, cnt = -num;
	float x, y;
	while(cnt++)
	{
		x = dis(gen);
		y = dis(gen);
		if(x*x + y*y <= 1.0f) ans++;
	}
	for(int i = 0 ; i < core-1 ; i++) pthread_join(thread[i], NULL);
	for(int i = 0 ; i < core-1 ; i++) ans += record[i][0];
	std::printf("PI : %.8lf\n", (double)ans/total*4.0);
	gettimeofday(&end, 0);
	std::printf("%.8lf", end.tv_sec-start.tv_sec + (end.tv_usec-start.tv_usec)/1000000.0);
	return 0;
}