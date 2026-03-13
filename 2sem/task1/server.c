#include "common.h"

static int child_pids[BACKLOG];
static int child_count = 0;

void father_sig_handler(int sig) {
    if (sig == SIGINT || sig == SIGTERM) {
        for (int i = 0; i < child_count; i++) {
            if (child_pids[i] > 0) {
                kill(child_pids[i], SIGTERM);
            }
        }
        while (wait(NULL) > 0);
        exit(EXIT_SUCCESS);
    }
}

void child_sig_handler(int sig) {
    if (sig == SIGTERM) {
        exit(EXIT_SUCCESS);
    }
}

void handle_client(int client_fd) {
    char buffer[BUFFER_SIZE];
    int increment = 1;
    ssize_t bytes_read;

    signal(SIGTERM, child_sig_handler);

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        bytes_read = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_read <= 0) {
            break;
        }

        buffer[strcspn(buffer, "\r\n")] = '\0';

        if (strlen(buffer) == 0) {
            continue;
        }

        if (buffer[0] == '+') {
            int new_inc = atoi(buffer + 1);
            if (new_inc > 0) {
                increment = new_inc;
                send(client_fd, "Ok\n", 3, 0);
            } else {
                send(client_fd, "Ошибка: число должно быть положительным\n", 41, 0);
            }
        }
        else if (buffer[0] == '\\') {
            if (strcmp(buffer, "\\?") == 0) {
                char reply[BUFFER_SIZE];
                snprintf(reply, sizeof(reply), "%d\n", increment);
                send(client_fd, reply, strlen(reply), 0);
            }
            else if (strcmp(buffer, "\\-") == 0) {
                send(client_fd, "Bye\n", 4, 0);
                break;
            }
            else {
                send(client_fd, "Неизвестная команда\n", 21, 0);
            }
        }
        else {
            int num = atoi(buffer);
            num += increment;
            char reply[BUFFER_SIZE];
            snprintf(reply, sizeof(reply), "%d\n", num);
            send(client_fd, reply, strlen(reply), 0);
        }
    }

    close(client_fd);
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    int port = DEFAULT_PORT;

    if (argc > 1) {
        port = atoi(argv[1]);
        if (port <= 0 || port > 65535) {
            fprintf(stderr, "Неверный порт.\n");
            exit(EXIT_FAILURE);
        }
    }

    signal(SIGINT, father_sig_handler);
    signal(SIGTERM, father_sig_handler);
    signal(SIGCHLD, SIG_IGN);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, BACKLOG) < 0) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Сервер запущен на порту %d.", port);

    while (1) {
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        if (client_fd < 0) {
            perror("accept");
            continue;
        }

        printf("Новое соединение от %s:%d\n",
               inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            close(client_fd);
            continue;
        }

        if (pid == 0) {
            close(server_fd);
            handle_client(client_fd);
        } else {
            close(client_fd);
            if (child_count < BACKLOG) {
                child_pids[child_count++] = pid;
            }
        }
    }

    close(server_fd);
    return 0;
}
