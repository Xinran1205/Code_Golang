#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int N =19;
    double *A = malloc(N*sizeof(double));
    double *B = malloc(N*sizeof(double));
    double *C = malloc(N*sizeof(double));

    // init
    for (int i=0; i<N; i++) {
        A[i] = 0.0;
        B[i] = 1.0;
        C[i] = 2.0;
    }

    // 这么写很好，即使N小于工作进程数，也不会出现问题
    int workingProcessSize = size - 1;
    int basic_work = N/workingProcessSize;
    int remainder = N%workingProcessSize;
    // 这个判断是为了防止只有一个进程的时候，不需要分配工作
    if (size >1){
        if (rank != 0) {
            // 这里使用把余数平均分配给每个进程
            // 因为余数一定是小于等于workingProcessSize的，所以只需要在前remainder个进程多分配一个work
            int work_per_process = basic_work + (rank <= remainder ? 1 : 0);
            int start = (rank-1)*basic_work + (rank <= remainder ? rank-1 : remainder);
            int end = start + work_per_process;
            for (int i=start; i<end; i++) {
                A[i] = B[i] + C[i];
            }
            printf("Process %d: start = %d, end = %d\n", rank, start, end);
            MPI_Send(A+start, work_per_process, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        } else {
            for (int i = 1; i <= workingProcessSize; i++) {
                int recv_work_per_process = basic_work + (i <= remainder ? 1 : 0);
                int recv_start = (i - 1) * basic_work + (i - 1 < remainder ? i - 1 : remainder);

                //这里收到的数据会直接覆盖A数组中指定的一段位置，这个A数组是当前进程的A数组，每个进程所有变量都是独立的
                MPI_Recv(A + recv_start, recv_work_per_process, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }

            for (int i = 0; i < N; i++) {
                printf("A[%d] = %f\n", i, A[i]);
            }
        }
    }else{
        // If there's only one process, do everything in root
        for (int i=0; i<N; i++) {
            A[i] = B[i] + C[i];
            printf("A[%d] = %f\n", i, A[i]);
        }
    }

    free(A);
    free(B);
    free(C);

    MPI_Finalize();
    return 0;
}