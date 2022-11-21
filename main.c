/*

Program models steady-state heat distribution across a square plate.
The heat distribution is modeled via the steady-state 2D heat equation.
The resulting system of algebraic equations is solved by parallelising
the iterative solver via the OpenMP API.


Scenario
--------

the north (top) plate has a temperature of 0 deg. celsius
the rest of the plate-sides have temperatures of 100 deg. celsius


                    I = 0

          [0][0]-------------[0][N-1]
             |                  |
      J = 0  |                  |  J = N-1
             |                  |
        [M-1][0]-----------[M-1][N-1]

                    I = M-1


this diagram will prove very useful so refer to it.

solution to the heat-equation:

W(Central) = (1/4)*[W(North] + w[South] + w[East] + w[West])


*/
//libraries
#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<omp.h>

int main(int argc, char *argv[]) {

    //number of nodes across plate; N columns, M rows
    # define M 500
    # define N 500
    //counters
    int i, j;
    //variable to be used for differences between tolerance and iterations
    double diff;
    //variable used for threading purposes; will be used for reduction operation
    double my_diff;
    //level of acceptable error to solution
    double tolerance = 0.001;
    //iterations to be performed
    int iterations;
    //tell user about number of iterations
    int iterations_print;
    //average of boundary values
    double mean;
    //solution at previous iteration
    double u[M][N];
    //solution at current/latest iteration
    double w[M][N];
    //wall/clock time; how long did it take
    double wtime;

    printf("\n");
    printf("The iteration will be repeated until the change is <= %e\n", tolerance); 
    printf("Number of processors available = %d\n", omp_get_num_procs());
    printf("Number of threads =              %d\n", omp_get_max_threads());

    mean = 0.0;

    //w shared between threads
    //node-counters (i, j); each thread requires its own instance of i and j
    #pragma omp parallel shared(w) private(i, j)
    {
        #pragma omp for
        //west wall heat distribution
        for (int i = 1; i < M - 1; i++)
        {
            w[i][0] = 100.0;
        }
        #pragma omp for
        //east wall heat distribution
        for (int i = 1; i < M - 1; i++)
        {
            w[i][N-1] = 100.0;
        }
        #pragma omp for
        //south wall heat distribution
        for (int j = 0; j < N; j++)
        {
            w[M-1][j] = 100.0;
        }
        #pragma omp for
        //north wall heat distribution
        for (int j = 0; j < N; j++)
        {
            w[0][j] = 0.0;
        }
        //average boundary values; this will give decent initial value for interior of plate
        #pragma omp for reduction ( + : mean )
        for (int i = 1; i < M - 1; i++)
        {
            mean = mean + w[i][0] + w[i][N-1];
        }
        #pragma omp for reduction ( + : mean )
        for (int j = 0; j < N; j++)
        {
            mean = mean + w[M-1][j] + w[0][j];
        }
    }
    //mean (average value) is private to each thread; find the mean outside of parallel region.
    //once this has been found, take the value back into the parallel region.
    mean = mean/(double)(2*M + 2*N - 4);

    printf("\n");
    printf("Average = %f\n", mean);

    #pragma omp parallel shared(mean, w) private(i, j)
    {
        #pragma omp for
        //initialize interior solution to the average value of solutions
        for (int i = 1; i < M - 1; i++)
        {
            for (int j = 1; j < N - 1; j++)
            {
                w[i][j] = mean;
            }
        }
    }
    //iterate until w (the current solution) differs from u (the previous solution)
    //below the tolerance value. 
    //i.e. find where the previous and current heat distribution is (approximately) the same

    iterations = 0;
    iterations_print = 1;
    printf("\n");
    printf("Iteration  Change\n");
    printf("\n");
    wtime = omp_get_wtime ();

    diff = tolerance;
    //start finding where previous and current heat distribution is approximately the same
    while (tolerance <= diff )
    {
        //previous and current heat distribution values share same threads
        #pragma omp parallel shared ( u, w ) private ( i, j )
        {
            //previous solution saved in u
            #pragma omp for
            for (int i = 0; i < M; i++ ) 
            {
                for (int j = 0; j < N; j++ )
                {
                    u[i][j] = w[i][j];
                }
            }

            #pragma omp for
            for (int i = 1; i < M - 1; i++ )
            {
                for (int j = 1; j < N - 1; j++ )
                {
                    w[i][j] = ( u[i-1][j] + u[i+1][j] + u[i][j-1] + u[i][j+1] )/4.0;
                }
            }
        }   
    //need to find maximum value as a reduction operator. my_diff
    diff = 0.0;

    #pragma omp parallel shared(diff, u, w) private(i, j, my_diff)
    {
        my_diff = 0.0;

        #pragma omp for
        for (int i = 1; i < M - 1; i++ )
        {
            for (int j = 1; j < N - 1; j++ )
            {
                if (my_diff < fabs ( w[i][j] - u[i][j]))
                {
                    my_diff = fabs ( w[i][j] - u[i][j] );
                }
            }
        }
        //execute this to 1 thread (at a time)
        #pragma omp critical
        {
            if (diff < my_diff)
            {
                diff = my_diff;
            }
        }
    }

    iterations++;

    if( iterations == iterations_print )
    {
        printf("  %8d  %f\n", iterations, diff);
        iterations_print = 2 * iterations_print;
    }
  } 

  wtime = omp_get_wtime() - wtime;

  //end of task: output information for users
  printf("\n");
  printf("  %8d  %f\n", iterations, diff);
  printf("\n");
  printf("Error tolerance achieved.\n");
  printf("Wallclock time = %f\n", wtime);

  
  return 0;

  #undef M
  #undef N

}