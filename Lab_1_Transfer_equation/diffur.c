#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <mpi.h>

#define pi 3.1416

const double A = 0; // start of a x-interval
const double B = 1; // and end
const double h = 0.001; // x-step

const double a = 2;

const double t = 0; // start of a t-interval
const double T = 1; // and end
const double tau = h / 2; // t-step; Courant condition

double real(double x, double t)
{
    if (x >= 2 * t)
        return x * t - t * t / 2 + cos(pi * (2 * t - x));
    else
        return x * t - t * t / 2 + (2 * t - x) * (2 * t - x) / 8 + exp(x / 2 - t);
}

double f(double x, double t)
{
    return x + t;
}

double u1(double t)
{
    return exp(-t);
}

double u0(double x)
{
    return cos(pi * x);
}

double leftCorner(double u_prev, double u_cur, double f)
{
    return u_cur - tau*a/h*(u_cur - u_prev) + tau*f;
}

double cross(double u_left, double u_down, double u_right, double f)
{
    return u_down - tau*a/h*(u_right - u_left) + 2*tau*f; 
}

int main(int argc, char * argv[])
{
    MPI_Init(&argc, &argv);

    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Barrier(MPI_COMM_WORLD);
    double time_begin;
    if(rank == 0) 
        time_begin = MPI_Wtime();


    int N_x = round((B - A) / h + 1); // number of x-steps
    int M_t = round((T - t) / tau + 1); // number of t-steps
    int len = floor((double)N_x / size);
    int start  = rank * len;
    int end = start + len - 1;
    if(rank == size - 1)
        end = N_x - 1;
    int num = end - start + 1;

    /* MPI cannot correctly send 2-dimensional arrays so we should make this monster:*/
    double* u = (double*)calloc(num * M_t, sizeof(double));

    /* boundary conditions */
    /* t = 0 */
    for(int i = 0; i < num; ++i)
        u[i*M_t + 0] = u0((i + start)*h + A);

    /* x = 0 */
    if(rank == 0)
        for(int i = 0; i < M_t; ++i)
            u[0*M_t + i] = u1(i*tau + t);
    
    /* first layer - left corner scheme with no data exchange */
    for(int i = 1; i < num; ++i)
        u[i*M_t + 1] = leftCorner(u[(i-1)*M_t + 0],  u[i*M_t + 0], f((i + start)*h + A, 0));
    
    if(rank != size - 1)
        MPI_Send(&u[(num-1)*M_t + 0], 1, MPI_DOUBLE, rank + 1, 1, MPI_COMM_WORLD);
    
    if(rank != 0)
    {
        double u_left = 0;
        MPI_Status status;
        MPI_Recv(&u_left, 1, MPI_DOUBLE, rank - 1, 1, MPI_COMM_WORLD, &status);
        u[0*M_t + 1] = leftCorner(u_left, u[0*M_t + 0], f(start*h + A, 0));
    }
    
    /* next layers - cross scheme */
    for(int i = 1; i < M_t - 1; ++i)
    {
        /* first dot */
        if(rank != size - 1)
            MPI_Send(&u[(num-1)*M_t + i], 1, MPI_DOUBLE, rank + 1, i, MPI_COMM_WORLD);

        if(rank != 0)
        {
            double u_left;
            MPI_Status status;
            MPI_Recv(&u_left, 1, MPI_DOUBLE, rank - 1, i, MPI_COMM_WORLD, &status);
            u[0*M_t + i+1] = cross(u_left, u[0*M_t + i-1], u[1*M_t + i], f(start*h + A, i*tau));
        }

        /* central dots */
        for(int j = 1; j < num - 1; j++)
            u[j*M_t + i+1] = cross(u[(j-1)*M_t + i], u[j*M_t + i-1], u[(j+1)*M_t + i], f((j + start)*h, i*tau));


        /* last dot */
        if(rank != 0)
            MPI_Send(&u[0*M_t + i], 1, MPI_DOUBLE, rank - 1, i + size, MPI_COMM_WORLD);

        if(rank != size - 1)
        {
            double u_right;
            MPI_Status status;
            MPI_Recv(&u_right, 1, MPI_DOUBLE, rank + 1, i + size, MPI_COMM_WORLD, &status);
            u[(num-1)*M_t + i+1] = cross(u[(num-2)*M_t + i], u[(num-1)*M_t + i - 1], u_right, f(end*h + A, i*tau));
        
        }
        else
            u[(num-1)*M_t + i+1] = leftCorner(u[(num-2)*M_t + i], u[(num-1)*M_t + i], f(end*h + A, i*tau));
    }
        
    double* res;
    int* counts;
    int* displs;

    if(rank == 0)
        res = (double*)calloc(N_x*M_t, sizeof(double));

    if(rank == 0)
    {
        counts = (int*)calloc(size, sizeof(int));
        displs = (int*)calloc(size, sizeof(int));

        for(int i = 0; i < size - 1; ++i)
        {
            counts[i] = num * M_t;
            displs[i] = num * i * M_t;
        }

        counts[size-1] = M_t*N_x - (size-1)*num*M_t;
        displs[size-1] = num*(size-1)*M_t;
    }
    
    /* gather all data to the one array on the first processor */
    MPI_Gatherv(u, num*M_t, MPI_DOUBLE, res, counts, displs, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if(rank == 0)
    {
        double time_end = MPI_Wtime();
        printf("Program time = %lg mks\n", (time_end - time_begin)*1e6);
        double max = -100;
        for(int j = 0; j < M_t; ++j)
            for(int i = 0; i < N_x; ++i)
            {
                double tmp = fabs(res[i*M_t + j] - real(i*h, j*tau));
                if(tmp > max)
                    max = tmp;
            }

        printf("error between real and counted: %lg\n", max);
        free(res);
        free(counts);
        free(displs);
    }
    free(u);

    MPI_Finalize();
    return 0;
}
