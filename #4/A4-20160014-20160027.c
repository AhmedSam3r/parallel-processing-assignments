#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include "mpi.h"

/*
 * To compile correctly use mpicc -fopenmp filename.c -lm
 Ahmed Samer Fekry      20160014
 Ahmed Mohamed Gaballah 20160027
 */


int main ( int argc , char *argv[]) {
    int my_rank, p;
    double global_sum = 0.0, local_sum = 0.0, error;
    double master_sum;
    int i;
    int N = 20;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    {
        //sending N to all processes
        MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
        double pragma_sum=0;
        #pragma omp parallel  shared(global_sum) private(i,local_sum)
        {
            //when i put dynamic it gives me NAN , why ?
            #pragma omp for schedule(static)
            for (i = my_rank + 1; i <= N; i += p) {
                printf("rank : %d && thread : %d of %d threads\n", my_rank, omp_get_thread_num(),omp_get_num_threads());
                //printf("thread #%d && local_sum = %f\n",omp_get_thread_num(),local_sum);
                //there're times where local_sum=0 which means it doesn't add thing solution:reduction
                //also when i remove local_sum from private this lead to wrong results,,why ??
                local_sum += (1.0 / pow(2,i)*1.0) ;
            }

            # pragma omp critical
            global_sum += local_sum;
        }
;

        MPI_Reduce(&global_sum, &master_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);


        // We must reduce global_sum as if we calculated error value above won't be completed as there're some threads still going
        if(my_rank==0){
            error = (1.0 - master_sum)*1.0 ;
            printf("Result = %f\n ,Error = %f\n",master_sum,error);
        }


    }


    MPI_Finalize();

    return 0;

}

