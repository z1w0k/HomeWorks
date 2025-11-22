#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

#define BUFFER_SIZE 4096

int main(int argc, char *argv[]) {
    if (argc != 3) {
        const char *error = "Usage: cp <source><destination>";
        write(STDERR_FILENO, error, strlen(error));
        return 1;
    }

    int src_fd, dst_fd;
    ssize_t bytes_read, bytes_written;
    char buffer[BUFFER_SIZE];
    struct stat stat_buf;

    src_fd = open(argv[1], O_RDONLY);
    if (src_fd == -1) {
        const char *error = "cp: cannot open source file\n";
        write(STDERR_FILENO, error, strlen(error));
        return 1;
    }

    if (fstat(src_fd, &stat_buf) == -1) {
        const char *error = "cp: canot get file stats\n";
        write(STDERR_FILENO, error, strlen(error));
        return 1;
    }

    dst_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, stat_buf.st_mode);
    if(dst_fd == -1) {
        const char *error = "cp: cannot create destination file\n";
        write(STDERR_FILENO, error, strlen(error));
        return 1;
    }

    while ((bytes_read = read(src_fd, buffer, BUFFER_SIZE)) > 0) {
        bytes_written = write(dst_fd, buffer, bytes_read);
        if (bytes_written != bytes_read) {
            const char *error = "cp: write error\n";
            write(STDERR_FILENO, error, strlen(error));
            close(src_fd);
            close(dst_fd);
            return 1;
        }
    }

    if(bytes_read == -1) {
        const char *error = "cp: read error\n";
        write(STDERR_FILENO, error, strlen(error));
        close(src_fd);
        close(dst_fd);
        return 1;
    }

    close(src_fd);
    close(dst_fd);
    return 0;
}
