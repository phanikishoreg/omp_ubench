all: clean obj

obj: ubench.c
	gcc -fopenmp -fno-stack-protector ubench.c -o omp_ubench

clean:
	rm -f omp_ubench 
