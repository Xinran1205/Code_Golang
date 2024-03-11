#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>


//一个典型的场景是环形通信，其中每个进程将数据发送给下一个进程，并从上一个进程接收数据。假设有N个进程形成一个环形，进程编号从0到N-1。
//编写一个MPI程序，实现上述的环形数据交换逻辑。每个进程将包含一个整数值，这个值初始化为其进程编号。
// 然后，每个进程使用MPI_Sendrecv函数，将其值发送给下一个进程，同时从上一个进程接收一个值。
int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int send_data = rank;
    int recv_data;
    // MPI_Sendrecv是一个阻塞操作，它在发送和接收操作都完成之前不会返回
    MPI_Sendrecv(&send_data, 1, MPI_INT, (rank + 1) % size, 0, &recv_data, 1, MPI_INT, (rank - 1 + size) % size, 0,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("Process %d received %d from process %d\n", rank, recv_data, (rank - 1 + size) % size);

    MPI_Finalize();
    return 0;
}