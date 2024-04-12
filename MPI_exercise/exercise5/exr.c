#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

// new array, each new element is the sum of the four elements of the original array, and the boundary neighbors are 0
// 1 2  3  4  5  6  7  8              new:   11  14  17  20  23  26  29  23
// 9 10 11 12 13 14 15 16                    28  40  44  48  52  56  60  47
// 17 18 19 20 21 22 23 24
// 25 26 27 28 29 30 31 32
// 33 34 35 36 37 38 39 40
// 41 42 43 44 45 46 47 48
// 49 50 51 52 53 54 55 56
// 57 58 59 60 61 62 63 64


int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    // size = 8
    const int N = 64; // size of the whole array
    int local_n = N / size; // each process handles local_n elements
    //2*local_n is for storing the values of the two rows of the upper and lower boundaries
    int *local_data = (int*)malloc((local_n + 2*local_n) * sizeof(int)); // local array with additional space for storing boundary values
    //每个本地算出来的新8个值
    int *result = (int*)malloc(local_n * sizeof(int));

    // 主进程初始化数组并分发
    if (rank == 0) {
        int *data = (int*)malloc(N * sizeof(int));
        // initialize the array
        // (1-64)
        for (int i = 0; i < N; i++) {
            data[i] = i + 1;
        }
        MPI_Scatter(data, local_n, MPI_INT, &local_data[local_n], local_n, MPI_INT, 0, MPI_COMM_WORLD);
        free(data);
    } else {
        // other processes receive data, NULL here means no data is sent
        MPI_Scatter(NULL, local_n, MPI_INT, &local_data[local_n], local_n, MPI_INT, 0, MPI_COMM_WORLD);
    }

    // 设置数组两端的边界条件
    if (rank == 0) {
        for (int i = 0; i < local_n; i++) {
            local_data[i] = 0; // 最上面的进程将上边界设置为0
        }
    }
    if (rank == size - 1) {
        for (int i = local_n + local_n; i < local_n + local_n + local_n; i++) {
            local_data[i] = 0; // 最下面的进程将下边界设置为0
        }
    }

    // Halo exchange
    int up_neighbor = (rank - 1 + size) % size;
    int down_neighbor = (rank + 1) % size;
    // 向上邻居发送和接收
    if (up_neighbor >= 0) {
        MPI_Sendrecv(&local_data[cols], cols, MPI_INT, up_neighbor, 0,
                     &local_data[cols + local_n], cols, MPI_INT, down_neighbor, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    // 向下邻居发送和接收
    if (down_neighbor < size) {
        MPI_Sendrecv(&local_data[cols+local_n-cols], cols, MPI_INT, down_neighbor, 0,
                     &local_data[0], cols, MPI_INT, up_neighbor, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    // calculate the new values
    for (int i = 0; i < local_n; i++) {
        //真正数据存储在local_data[local_n]~local_data[local_n+local_n-1]中
        int up = local_data[i];
        int down = local_data[i + 2*local_n];
        int left = local_data[i + local_n - 1];
        if (i==0) left = 0; // 左边界设置为0
        int right = local_data[i + local_n + 1];
        if (i==local_n-1) right = 0; // 右边界设置为0
        result[i] = up + down + left + right;
    }

    // collect the updated data
    if (rank == 0) {
        int *final_result = (int*)malloc(N * sizeof(int));
        // MPI_Gather函数从所有进程（包括根进程自己）收集数据。每个进程发送其result数组中的local_n个整数，
        // 这些数据被收集并按进程排名的顺序存储在根进程的final_result数组中。
        MPI_Gather(result, local_n, MPI_INT, final_result, local_n, MPI_INT, 0, MPI_COMM_WORLD);
        // 打印最终结果
        printf("Final result:\n");
        for (int i = 0; i < N; i++) {
            printf("%d ", final_result[i]);
        }
        printf("\n");
        free(final_result);
    } else {
        //非根进程也调用MPI_Gather函数，但它们在收集操作中不接收数据，
        //因此接收缓冲区参数设置为NULL。这些进程只负责发送自己的result数组中的数据。
        MPI_Gather(result, local_n, MPI_INT, NULL, local_n, MPI_INT, 0, MPI_COMM_WORLD);
    }

    free(local_data);
    free(result);
    MPI_Finalize();
    return 0;
}