#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>

#define N 5     
#define KEY 0x1234  

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

void down(int semid, int semnum) {
    struct sembuf op = {semnum, -1, 0};
    semop(semid, &op, 1);
}

void up(int semid, int semnum) {
    struct sembuf op = {semnum, 1, 0};
    semop(semid, &op, 1);
}

void philosopher(int i, int semid) {
    int left = i;
    int right = (i + 1) % N;

    while (1) {
        printf("Философ %d думает...\n", i);
        sleep(rand() % 3 + 1);

        printf("Философ %d голоден...\n", i);

        if (i % 2 == 0) {
            down(semid, left);
            down(semid, right);
        } else {
            down(semid, right); 
            down(semid, left);
        }

        printf("Философ %d ест...\n", i);
        sleep(rand() % 3 + 1);

        up(semid, left);
        up(semid, right);

        printf("Философ %d поел и кладёт вилки...\n", i);
    }
}

int main() {
    int semid;
    union semun arg;
    unsigned short vals[N];

    semid = semget(KEY, N, IPC_CREAT | 0666);
    if (semid < 0) {
        perror("semget");
        exit(1);
    }

    for (int i = 0; i < N; i++) {
        vals[i] = 1;
    }
    arg.array = vals;
    if (semctl(semid, 0, SETALL, arg) < 0) {
        perror("semctl SETALL");
        exit(1);
    }

    for (int i = 0; i < N; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            philosopher(i, semid);
            exit(0);
        } else if (pid < 0) {
            perror("fork");
            exit(1);
        }
    }

    sleep(30);
    semctl(semid, 0, IPC_RMID);
    return 0;
}
