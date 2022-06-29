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

enum _flags {
	_SIZE = 01,
	_HUMAN_READABLE = 02,
	// _ERROR = 010,
};

struct Readable_Name {
	int size;
	char letter;
};

void ls(char *, int);
void dirwalk(char *, void (*fcn)(char *)); 
void make_readable(int, struct Readable_Name *);

int main (int argc, char **argv)
{
	int i;
	char cur_char;
	int flags = 0;
	if (argc == 1){
		ls(".", 0);
	} else {
		while (--argc > 0) {
			argv++;
			if ((*argv)[0] == '-'){
				for (i=1; (cur_char = (*argv)[i]) != '\0'; i++){
					switch(cur_char){
						case 'l':
							flags = flags | _SIZE;
							break;
						case 'h':
							flags = flags | _HUMAN_READABLE;
							break;
					}
				}
			} else {
				ls(*argv, flags);
			}			
		}
	}
	return 0;
}




void ls(char *name, int flags)
{
	struct stat stbuf;
	char content_path[1000];
	if (stat(name, &stbuf) == -1) {
		fprintf(stderr, "ls: can't access %s\n", name);
		return;
	}
	if ((stbuf.st_mode & S_IFMT) != S_IFDIR) { 
		DIR *d;
		struct dirent *dir;
		struct stat *file_info;
		d = opendir(name);
		if (d) {
			printf("%s\n", name);
			while ((dir = readdir(d)) != NULL) {
				if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
					continue;
				if (strlen(name) + strlen(dir->d_name) + 2 > sizeof(content_path)){
					fprintf(stderr, "dirwalk: name %s/%s./too long\n", name, dir->d_name);
				}
				if (flags & _SIZE){
					file_info = (struct stat *)malloc(sizeof(struct stat));
					sprintf(content_path, "%s/%s", name, dir->d_name);
					stat(content_path, file_info);
					if (flags & _HUMAN_READABLE){
						struct Readable_Name Readable;
						make_readable(file_info->st_size, &Readable);
						printf("%d%c %s\n", Readable.size, Readable.letter, dir->d_name);
					} else {
						printf("%lld %s\n", file_info->st_size, dir->d_name);
					}
					
					free(file_info);
				} else {
					printf("%s\n", dir->d_name);
				}
			}
			closedir(d);
		}
	} else {
		printf("%s\n", name);
	}
}

void make_readable(int size, struct Readable_Name * ptr){
	char mag_char[] = "1KMGT";
	char *cur_mag = mag_char;
	int i =0;
	while(size > 1000){
		size = size / 1000;
		cur_mag += 1;
	}
	(*ptr).size = size;
	if (*cur_mag != '1'){
		(*ptr).letter = cur_mag[0];
	}
}