#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv) {
    ssize_t str1;
    ssize_t str2;
    char ch1, ch2;

    int fd1, fd2;

    struct stat s1, s2;

    fd1 = open(argv[1], O_RDONLY);
    fd2 = open(argv[2], O_RDONLY);

    int lineN = 1;
    int charN = 1;


    while (((str1 = read(fd1, &ch1, 1)) > 0) && ((str2 = read(fd2, &ch2,1)) > 0)) {
        if (ch1 == '\n') {
            charN = 1;
            lineN++;
        } else {
            charN++;
        }

        if (ch1 != ch2) {
            printf("%s differs from %s: line %d char %d\n", argv[1], argv[2], lineN, charN);
            return 0;
        }
    }

        if (stat(argv[1], &s1) == -1 || stat(argv[2], &s2) == -1) {
            fprintf(stderr, "Can't get file status");
            close(fd1);
            close(fd2);
            return 0;
        }

        if (s1.st_size < s2.st_size || s2.st_size < s1.st_size) {
            printf("%s differs from %s: files have different sizes\n");
            close(fd1);
            close(fd2);
            return 0;
        }

    printf("%s is equal to %s\n", argv[1], argv[2]);
    close(fd1);
    close(fd2);
    return 0;
}
