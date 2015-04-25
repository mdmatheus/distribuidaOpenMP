mean: mean.c
	gcc -fopenmp mean.c -o mean.out

clean:
	rm *.out -f
