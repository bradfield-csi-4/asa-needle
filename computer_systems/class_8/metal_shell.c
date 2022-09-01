#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/syscall.h>
#include <pwd.h> 
#include <sched.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>

int handle_input(char *token, char input[100]);
int handle_process(char input[100]);
void sighandler(int dummy);

#ifndef MAX_BUF
#define MAX_BUF 200
#endif

// add a tokenize function

int main (){
    int c, i;
    i = 0;
    char input[100];
    printf("🤟 ");
	while ((c = getchar()) != EOF) {
        if (c == '\n') {
            input[i] = '\0';
            signal(SIGINT, sighandler);
            int result = handle_process(input);
            signal(SIGINT, SIG_DFL);

            if (result < 0){
                printf("ending loop due to ctrl-c\n");
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
    printf("c was %d\n", c == EOF);
    printf("\n😈😈😈😈 ROCK ON! 😈😈😈😈\n");
}

void sighandler(int dummy){
    printf("\n");
}

int handle_process(char input[100]){
    int status = 0;
    pid_t wpid;

    int fd[2];

    char *token;
    token = strtok(input, " ");
    int cmp_result;

    cmp_result = strcmp(input, "exit");
    if (cmp_result == 0){ 
        return -1;
    }

    struct sigaction *myaction;

    if (fork() == 0){
        // CHILD
        int result = handle_input(token, input);
        exit(0);
    } else {
        // PARENT
        while ((wpid = wait(&status)) > 0);
        return 1;
    }
}

int handle_input(char *token, char input[100]){

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

    cmp_result = strcmp(input, "sleep");
    if (cmp_result == 0){ 
        token = strtok(NULL, " ");
        if (token == NULL) {
            printf("usage: sleep seconds");
            return 1;
        } else {
            char *seconds = token;
            token = strtok(NULL, " ");
            if (token != NULL) {
                printf("usage: sleep seconds");
                return 1;
            } else {
                sleep(atoi(seconds));
                return 1;
            }
        } 
    }

    printf("😭 %s: command not found\n", input);
    return 1;
}