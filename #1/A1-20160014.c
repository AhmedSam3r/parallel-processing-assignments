#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int countingPrime(int a,int b)
{
	int flag;
	int i,j;
    int sub_count=0;

    for (i=a; i<b; i++) {

        if (i == 1 || i == 0 || i < 0)
            continue;

        flag = 1;

        for (j = 2; j <=i/2; ++j) {
            if (i % j == 0) {
                flag = 0;
                break;
            }
        }
        // flag = 1 means i is prime
        // and flag = 0 means i is not prime
        if (flag)
            sub_count+=1;

    }

	return sub_count;
}


int main(int argc, char* argv[]) {
    int my_rank;
    int p;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if(my_rank==0)
    {
        int x,y; //global lower bound & global upper bound
    
	    printf("Enter value of x\n");
	    scanf("%d",&x);

	    printf("Enter value of y\n");
	    scanf("%d",&y);

        int total_count=0;
        int r ;
        r = (y-x) / (p-1);


        int i;
        int diff,flag = 0;
        // in case the last subrange doesn't cover all numbers from [local_lower,global_upper_bound]
        int last_num = ( r * (p-1) + x );
        if(last_num < y){
            flag = 1;
            diff = y - last_num;
        }

        //it calculates prime number from [x,y] (x to y)
        for (i = 1; i < p ; i++) {

            if(i+1==p && flag){
                r += diff;
            }

            // in order to avoid exceeding our range
            if(x+r > y)
                r = 0;

            //countingPrime [a,y+1] so that ((y)) is calculated
            if(i+1 ==p){
                r = r + 1;
            }


            MPI_Send(&x,1,MPI_INT,i,0,MPI_COMM_WORLD);
            MPI_Send(&r,1,MPI_INT,i,1,MPI_COMM_WORLD);
      
            x += r;
        }

        for (i = 1; i < p ; i++) {
            int sub_count;
            MPI_Recv(&sub_count,1,MPI_INT,i,0,MPI_COMM_WORLD,&status);
	        printf("Total number of prime numbers in P%d = %d\n",i,sub_count);
            total_count+=sub_count;
        }

        printf("------Toal Number Of Primes------ = %d\n",total_count);
    }

    else{
        int a,b,r;
        
        MPI_Recv(&a,1,MPI_INT,0,0,MPI_COMM_WORLD,&status);
        MPI_Recv(&r,1,MPI_INT,0,1,MPI_COMM_WORLD,&status);

        //Calculating local upper bound
        b = a + r;


        //counting prime numbers within local bounds [a,b[
        int sub_count = countingPrime(a,b);

        printf("Partial count = %d\n",sub_count);

        MPI_Send(&sub_count,1,MPI_INT,0,0,MPI_COMM_WORLD);

    }

    MPI_Finalize();

    return 0;
}
