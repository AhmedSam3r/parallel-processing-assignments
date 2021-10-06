#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

/*
Ahmed Samer Fekry
20160014
Ahmed Mohamed Gaballah
20160027
*/


int main(int argc, char* argv[]) {


    int row,column;
    char *matrix;
    int i, j;
    char ch ;

    char current,left,right;

    printf("Enter row & column\n");
    scanf("%d",&row);
    scanf("%d",&column);


    matrix = (char*)malloc(row * column * sizeof(char));

    printf("entering content of matrix\n");
    for (i = 0; i <  row; i++){
        for (j = 0; j < column; j++){
            scanf(" %c", &ch);
            *(matrix + i*column + j) = ch;
        }
    }


    
    #pragma omp parallel  shared(matrix,row,column) private(i,j)
    {
        #pragma omp for schedule(static)
        for (i = 0; i < row; i++ ){
                for (j = 1; j < column - 1; j++) {
                    current = *(matrix + i * column + j);
                    left = *(matrix + i * column + (j - 1));
                    right = *(matrix + i * column + (j + 1));

                    //In case capital only or small only was entered
                    if (current == 'O' || current == 'o') {
                        if ( (left == 'X' && right == 'X') || (left == 'x' && right == 'x' ) )
                            *(matrix + i * column + j) = 'X';
                    }

                }
            printf("thread #%d out of %d\n", omp_get_thread_num(),omp_get_num_threads());

        }

    }
    // Out of parallel to print matrix in right order
    printf("printing matrix content\n");
    for (i = 0; i < row; i++) {
        for (j = 0; j < column; j++) {
            ch = *(matrix + i * column + j);
            printf("%c ", ch);
        }
            printf("\n");
    }




    return 0;

}
