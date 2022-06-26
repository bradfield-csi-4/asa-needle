#define NAME_MAX 14 /* longest filename component */
#include <stdio.h>
#include <sys/stat.h>

typedef struct { /* portable directory entry */
    long ino; /* inode number */
    char name[NAME_MAX+1]; /* name + '\0' terminator */
} Dirent;

typedef struct { /* minimal DIR: no buffering, etc. */
    int fd; /* file descriptor for the directory */
    Dirent d; /* the directory entry */
} MY_DIR;


MY_DIR *my_opendir(char *dirname);
Dirent *my_readdir(MY_DIR *dfd);
void my_closedir(MY_DIR *dfd);


// #define S_IFMT    0160000  /* type of file: */
// #define S_IFDIR   0040000  /* directory */
// #define S_IFCHR   0020000  /* character special */
// #define S_IFBLK   0060000  /* block special */
// #define S_IFREG   0010000  /* regular */
/* ... */

// struct stat /* inode information returned by stat */
// {
//     dev_t st_dev;      /* device of inode */
//     ino_t st_ino;      /* inode number */
//     short st_mode;     /* mode bits */
//     short st_nlink;    /* number of links to file */
//     short st_uid;      /* owners user id */
//     short st_gid;      /* owners group id */
//     dev_t st_rdev;     /* for special files */
//     off_t  st_size;     /* file size in characters */
//     time_t my_st_atime;   /* time last accessed */
//     time_t my_st_mtime;   /* time last modified */
//     time_t my_st_ctime;    /* time originally created */
// };


// char *name;
// struct stat stbuf;
// int stat(char *, struct stat *);
