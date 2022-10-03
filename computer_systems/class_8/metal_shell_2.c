#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/syscall.h>
#include <pwd.h> 
#include <sched.h>

int handle_input(char input[100]);

#ifndef MAX_BUF
#define MAX_BUF 200
#endif

int main() {

   FILE *fp;
   char buff[255];
   int c;

   fp = fopen("io.txt", "r+");
   fprintf(fp, "This is testing for fprintf...\n");
   while((c = fgetc(fp)) != EOF){ 
        putchar(c);
   };
   fclose(fp);
}

int handle_input(char input[100]){
    char *token;
    token = strtok(input, " ");
    int cmp_result;

    cmp_result = strcmp(token, "whoami");
    if (cmp_result == 0){ 
        token = strtok(NULL, " ");
        if (token != NULL) {
            printf("usage: whoami\n");
            return 1;
        }
        int id;
        struct passwd *pwd_ptr;
        id = geteuid();
        pwd_ptr = getpwuid(id);
        printf("%s\n", (*pwd_ptr).pw_name);
        return 1;
    }

    cmp_result = strcmp(input, "echo");
    if (cmp_result == 0){ 
        token = strtok(NULL, " ");
        while (token != NULL) {
            printf("%s ", token);
            token = strtok(NULL, " ");
        }
        printf("\n");
        return 1;
    }

    cmp_result = strcmp(input, "pwd");
    if (cmp_result == 0){ 
        token = strtok(NULL, " ");
        if (token != NULL) {
            printf("pwd: too many arguments\n");
            return 1;
        }
        char working_dir[MAX_BUF];
        getcwd(working_dir, MAX_BUF);
        printf("%s\n", working_dir);
        return 1;
    }

    cmp_result = strcmp(input, "exit");
    if (cmp_result == 0){ 
        return -1;
    }
    printf("😭 %s: command not found\n", input);
    return 1;
}