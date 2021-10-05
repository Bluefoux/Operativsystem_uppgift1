/***************************************************************************
 *
 * Sequential version of Matrix-Matrix multiplication
 *
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 1024

static double a[SIZE][SIZE];
static double b[SIZE][SIZE];
static double c[SIZE][SIZE];

struct threadArgs{
    int row;
};

static void
init_matrix(void)
{
    int i, j;

    for (i = 0; i < SIZE; i++)
        for (j = 0; j < SIZE; j++) {
	        /* Simple initialization, which enables us to easy check
	         * the correct answer. Each element in c will have the same
	         * value as SIZE after the matmul operation.
	         */
	        a[i][j] = 1.0;
	        b[i][j] = 1.0;
        }
}

void* mult(void* param){
    struct threadArgs *args = (struct threadArgs*) param;
	int start = args->row;
    int j, k;
    int end = start +256;
    for (int i = start; i < end; i++){
        for (j = 0; j < SIZE; j++) {
            c[i][j] = 0.0;
            for (k = 0; k < SIZE; k++)
                c[i][j] = c[i][j] + a[i][k] * b[k][j];
        }
    }
}


static void
matmul_seq()
{
    pthread_t *parra;
    struct threadArgs *args; //buffer with arguments
    parra = malloc(4 * sizeof(pthread_t));
    args = malloc(4 * sizeof(struct threadArgs*));
    for (int i = 0; i < 4; i++) {
        args[i].row = i*256;
        pthread_create(&(parra[i]), NULL, mult, (void*)&args[i]);
    }
    for (int i = 0; i < 4; i++) {
		pthread_join(parra[i], NULL );
	}
    free(args);
    free(parra);
}

static void
print_matrix(void)
{
    int i, j;
    int antal = 0;
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++){
            if (c[i][j] == 1024)
                antal++;
        }
	        //printf(" %7.2f", c[i][j]);
	    //printf("\n");
    }
    printf("antal = %d", antal);
}

int
main(int argc, char **argv)
{
    init_matrix();
    matmul_seq();
    print_matrix();
}
