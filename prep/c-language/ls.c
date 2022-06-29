#include <stdio.h>
#include <string.h>
#include <unistd.h> 
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/dir.h>
#include "dirent.h"
#include <stdlib.h>

#undef DIRSIZ
#define DIRSIZ 14

void ls(char *);
void dirwalk(char *, void (*fcn)(char *));

// int main(void) {
//   DIR *d;
//   struct dirent *dir;
//   d = opendir(".");
//   if (d) {
//     while ((dir = readdir(d)) != NULL) {
//       printf("%s\n", dir->d_name);
//     }
//     closedir(d);
//   }
//   return(0);
// }

int main (int argc, char **argv)
{
	if (argc == 1){
		ls(".");
	} else {
		while (--argc > 0) {
			++argv;
			ls(*argv);
		}
	}
	return 0;
}


void ls(char *name)
{
	struct stat stbuf;
	char content_path[1000];
	if (stat(name, &stbuf) == -1) {
		fprintf(stderr, "ls: can't access %s\n", name);
		return;
	}
	if ((stbuf.st_mode & S_IFMT) == S_IFDIR) { 
		DIR *d;
		struct dirent *dir;
		struct stat *file_info;
		d = opendir(name);
		if (d) {
			printf("%s\n", name);
			while ((dir = readdir(d)) != NULL) {
				if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
					continue;
				if (strlen(name) + strlen(dir->d_name) + 2 > sizeof(content_path))
					fprintf(stderr, "dirwalk: name %s/%s./too long\n", name, dir->d_name);
				else {
					file_info = (struct stat *)malloc(sizeof(struct stat));
					sprintf(content_path, "%s/%s", name, dir->d_name);
					stat(content_path, file_info);
					printf("%lld %s\n", file_info->st_size, dir->d_name);
					free(file_info);
				}
			}
			closedir(d);
		}
	} else {
		printf("%s\n", name);
	}
}


