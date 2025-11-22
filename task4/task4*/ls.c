#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
 
#ifndef MAJOR
  #define MINOR_BITS 8
  #define MAJOR(dev) ((unsigned) dev >> MINOR_BITS)
  #define MINOR(dev) ((dev) & ((1 << MINOR_BITS) - 1))
#endif

int flag_R = 0;
int flag_l = 0;
int flag_g = 0;

// показать тип файла в первой позиции выходной строки 
void display_file_type ( int st_mode ) 
{                                   
    switch ( st_mode & S_IFMT )
    {
        case S_IFDIR:  putchar ( 'd' ); return;
        case S_IFCHR:  putchar ( 'c' ); return;
        case S_IFBLK:  putchar ( 'b' ); return;
        case S_IFREG:  putchar ( '-' ); return;
        case S_IFLNK:  putchar ( 'l' ); return;
        case S_IFSOCK: putchar ( 's' ); return;
    }
} 
 
// показать права доступа для владельца, группы и прочих пользователей, а также все спец.флаги 
void display_permission ( int st_mode )
{
  static const char xtbl[10] = "rwxrwxrwx";
  char     amode[10];
  int      i, j;
 
  for ( i = 0, j = ( 1 << 8 ); i < 9; i++, j >>= 1 )
    amode[i] = ( st_mode&j ) ? xtbl[i]: '-';
  if ( st_mode & S_ISUID )   amode[2]= 's';
  if ( st_mode & S_ISGID )   amode[5]= 's';
  if ( st_mode & S_ISVTX )   amode[8]= 't';
  amode[9]='\0';
  printf ( "%s ",amode );
}

// перечислить атрибуты одного файла
void long_list ( char * path_name )
{
  struct stat     statv;
  struct passwd  *pw_d;

  if ( lstat ( path_name, &statv ) )
  { 
    perror ( path_name ); 
    return;
  }

  if (flag_l) {
    display_file_type ( statv.st_mode );
    display_permission ( statv.st_mode );
    printf ( "%d ",statv.st_nlink );
    pw_d = getpwuid ( statv.st_uid );
    if ( pw_d != NULL )
      printf ( "%s ",pw_d->pw_name );
    else
      printf ( "%d ", statv.st_uid );
    
    if (flag_g) {
      printf ( "%d ", statv.st_gid );
    }

    if (( statv.st_mode & S_IFMT) == S_IFCHR  ||
        ( statv.st_mode & S_IFMT) == S_IFBLK)
      printf ( "%d, %d",MAJOR(statv.st_rdev), MINOR(statv.st_rdev) );
    else
      printf ( "%d", statv.st_size );
    
    printf ( " %s\n", path_name );
  } else {
    printf("%s\n", path_name);
  }
}

// рекурсивный обход каталога
void process_directory(char *path_name) {
    DIR *dir;
    struct dirent *entry;
    struct stat statv;
    char full_path[1024];
    
    dir = opendir(path_name);
    if (!dir) {
        perror(path_name);
        return;
    }
    
    printf("\n%s:\n", path_name);
    
    while ((entry = readdir(dir)) != NULL) {
        // Пропускаем . и ..
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
            
        // Создаем полный путь
        snprintf(full_path, sizeof(full_path), "%s/%s", path_name, entry->d_name);
        
        // Показываем файл
        long_list(full_path);
        
        // Проверяем, является ли он каталогом для рекурсии
        if (flag_R && lstat(full_path, &statv) == 0) {
            if ((statv.st_mode & S_IFMT) == S_IFDIR) {
                process_directory(full_path);
            }
        }
    }
    
    closedir(dir);
}
 
// главный цикл отображения атрибутов для каждого файла
int main ( int argc, char * argv[] )
{
  int i = 1;
  struct stat statv;

  // Обработка флагов
  while (i < argc && argv[i][0] == '-') {
    if (strcmp(argv[i], "-R") == 0) {
      flag_R = 1;
    } else if (strcmp(argv[i], "-l") == 0) {
      flag_l = 1;
    } else if (strcmp(argv[i], "-g") == 0) {
      flag_g = 1;
    } else if (strcmp(argv[i], "-Rl") == 0 || strcmp(argv[i], "-lR") == 0) {
      flag_R = flag_l = 1;
    } else if (strcmp(argv[i], "-Rg") == 0 || strcmp(argv[i], "-gR") == 0) {
      flag_R = flag_g = 1;
    } else if (strcmp(argv[i], "-lg") == 0 || strcmp(argv[i], "-gl") == 0) {
      flag_l = flag_g = 1;
    } else if (strcmp(argv[i], "-Rlg") == 0 || strcmp(argv[i], "-Rgl") == 0 ||
               strcmp(argv[i], "-lRg") == 0 || strcmp(argv[i], "-lgR") == 0 ||
               strcmp(argv[i], "-gRl") == 0 || strcmp(argv[i], "-glR") == 0) {
      flag_R = flag_l = flag_g = 1;
    }
    i++;
  }

  if ( i == argc ) {
    // Если нет аргументов, используем текущий каталог
    if (flag_R) {
        process_directory(".");
    } else {
        long_list(".");
    }
  } else {
    while ( i < argc ) {
      if (flag_R && lstat(argv[i], &statv) == 0 && (statv.st_mode & S_IFMT) == S_IFDIR) {
        process_directory(argv[i]);
      } else {
        long_list(argv[i]);
      }
      i++;
    }
  }
  return 0;
}
