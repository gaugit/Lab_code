#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>

int var_glb; /* A global variable*/

int main(void)
{
    pid_t childPID,childPID2,childPID3;
    int var_lcl = 0;

    childPID = fork();
    printf("\n Child's process ID:%d\n", childPID);
    if(childPID >= 0) // fork was successful
    {
        if(childPID == 0) // child process
        {
            childPID2 = fork();
            if(childPID2 > 0) // fork was successful
            {
             printf("\nProcess 2 : ID: %d\n", childPID2);
            }
            else if(childPID2 == 0)
            {
             childPID3 = fork();
             if(childPID3 > 0) // fork was successful
             {
              printf("\nProcess 3 : ID: %d\n", childPID3);
             }
            }
        }
    }
    else // fork failed
    {
        printf("\n Fork failed, quitting!!!!!!\n");
        return 1;
    }

    return 0;
}
