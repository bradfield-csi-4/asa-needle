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

int main (){
    int c, i;
    i = 0;
    char input[100];
    printf("🤟 ");
	while ((c = getchar()) != EOF) {
        if (c == '\n') {
            input[i] = '\0';
            int result = handle_input(input);
            if (result < 0){
                break;
            }
            input[0] = '\0';
            i = 0;
            printf("🤟 ");
        } else {
            input[i] = c;
            i++;
        }
    }
    printf("\n😈😈😈😈 ROCK ON! 😈😈😈😈\n");
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