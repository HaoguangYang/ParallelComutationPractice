#include <stdio.h>
#include <sys/time.h>

long long num_steps = 1000000000;
double step;

double calc(void)
{
	struct timeval start, stop;
	double x, pi, sum=0.0;
	int ret, i;
	step = 1./(double)num_steps;
	ret = gettimeofday(&start, NULL);
	if( ret != 0 ) printf("start timer error");

#pragma omp parallel for private(x) reduction(+:sum)
	for (i=0; i<num_steps; i++)
	{
		x = (i + .5)*step;
		sum = sum + 4.0/(1.+ x*x);
	}
	
	pi = sum*step;

	ret = gettimeofday(&stop, NULL);
	if( ret != 0 ) printf("stop timer error");

	printf("Size of problem is %d\n",num_steps);
	printf("The value of PI is %20.17f\n",pi);
	printf("The time to calculate PI was %f microseconds\n\n",((double)(stop.tv_sec - start.tv_sec)*1000000.+(double)(stop.tv_usec - start.tv_usec)));
	return 0;
}

int main(int argc, char* argv[])
{
	if (argc>1)
	{
		int j;
		for (j=1; j<argc; j++)
		{
			num_steps = atoll(argv[j]);
			step = 0.0;
			int errnum = calc();
		}
	}
	return 0;
}