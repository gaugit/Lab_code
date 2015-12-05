#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#include<sys/param.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<errno.h>
#include<netdb.h>
#include<sys/fcntl.h>
#include<netinet/in.h>
#include<string.h>
#include<arpa/inet.h>
#include<time.h>
#include<unistd.h>
#include<sys/time.h>
#include<stddef.h>
#include<sys/stat.h>
#include <ifaddrs.h>
int c=0;

int popexc(char *cmd, char *buff);
int childprocess (int);

extern int errno;

void report_error(char *s)
{
 printf("error in %s, errno= %d\n", s, errno);
 exit(1);
}

int main(int argc, char *argv[])
{
 int socket1,asocket1,bsocket1,len;
 int n,pid;
 int i=1;
 struct sockaddr_in sa={0}, csa={0};
 socklen_t clien1;
 char hello[100]="Hello",closed[100];

 //IP_ADDR addr;
 char *strAddr;
 //Socket creating and opening
 socket1=socket(AF_INET,SOCK_STREAM,0);
 if (socket1<0)
 {
  report_error("opening socket");
 }
 else
 {
  printf("Socket opened\n");
 }

 sa.sin_family = AF_INET;
 sa.sin_addr.s_addr=INADDR_ANY;
 sa.sin_port =htons(IPPORT_USERRESERVED + getuid());
 //binding the socket address
 bsocket1=bind(socket1, (struct sockaddr *)&sa, sizeof(sa));

 if(bsocket1<0)
 {
  report_error("binding");
 }
 else
 {
  printf("Binding successfully done\n");
 }
 //listening for the clients
 listen(socket1,5);
 clien1=sizeof(csa);
 
 while(1)
 {
  asocket1=accept(socket1,(struct sockaddr *) &csa, &clien1);//accepting the clients
 
  if (asocket1<0)
  {
  report_error("Accepting client please wait!");
  }
  else
  {
  printf("Client accepted successfully!\n");
  }
 
  //sending the hello to client
  send(asocket1,hello,100,0);
  c=c+1;//Counting the clients connected
  printf("Total number of clients connected to this server today: %d\n",c);
  sprintf(closed,"%d",c);
  //Sending the client number to the client
  send(asocket1,closed,100,0);
  //forking process
  pid = fork();
 
  if (pid < 0)
  {
  error("ERROR on fork");
 
  }
  if (pid == 0)
  {
  close(socket1);//closing socket1
  childprocess (asocket1);//Calling the child process
  c--;
  exit(0);//exit after childprocess returns
  }
  else
  {
  close(asocket1);//closing asocket1
  }
 }
 close(socket1);//closing socket1
 return 0;
}



int childprocess (int rsocket)
{
 int n,i=0,portnum;
 struct sockaddr_in sa={0}, csa={0};
 socklen_t clilen;
 char buff[100],ok[2]="OK",cont[1024],hello[10]="Hello",path[50];
 char cmdlst[256]="\nSet of commands:\nls \ncd \nidle \nquit \n";

 while(1)
 {
  n = write(rsocket,cmdlst,strlen(cmdlst));//display cmdlist in client
  if (n < 0)
  {
   report_error("Writing the set of commands to the socket.\n");
  }
  printf("\nCMD list sent to client.\n");
  bzero(buff,100);
  n = read(rsocket,buff,100);//reading the option in buff

  if(strncmp(buff,"ls",2)==0)//For list command
  {
   memset(cont,'\0',1024);
   popexc("/bin/ls", cont);
   //printf("\noutput of LS: %s\n",cont);
   n= write(rsocket,cont,strlen(cont)); //writing reply to client
   if(n < 0)
   {
    report_error("writing to socket");
   }
   printf("\nLS reply sent to client.\n");
   //bzero(cont,sizeof(cont));
  }
  else if(strncmp(buff,"cd",2)==0)//For change directory command
  {
   strcpy(path,(buff+3));
   path[strlen(path)-1]='\0';
   printf("\nChanging dir to: %s\n",path);
   if(chdir(path) < 0)
   {
     printf("\nInvalid path received\n");
     n= write(rsocket,"Invalid path",12); //writing reply to client
     if(n < 0)
     {
      report_error("writing to socket");
     }
   }
   else
   {
     memset(cont,'\0',1024);
     popexc("/bin/pwd", cont);
     n= write(rsocket,cont,strlen(cont)); //writing reply to client
     if(n < 0)
     {
      report_error("writing to socket");
     }
     printf("\nDirectory change.\n");
   }  
  }
  else if(strncmp(buff,"idle",4)==0)//For quit command
  {/*
   n= write(rsocket,ok,2); //writing reply to client
   
   if(n < 0)
   {
    report_error("writing to socket");
   }*/
   printf("\nI'm doing nothing.\n");
  }
  else if(strncmp(buff,"quit",4)==0)//For quit command
  {
   n= write(rsocket,ok,2); //writing reply to client
   
   if(n < 0)
   {
    report_error("writing to socket");
   }
   printf("\nTerminating client on client.\n");
   close(rsocket);
   return 0;
  }
  else
  {
   n= write(rsocket,"Invalid Command",15); //writing reply to client
   
   if(n < 0)
   {
    report_error("writing to socket");
   }
  }
 }
 close(rsocket);//closing the rsocket
 exit(0); //exiting from the child process
}

int popexc(char *cmd, char *buff)
{

  FILE *fp;
  char path[1035];
  long lSize;
 
  fp = popen(cmd, "r");
  if (fp == NULL) {
    printf("Failed to run command\n" );
    exit(1);
  }

  /* Read the output a line at a time - output it. */
  while (fgets(path, sizeof(path)-1, fp) != NULL) {
    strcat(buff,path);
  }
/*
  fseek (fp , 0 , SEEK_END);
  lSize = ftell (fp);
  rewind (fp);
  fread (path,1,lSize,fp);
*/
  printf("\n%s", buff);
  /* close */
  pclose(fp);
  return 0;
}
