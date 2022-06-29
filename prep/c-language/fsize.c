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
void fsize(char *);

int main (int argc, char **argv)
{
	if (argc == 1)
		fsize(".");
	else
		while (--argc > 0)
			fsize(*++argv);
	return 0;
}

void dirwalk(char *, void (*fcn)(char *));

void fsize(char *name)
{
	struct stat stbuf;

	if (stat(name, &stbuf) == -1) {
		fprintf(stderr, "fsize: can't access %s\n", name);
		return;
	}
	if ((stbuf.st_mode & S_IFMT) == S_IFDIR)
		dirwalk(name, fsize);
	printf("%lld %s\n", stbuf.st_size, name);
}

#define MAX_PATH 1024
 
void dirwalk(char *dir, void (*fcn)(char *))
{
	char name[MAX_PATH];
	Dirent *dp;
	MY_DIR *dfd;
	printf("starting dir walk\n");
	if ((dfd = my_opendir(dir)) == NULL) { 
		fprintf(stderr, "dirwalk: can't open %s\n", dir);
		return;
	}
	printf("file descriptor: %d\n", dfd->fd);
	while ((dp = my_readdir(dfd)) != NULL) {
		if (strcmp(dp->name, ".") == 0
		 || strcmp(dp->name, "..") == 0)
			continue;
		if (strlen(dir) + strlen(dp->name) + 2 > sizeof(name))
			fprintf(stderr, "dirwalk: name %s/%s./too long\n",
				dir, dp->name);
		else {
			sprintf(name, "%s/%s", dir, dp->name);
			(*fcn)(name);
		}
	}
	my_closedir(dfd);
}

int fstat(int fd, struct stat *);

MY_DIR *my_opendir(char *dirname)
{
	int fd;
	struct stat stbuf;
	MY_DIR *dp;
	if ((fd = open(dirname, O_RDONLY, 0)) == -1
	 || fstat(fd, &stbuf) == -1
	 || (stbuf.st_mode & S_IFMT) != S_IFDIR
	 || (dp = (MY_DIR *) malloc(sizeof(MY_DIR))) == NULL)
		return NULL;
	dp->fd = fd;
	return dp;
}

void my_closedir(MY_DIR *dp)
{
	if (dp) {
		close(dp->fd);
		free(dp);
	}
}

Dirent *my_readdir
(MY_DIR *dp)
{
	struct direct dirbuf;
	static Dirent d;

	while (read(dp->fd, (char *) &dirbuf, sizeof(dirbuf))
					== sizeof(dirbuf)) {
		if (dirbuf.d_ino == 0)
			continue;
		d.ino = dirbuf.d_ino;
		strncpy(d.name, dirbuf.d_name, DIRSIZ);
		d.name[DIRSIZ] = '\0';
		return &d;
	}
	return NULL;
}

