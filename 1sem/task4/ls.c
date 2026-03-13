#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <unistd.h>

#define MAX_DEPTH 100

void output(int isR, int isG, int isL, const char *path, int depth);
void longList(int isG, const char *fullpath, const char *filename);
void display_file_type(int st_mode);
void display_permission(int st_mode);

int main(int argc, char **argv) {
    int isR = 0;
    int isG = 0;
    int isL = 0;
    int i = 1;

    if ((argc > 1) && (argv[1][0] == '-') && (strlen(argv[1]) >= 2)) {
        for (int j = 1; j < strlen(argv[1]); j++) {
            if (argv[1][j] == 'R') {
                isR = 1;
            } else if (argv[1][j] == 'l') {
                isL = 1;
            } else if (argv[1][j] == 'G') {
                isG = 1;
            } else {
                fprintf(stderr, "Error: unknown arguments!");
                exit(-1);
            }
        }
        i++;
    }

    if (argc == i) {
        output(isR, isG, isL, ".", 0);
    } else {
        for (; i < argc; i++) {
            output(isR, isG, isL, argv[i], 0);
        }
    }
    
    return 0;
}

void output(int isR, int isG, int isL, const char *path, int depth) {
    DIR *dir;
    struct dirent *entry;
    
    if (depth > MAX_DEPTH) {
        fprintf(stderr, "Max recursion depth exceeded for: %s\n", path);
        return;
    }
    
    dir = opendir(path);
    if (dir == NULL) {
        perror("opendir");
        return;
    }
    
    printf("%s:\n", path);
    
    char **filenames = NULL;
    int *dir_flags = NULL;
    int count = 0;
    int capacity = 100;
    
    filenames = malloc(sizeof(char *) * capacity);
    dir_flags = malloc(sizeof(int) * capacity);
    if (!filenames || !dir_flags) {
        perror("malloc");
        free(filenames);
        free(dir_flags);
        closedir(dir);
        return;
    }
    
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] != '.') {
            char *name_copy = strdup(entry->d_name);
            if (!name_copy) {
                perror("strdup");
                continue;
            }
            
            if (count >= capacity) {
                capacity *= 2;
                char **new_filenames = realloc(filenames, sizeof(char *) * capacity);
                int *new_dir_flags = realloc(dir_flags, sizeof(int) * capacity);
                
                if (!new_filenames || !new_dir_flags) {
                    perror("realloc");
                    free(name_copy);
                    for (int k = 0; k < count; k++) {
                        free(filenames[k]);
                    }
                    free(filenames);
                    free(dir_flags);
                    closedir(dir);
                    return;
                }
                
                filenames = new_filenames;
                dir_flags = new_dir_flags;
            }
            
            filenames[count] = name_copy;
            dir_flags[count] = (entry->d_type == DT_DIR);
            count++;
        }
    }
    
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (strcmp(filenames[j], filenames[j+1]) > 0) {
                char *temp_name = filenames[j];
                filenames[j] = filenames[j+1];
                filenames[j+1] = temp_name;
                
                int temp_flag = dir_flags[j];
                dir_flags[j] = dir_flags[j+1];
                dir_flags[j+1] = temp_flag;
            }
        }
    }
    
    for (int i = 0; i < count; i++) {
        char newpath[PATH_MAX];
        snprintf(newpath, sizeof(newpath), "%s/%s", path, filenames[i]);
        
        if (isL) {
            longList(isG, newpath, filenames[i]);
        } else {
            if (dir_flags[i]) {
                printf("%s/\t", filenames[i]);
            } else {
                printf("%s\t", filenames[i]);
            }
        }
    }
    
    if (!isL && count > 0) printf("\n");
    
    if (isR) {
        for (int i = 0; i < count; i++) {
            if (dir_flags[i]) {
                char newpath[PATH_MAX];
                snprintf(newpath, sizeof(newpath), "%s/%s", path, filenames[i]);
                printf("\n");
                output(isR, isG, isL, newpath, depth + 1);
            }
        }
    }
    
    for (int i = 0; i < count; i++) {
        free(filenames[i]);
    }
    free(filenames);
    free(dir_flags);
    
    closedir(dir);
}

void display_file_type(int st_mode) {
    switch (st_mode & S_IFMT) {
        case S_IFDIR:  putchar('d'); return;
        case S_IFCHR:  putchar('c'); return;
        case S_IFBLK:  putchar('b'); return;
        case S_IFREG:  putchar('-'); return;
        case S_IFLNK:  putchar('l'); return;
        case S_IFSOCK: putchar('s'); return;
        case S_IFIFO:  putchar('p'); return;
        default:       putchar('?'); return;
    }
}

void display_permission(int st_mode) {
    static const char xtbl[10] = "rwxrwxrwx";
    char amode[10];
    int i, j;

    for (i = 0, j = (1 << 8); i < 9; i++, j >>= 1)
        amode[i] = (st_mode & j) ? xtbl[i] : '-';

    if (st_mode & S_ISUID) amode[2] = (st_mode & S_IXUSR) ? 's' : 'S';
    if (st_mode & S_ISGID) amode[5] = (st_mode & S_IXGRP) ? 's' : 'S';
    if (st_mode & S_ISVTX) amode[8] = (st_mode & S_IXOTH) ? 't' : 'T';
    
    amode[9] = '\0';
    printf("%s ", amode);
}

void longList(int isG, const char *fullpath, const char *filename) {
    struct stat statv;
    struct passwd *pw_d;
    struct group *gr_d;
    char timebuf[256];
    char linkbuf[PATH_MAX];
    ssize_t len;
    
    if (lstat(fullpath, &statv)) {
        perror(fullpath);
        return;
    }
    
    display_file_type(statv.st_mode);
    display_permission(statv.st_mode);
    
    printf("%ld ", statv.st_nlink);
    
    pw_d = getpwuid(statv.st_uid);
    if (pw_d != NULL) {
        printf("%s ", pw_d->pw_name);
    } else {
        printf("%d ", statv.st_uid);
    }
    
    if (!isG) {
        gr_d = getgrgid(statv.st_gid);
        if (gr_d != NULL) {
            printf("%s ", gr_d->gr_name);
        } else {
            printf("%d ", statv.st_gid);
        }
    }
    
    if ((statv.st_mode & S_IFMT) == S_IFCHR ||
        (statv.st_mode & S_IFMT) == S_IFBLK) {
        printf("%4d,%4d ", major(statv.st_rdev), minor(statv.st_rdev));
    } else {
        printf("%8ld ", statv.st_size);
    }
    
    struct tm *tm_info = localtime(&statv.st_mtime);
    if (tm_info != NULL) {
        strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", tm_info);
        printf("%s ", timebuf);
    }
    
    printf("%s", filename);
    
    if ((statv.st_mode & S_IFMT) == S_IFLNK) {
        len = readlink(fullpath, linkbuf, sizeof(linkbuf) - 1);
        if (len != -1) {
            linkbuf[len] = '\0';
            printf(" -> %s", linkbuf);
        }
    }
    
    printf("\n");
}
