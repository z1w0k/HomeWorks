#include <unistd.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    char *buf = "Это один и тот же файл\n";

    if (argc != 3) {
        write(STDERR_FILENO, "Usage: mv <old_file><new_file>\n", 32);
        return 1;
    }

    if (strcmp(argv[1], argv[2]) == 0) {
        write(STDERR_FILENO, buf, strlen(buf));
        return 1;
    }

    if (rename(argv[1], argv[2]) == -1) {
        const char *error_msg = "mv: error\n";
        write(STDERR_FILENO, error_msg, 11);
        return 1;
    }

    return 0;
}
