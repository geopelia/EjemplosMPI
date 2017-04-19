#include "mpi.h"
#include <stdio.h>

#define USE_MPI
#include "sprng.h"
#define BATCHSIZE 1000000
#define SEED 985456376

int main(int argc, char *argv[]) {
    int i, j, numin = 0, totalin, total, numbatches, rank, numprocs, streamnum, nstreams, *stream;
    double x, y, approx, pi = 3.141592653589793238462643;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    streamnum = rank;
    nstreams = numprocs;
    stream = init_sprng(1, streamnum, nstreams, SEED, SPRNG_DEFAULT);

    if (rank == 0) {
       numbatches = atoi(argv[1]);
       printf("%s%23s%21s\n", "pi", "error", "points");
    }
    MPI_Bcast(&numbatches, 1, MPI_INT, 0, MPI_COMM_WORLD); 
    for ( i = 0; i < numbatches; i++) {
        for (j = 0; j < BATCHSIZE; j++) {
            x = sprng(stream); y = sprng(stream);
            if (x * x + y * y < 1.0) {
                numin++;
            }
        }
        //printf("numin es: %d\n", numin);
        MPI_Reduce(&numin, &totalin, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
        if (rank == 0) {
            total = BATCHSIZE * (i +1) * numprocs;
            approx = 4.0 * ((double) totalin / total);
            //printf("pi = %.16f; error = %.16f, points = %d\n", approx, pi - approx, total);
            printf("%.16f; %.16f; %d\n", approx, pi - approx, total);
        }
    }
    free_sprng(stream);
    MPI_Finalize();
    return 0;
}

