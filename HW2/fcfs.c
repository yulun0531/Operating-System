#include <stdio.h>

// 定義進程結構
typedef struct Process {
    int pid; // 進程ID
    int arrivalTime; // 到達時間
    int burstTime; // 執行時間
} Process;

// 循環輪詢排程算法
void roundRobin(Process processes[], int n, int timeQuantum) {
    int remainingTime[n];
    for (int i = 0; i < n; i++) {
        remainingTime[i] = processes[i].burstTime;
    }
    
    int currentTime = 0;
    int completed = 0;
    while (completed < n) {
        for (int i = 0; i < n; i++) {
            if (remainingTime[i] > 0) {
                int executeTime = remainingTime[i] < timeQuantum ? remainingTime[i] : timeQuantum;
                currentTime += executeTime;
                remainingTime[i] -= executeTime;
                
                // 如果進程已經執行完畢
                if (remainingTime[i] == 0) {
                    completed++;
                    printf("Process %d finishes at time %d.\n", processes[i].pid, currentTime);
                } else {
                    printf("Process %d executes at time %d.\n", processes[i].pid, currentTime);
                }
            }
        }
    }
}

int main() {
    // 輸入進程數量和時間片大小
    int n, timeQuantum;
    printf("Enter the number of processes: ");
    scanf("%d", &n);
    printf("Enter the time quantum: ");
    scanf("%d", &timeQuantum);
    
    // 輸入進程信息
    Process processes[n];
    printf("Enter arrival time and burst time for each process:\n");
    for (int i = 0; i < n; i++) {
        processes[i].pid = i + 1;
        printf("Process %d: ", i + 1);
        scanf("%d %d", &processes[i].arrivalTime, &processes[i].burstTime);
    }
    
    // 使用循環輪詢算法排程進程
    printf("\nRound Robin Scheduling:\n");
    roundRobin(processes, n, timeQuantum);
    
    return 0;
}