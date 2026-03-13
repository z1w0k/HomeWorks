#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int no_newline = 0;
    int i = 1;

    if (i < argc && strcmp(argv[i], "-n") == 0) {
        no_newline = 1;
        i++;
    }

    for(i = 1; i < argc; i++) {
        write(STDOUT_FILENO, argv[i], strlen(argv[i]));
        if (i < argc - 1) {
            write(STDOUT_FILENO, " ", 1);
        }
    }

    if (!no_newline) {
        write(STDOUT_FILENO, "\n", 1);
    }
    return 0;
}
