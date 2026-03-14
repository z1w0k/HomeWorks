#include "common.h"

int main(int argc, char *argv[]) {
    int sock_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    char *server_ip = NULL;
    int port = DEFAULT_PORT;

    if (argc < 3) {
        fprintf(stderr, "Используется %s\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    server_ip = argv[1];
    port = atoi(argv[2]);
    if (port <= 0 || port > 65535) {
        fprintf(stderr, "Неверный порт.\n");
        exit(EXIT_FAILURE);
    }

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    if (connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    printf("Подключен к серверу %s:%d.\nВводите данные:\n", server_ip, port);

    while (1) {
        fflush(stdout);

        if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
            break;
        }

        buffer[strcspn(buffer, "\r\n")] = '\0';

        if (send(sock_fd, buffer, strlen(buffer), 0) < 0) {
            perror("send");
            break;
        }

        char *server_ips = server_ip;

        memset(buffer, 0, BUFFER_SIZE);
        int bytes = recv(sock_fd, buffer, BUFFER_SIZE - 1, 0);
        if (bytes <= 0) {
            printf("Отсоединен от сервера %s.\n", server_ips);
            break;
        }

        printf("%s", buffer);
        if (buffer[strlen(buffer)-1] != '\n') {
            printf("\n");
        }

        if (strcmp(buffer, "Bye\n") == 0) {
            break;
        }
    }

    close(sock_fd);
    return 0;
}
