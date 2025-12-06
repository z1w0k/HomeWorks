#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv) {
    char ch;
    ssize_t files;
    int fd;

    int isN = 0;
    int isNext = 0;
    int i = 1;
    int count = 1;

    for(i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-n") == 0) {
            isN = 1;
            break;
        }
    }


    i = 1;
    
    if (isN && argc > 1 && strcmp(argv[1], "-n") == 0) {
        i = 2;
    }

    for (; i < argc; i++) {
        fd = open(argv[i], O_RDONLY);
        
        while ((files = read(fd, &ch, 1)) > 0) {
            if (isN) {
                if (isNext) {
                    isNext = 0;
                    printf("\n%6d ", count);
                }

                if (ch == '\n') {
                    count ++;
                    isNext = 1;
                } else {
                    printf("%c", ch);
                }
            } else {
                printf("%c", ch);
            }
        }
        close(fd);
    }
    return 0;
}
