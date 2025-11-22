#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int symbolic = 0;
    int i = 1;

    if (argc > 1 && strcmp(argv[1], "-s") == 0) {
        symbolic = 1;
        i++;
    }

    if (argc - i != 2) {
        write(STDERR_FILENO, "Usage: ln [-s]<original><new_link>\n",38);
        return 1;
    }

    char *original = argv[i];
    char *new_link = argv[i + 1];

    if (symbolic) {
        if (symlink(original, new_link) == -1) {
            write(STDERR_FILENO, "ln: cannot creat symbolic link\n", 32);
            return 1;
        }
    } else {
        if (link(original,new_link) == -1) {
            write(STDERR_FILENO, "ln: cannot create hard link\n", 28);
            return 1;
        }
    }

    return 0;
}
