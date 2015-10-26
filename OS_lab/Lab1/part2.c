#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  pid_t childPID;
  int i;
  int status;
  
  if (argc < 2)
  {
	  printf("\n Please enter argument\n");
	  exit(0);
  }
  for(i=1; i<=2*atoi(argv[1]); i++)
  {
    childPID = fork();
    if(childPID == 0)
    {
      sleep(1);                                        //sleep in child part
      exit(0);
    }
    else if(childPID > 0)
    {
      printf("\n I'm process %d ID:%d\n",i,childPID);
      wait(&status);                                  //wait for child to terminate
    }
    else
    {
      printf("\n Fork failed\n");
      exit(0);
    }
  }
  printf("\n Parent Ends\n");
}
