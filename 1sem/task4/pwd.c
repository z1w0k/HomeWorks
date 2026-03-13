#include <unistd.h>
#include <string.h>

int main(int argc,char *argv[]) {
    char cwd[4096];

    if (argc > 1) {
        write(STDERR_FILENO, "pwd: too many arguments", 24);
        return 1;
    }

    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        write(STDOUT_FILENO, cwd, strlen(cwd));
        write(STDOUT_FILENO, "\n", 1);
        return 0;
    } else {
        write(STDERR_FILENO, "pwd: cannot get current directory\n", 34);
        return 1;
    }
}
