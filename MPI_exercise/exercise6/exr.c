#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 原数组，新数组每个新元素等于原数组中对应元素的上下左右四个元素之和，边界邻居为环形的！
// 1  2  3  4  5  6  7  8  9  10  11  12  13  14  15
// 16 17 18 19 20 21 22 23 24 25  26  27  28  29  30
// 31 32 33 34 35 36 37 38 39 40  41  42  43  44  45
// 46 47 48 49 50 51 52 53 54 55  56  57  58  59  60
// 61 62 63 64 65 66 67 68 69 70  71  72  73  74  75

// new 每个元素是原来这个位置的周围4个元素之和，边界邻居是环形的
// 94 83 87 91 95 99 103 107 111 115 119 123 127 131 120

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int cols = 15;  //15列
    const int rows = 5;
    const int N = rows * cols; // 整个板子的大小


    int basic_work = rows / size; // 每个进程处理的行数
    int remainder = rows % size; // 余数,假如size=8，余数最多是7行当rows=15时，不建议让最后一个线程处理8行，可以把这7行平均分给前7个线程
    //这样前面7个进程每个处理2行，最后一个进程处理1行

    // 为每个进程计算行数
    int rows_per_process = basic_work + (rank < remainder ? 1 : 0);
    // 为每个进程分配本地数组大小,一个进程要处理的元素个数
    // 最后一个进程的local_n可能会和其他进程不一样
    int local_n = rows_per_process * cols;

    // 分配本地数组
    // 本身的元素个数local_n + 额外的两行用于存储上下边界
    int *local_data = (int*)malloc((local_n + 2*cols) * sizeof(int)); // 额外空间用于存储边界值
    int *result = (int*)malloc(local_n * sizeof(int));
// 这里换个写法，直接在每个进程中初始化自己的数据
    // 这一段很有趣，是为了计算每个进程的起始行数
    int start_row = 0;
    for (int i = 0; i < rank; i++) {
        int rows_for_previous_process = basic_work + (i < remainder ? 1 : 0);
        start_row += rows_for_previous_process;
    }
// 这里计算每个进程的值要注意，每个进程的local_n可能不一样
    for (int i = 0; i < local_n; i++) {
        local_data[cols + i] = start_row * cols + i + 1;
    }

    // 设置数组两端的边界条件
    if (rank == 0) {
        for (int i = 0; i < cols; i++) {
            local_data[i] = 0;
        }
    }
    if (rank == size - 1) {
        for (int i = 0; i < cols; i++) {
            local_data[cols+ local_n + i] = 0;
        }
    }

    // 进行Halo exchange,环形的
    int up_neighbor = (rank - 1 + size) % size;
    int down_neighbor = (rank + 1) % size;

    // 向上邻居发送,并从下邻居接收
    MPI_Sendrecv(&local_data[cols], cols, MPI_INT, up_neighbor, 0,
                 &local_data[cols + local_n], cols, MPI_INT, down_neighbor, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    // 向下邻居发送并从上邻居接收
    MPI_Sendrecv(&local_data[cols+local_n-cols], cols, MPI_INT, down_neighbor, 0,
                     &local_data[0], cols, MPI_INT, up_neighbor, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    // 根据上下邻居还有自身左右相邻元素更新数据
    for (int i = 0; i < local_n; i++) {
        // local_data[0]到local_data[cols-1]用于存储上边界，local_data[cols+local_n]到local_data[cols+local_n+cols-1]用于存储下边界
        // 真正数据存储在local_data[cols]到local_data[cols+local_n-1]中
        // 真正数据的第i个元素的上下左右四个元素的和，计算完放到result中
        int up = local_data[i];
        int down = local_data[i + 2*cols];

        int index1 = i+cols-1;
        if (index1 % cols == cols-1) {
            index1 = index1 + cols;
        }
        int left = local_data[index1];
        int index = i+cols+1;
        if (index % cols == 0) {
            index = index - cols;
        }
        int right = local_data[index];
        result[i] = up + down + left + right;
    }

    // 不太能用gather，因为每个进程的数据量不一样
    if (rank == 0) {
        int *final_result = (int*)malloc(N * sizeof(int));
        // Copy the result from the master process first.
        memcpy(final_result, result, local_n * sizeof(int));
        int offset = local_n; // The offset for the next data block in the final array.

        // Now receive results from other processes.
        for (int i = 1; i < size; i++) {
            int rows_for_this_process = basic_work + (i < remainder ? 1 : 0);
            int count = rows_for_this_process * cols; // Calculate how much data to receive.
            MPI_Recv(final_result + offset, count, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            offset += count;
        }

        // Print the final result.
        printf("Final result:\n");
        for (int i = 0; i < N; i++) {
            printf("%d ", final_result[i]);
            if ((i + 1) % cols == 0) {
                printf("\n");
            }
        }
        printf("\n");

        free(final_result);
    } else {
        // Send the calculated result to process 0.
        MPI_Send(result, local_n, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    free(local_data);
    free(result);
    MPI_Finalize();
    return 0;
}


//    if (rank == 0) {
//        int *data = (int*)malloc(N * sizeof(int));
//        // 初始化数组
//        for (int i = 0; i < N; i++) {
//            data[i] = i + 1;
//        }
//
//        // 主进程分配自己的段
//        // 放到local_data 偏移cols后面，前面的（0-cols）用来放上边界的
//        memcpy(local_data + cols, data, local_n * sizeof(int));
//
//        // 发送给其他进程
//        int offset = local_n;
//        for (int i = 1; i < size; i++) {
//            int rows_for_this_process = basic_work + (i < remainder ? 1 : 0);
//            int count = rows_for_this_process * cols;
//            MPI_Send(data + offset, count, MPI_INT, i, 0, MPI_COMM_WORLD);
//            offset += count;
//        }
//        free(data);
//    } else {
//        // 非主进程接收数据
//        MPI_Recv(local_data + cols, local_n, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//    }