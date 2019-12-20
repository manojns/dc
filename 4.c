#include<stdio.h>
#include<omp.h>
#include<stdlib.h>

long long factorial(long n)
{
	long long fact=1;
	for(int i=2;i<=n;i++)
		fact*=i;
	return fact;
}

void main()
{
	int i,j=0,n=10;
	long long *x;
	x=(long long *)malloc(sizeof(long long)*(n+1));
	for(i=0;i<=n;i++)
		x[i]=factorial(i);

	#pragma omp parallel for firstprivate(x,j)
	for(i=1;i<=n;i++)
	{
		j += i;
		x[i] = j*x[i-1];
	}
	for(i=0;i<n;i++)
	printf("Factorial(%2d) = %14lld x[%2d] = %14lld\n",i,factorial(i),i,x[i]);	
}
