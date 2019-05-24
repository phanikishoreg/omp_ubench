#include <assert.h>
#include <stdio.h>
#include <omp.h>

#define ITERS 1000
#define RECUR 4 
#define rdtscll(val) __asm__ __volatile__("rdtsc" : "=A" (val))

typedef unsigned long long cycles_t;

#undef PRINT_VALS

void
test_parallel(void)
{
	cycles_t max = 0, total = 0;
	int i, x = 0;

	for (i = 0; i < ITERS; i++) {
		cycles_t st, en, diff;

		rdtscll(st);
		#pragma omp parallel
		{
			x++;
		}
		rdtscll(en);

		assert(en >= st);
		diff = en - st;
		total += diff;
		if (diff > max) max = diff;

#ifdef PRINT_VALS
		printf("%llu\n", diff);
#endif
	}

	printf("uBench Parallel (ITERS=%d): AVG:%llu WC:%llu\n", ITERS, total / ITERS, max);
}

void
test_parallel_critical(void)
{
	cycles_t max = 0, total = 0;
	int i, x = 0;

	for (i = 0; i < ITERS; i++) {
		cycles_t st, en, diff;

		rdtscll(st);
		#pragma omp parallel
		{
			#pragma omp critical
			{
				x++;
			}
		}
		rdtscll(en);

		assert(en >= st);
		diff = en - st;
		total += diff;
		if (diff > max) max = diff;
#ifdef PRINT_VALS
		printf("%llu\n", diff);
#endif

	}

	printf("uBench Parallel+Critical (ITERS=%d): AVG:%llu WC:%llu\n", ITERS, total / ITERS, max);
}

void
test_parallel_task(void)
{
	cycles_t max = 0, total = 0;
	int i, x = 0, y = 0;

	for (i = 0; i < ITERS; i++) {
		cycles_t st, en, diff;

		rdtscll(st);
		#pragma omp parallel
		{
			x++;
			#pragma omp task
			{
				y++;
			}
			#pragma omp taskwait
		}
		rdtscll(en);

		assert(en >= st);
		diff = en - st;
		total += diff;
		if (diff > max) max = diff;
#ifdef PRINT_VALS
		printf("%llu\n", diff);
#endif

	}

	printf("uBench Parallel+Task+Taskwait (ITERS=%d): AVG:%llu WC:%llu\n", ITERS, total / ITERS, max);
}

void
test_parallel_task_4levels(void)
{
	cycles_t max = 0, total = 0;
	int i, x = 0, y = 0;

	for (i = 0; i < ITERS; i++) {
		cycles_t st, en, diff;

		rdtscll(st);
		#pragma omp parallel
		{
			x++;
			#pragma omp task
			{
				#pragma omp task
				{
					#pragma omp task
					{
						#pragma omp task
						{
							y++;
						}
						#pragma omp taskwait
						y++;
					}
					#pragma omp taskwait
					y++;
				}
				#pragma omp taskwait
				y++;
			}
			#pragma omp taskwait
		}
		rdtscll(en);

		assert(en >= st);
		diff = en - st;
		total += diff;
		if (diff > max) max = diff;
#ifdef PRINT_VALS
		printf("%llu\n", diff);
#endif

	}

	printf("uBench Parallel+Task 4levels+Taskwait (ITERS=%d): AVG:%llu WC:%llu\n", ITERS, total / ITERS, max);
}

int
main(void)
{
	test_parallel();
//	test_parallel_critical();
//	test_parallel_task();
//	test_parallel_task_4levels();

	return 0;
}
