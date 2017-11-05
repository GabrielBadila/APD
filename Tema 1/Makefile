.PHONY: all clean

all: build

build: serial parallel

serial: g_serial.c
	gcc g_serial.c -o g_serial -Wall -Wextra

parallel: g_omp.c
	gcc -fopenmp g_omp.c -o g_omp -Wall -Wextra

clean:
	rm -rf g_serial g_omp out*.txt
