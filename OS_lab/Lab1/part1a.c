#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(void)
{
  pid_t childPID;
  char ckey = 0;
  int status, i;
  for(i=0; i < 10; i++)
  {
    childPID = fork();
    if(childPID == 0)
    {
      printf("\n Child's Started\n");
      sleep(1);
      printf("\n Child Ends\n");
      exit(0);
    }
    else if(childPID > 0)
    {
      printf("\n Parent's process ID:%d\n", getpid());
      printf("\n Child's process ID:%d\n", childPID);

      wait(&status);
      printf("\nChild Terminated Parent waiting\n");
    }
    else
    {
      printf("\n Fork failed\n");
      exit(0);
    }
  }
  printf("\n Parent Ends\n");
}
