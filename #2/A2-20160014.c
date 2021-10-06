#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "mpi.h"
/*
Ahmed Samer Fekry
20160014
*/

//use mpicc -lm filename.c to compile the file without errs


int main(int argc, char* argv[]){

    int my_rank;
    int p;
    MPI_Status status;

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
    MPI_Comm_size(MPI_COMM_WORLD,&p);
    srand(time(NULL)+my_rank);

    int n,i,total_sum=0;
    int *arr, local_sum=0;
    int total_mean=0;
    int squared_diff=0, variance;

    if(my_rank==0){
        printf("Enter n \n");
        scanf("%d",&n);

    }

    MPI_Bcast(&n,1,MPI_INT,0,MPI_COMM_WORLD);

    //creating n random numbers & calculating their sum
    if(my_rank!=0){
        //generating n random numbers
        arr = malloc(n * sizeof(int));
        for(i =0;i<n;i++){
            arr[i]= rand()%50;
            local_sum += arr[i];
        }

    }

    MPI_Allreduce(&local_sum,&total_sum,1,MPI_INT,MPI_SUM,MPI_COMM_WORLD);

    if(my_rank!=0){
        total_mean = total_sum / (n*(p-1));

        for (i = 0; i < n; i++)
            squared_diff += pow(arr[i] - total_mean, 2);
    }

    //doesn't reduce floats so, I changed squared_diff and std_dev to int
    MPI_Reduce(&squared_diff,&variance,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);

    //Calculating Standard deviation
    if(my_rank==0){
        // Standard deviation formula
        int std_dev = sqrt(variance/(n*(p-1)) );
        int mean = total_sum / ( (p-1)*n ) ;
        printf("Total Sum = %d\n",total_sum);
        printf("Total Mean = %d\n",mean);
        printf("Variance = %d\n",variance);
        printf("Standard Deviation (sqrt(Variance)) = %d\n",std_dev);

    }

    MPI_Finalize();

    return 0;

}
