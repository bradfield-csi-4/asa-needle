#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main2();

int main(){
    main2();
    return 0;
}

int main2()
{
    pid_t childpid;
    int i, fib_sum=0, fib1=1, fib2=1, temp, status;

    int fd[2];
    int val = 0;

    // create pipe descriptors
    // pipe(fd);

    // childpid = fork();
    if(childpid == 0) {
        // close(fd[1]);
        // // read the data (blocking operation)
        // read(fd[0], &val, sizeof(val));

        // printf("Parent received value: %d\n", val);
        // // close the read-descriptor
        // close(fd[0]);
    } else {
        // writing only, no need for read-descriptor:
        // close(fd[0]);

        // for(i=1; i<=12; i++)
        // {
        //     temp = fib1;
        //     fib_sum = fib1+fib2;
        //     fib1 = fib_sum;
        //     fib2 = temp;
        // }

        // // send the value on the write-descriptor:
        // write(fd[1], &fib_sum, sizeof(fib_sum)); 
        // printf("Child send value: %d\n", fib_sum);

        // // close the write descriptor:
        // close(fd[1]);

        return fib_sum;
    }
}