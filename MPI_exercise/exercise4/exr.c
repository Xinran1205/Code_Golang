#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

// 新数组，每个新元素等于原数组中对应元素的左右两个元素之和，边界邻居为0
// 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16
// 2 4 6 8 10 12 14 16 18 20 22 24 26 28 30 15

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int N = 16; // 整个数组的大小
    int local_n = N / size; // 每个进程处理的数组元素数，size=8,local_n=2
    //这里额外加2一个是为了存储左边界，一个是为了存储右边界，可以理解一个本地进程大小为4
    int *local_data = (int*)malloc((local_n + 2) * sizeof(int)); // 分配额外空间用于存储邻居的值
    //每个本地算出来2个值
    int *result = (int*)malloc(local_n * sizeof(int));

    // 主进程初始化数组并分发
    if (rank == 0) {
        int *data = (int*)malloc(N * sizeof(int));
        // 初始化数组
        //(1-16)
        for (int i = 0; i < N; i++) {
            data[i] = i + 1; // 简单示例
        }
        // 分发数据
        // 将16个值分发给8个进程包括自己，每个进程分到2个值，值存储在每个进程的local_data[1]和local_data[2]中
        // scatter必须要保证每个进程分到的数据个数是一样的
        MPI_Scatter(data, local_n, MPI_INT, &local_data[1], local_n, MPI_INT, 0, MPI_COMM_WORLD);
        free(data);
    } else {
        // 其他进程接收数据，这里的NULL表示不发送数据
        MPI_Scatter(NULL, local_n, MPI_INT, &local_data[1], local_n, MPI_INT, 0, MPI_COMM_WORLD);
    }

    // 设置数组两端的边界条件
    local_data[0] = (rank == 0) ? 0 : local_data[1]; // 最左边的进程将左边界设置为0
    local_data[local_n + 1] = (rank == size - 1) ? 0 : local_data[local_n]; // 最右边的进程将右边界设置为0

    // 进行Halo exchange
    int left_neighbor = rank - 1;
    int right_neighbor = rank + 1;
    // 向左邻居发送和接收
    // 这里当0号进程时，left_neighbor=-1，所以不会发送和接收，直接到下一步，可以防止deadlock
    if (left_neighbor >= 0) {
        MPI_Sendrecv(&local_data[1], 1, MPI_INT, left_neighbor, 0,
                     &local_data[0], 1, MPI_INT, left_neighbor, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    // 向右邻居发送和接收
    if (right_neighbor < size) {
        MPI_Sendrecv(&local_data[local_n], 1, MPI_INT, right_neighbor, 0,
                     &local_data[local_n + 1], 1, MPI_INT, right_neighbor, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    // 根据左右邻居更新数据
    for (int i = 1; i <= local_n; i++) {
        result[i - 1] = local_data[i - 1] + local_data[i + 1];
    }

    // 收集更新后的数据
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