#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>

#define N 5               // количество философов
#define KEY 0x1234        // ключ для IPC семафоров

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

// Операции для семафоров
void down(int semid, int semnum) {
    struct sembuf op = {semnum, -1, 0};
    semop(semid, &op, 1);
}

void up(int semid, int semnum) {
    struct sembuf op = {semnum, 1, 0};
    semop(semid, &op, 1);
}

void philosopher(int i, int semid) {
    int left = i;               // вилка слева
    int right = (i + 1) % N;    // вилка справа

    while (1) {
        // Думаем
        printf("Философ %d думает...\n", i);
        sleep(rand() % 3 + 1);

        // Голоден — пытаемся взять вилки
        printf("Философ %d голоден...\n", i);

        // Для предотвращения deadlock меняем порядок взятия вилок
        if (i % 2 == 0) {
            down(semid, left);   // сначала левая
            down(semid, right);  // затем правая
        } else {
            down(semid, right);  // сначала правая
            down(semid, left);   // затем левая
        }

        // Едим
        printf("Философ %d ест...\n", i);
        sleep(rand() % 3 + 1);

        // Кладём вилки обратно
        up(semid, left);
        up(semid, right);

        printf("Философ %d поел и кладёт вилки...\n", i);
    }
}

int main() {
    int semid;
    union semun arg;
    unsigned short vals[N];

    // Создаём набор из N семафоров
    semid = semget(KEY, N, IPC_CREAT | 0666);
    if (semid < 0) {
        perror("semget");
        exit(1);
    }

    // Инициализируем все семафоры значением 1
    for (int i = 0; i < N; i++) {
        vals[i] = 1;
    }
    arg.array = vals;
    if (semctl(semid, 0, SETALL, arg) < 0) {
        perror("semctl SETALL");
        exit(1);
    }

    // Создаём процессы-философы
    for (int i = 0; i < N; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            // Дочерний процесс — философ
            philosopher(i, semid);
            exit(0);
        } else if (pid < 0) {
            perror("fork");
            exit(1);
        }
    }

    // Ждём немного и завершаем (в реальной программе — ожидание сигнала)
    sleep(30);
    semctl(semid, 0, IPC_RMID); // удаляем семафоры
    return 0;
}
