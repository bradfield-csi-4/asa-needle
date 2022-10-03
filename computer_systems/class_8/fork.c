#include <stdio.h>
#include <unistd.h>


int forkexample(char c[])
{
    printf("char: %s\n", c);
    if (fork() == 0) {
        sleep(2);
        printf("Child  process\n");
        return 0;
    } else {
        sleep(2);
        printf("Parent process\n");
        return 1;
    }
}

int main (){
    int c, i;
    i = 0;
    char input[100];
    printf("🤟 ");
	while ((c = getchar()) != EOF) {
        if (c == '\n') {
            input[i] = '\0';
            int result = forkexample(input);
            if (result == 0){
                printf("exiting child process\n");
                return 0;
            }
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