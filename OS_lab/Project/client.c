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
#include<dirent.h>
#include<sys/stat.h>
extern int errno;

/* Function to report error */
void report_error(char *s)
{
 printf("Error in %s, errno=%d\n", s, errno);
 exit(1);
}
/* Main Function */
int main(int argc,char *argv[])
{
 int socket1, n,zsocket1;
 int i=0;
 
 struct sockaddr_in sa1={0},sa2={0};
 struct hostent *server;
 char buffer[256], buff[100], hello[100], closed[100];

 int sa1_l=sizeof(sa1);
 /* Socket creation */
 socket1=socket(AF_INET,SOCK_STREAM,0);

 if (socket1<0)
 {
  report_error("Opening socket");
 }
 else
 {
  printf("Socket opened successfully and ready to use.\n");
 }
 
 if (argc < 2)
 {
  printf("\n Please enter argument\n");
  exit(0);
 }
 server=gethostbyname(argv[1]);
 
 if (server < 0)
 {
  report_error("server name");
  exit(1);
 }

 sa1.sin_family =AF_INET;
 bcopy((char *)server->h_addr,(char *)&sa1.sin_addr.s_addr, server->h_length);
 sa1.sin_port=htons(IPPORT_USERRESERVED + getuid());
 /* Connection establishment */
 zsocket1=connect(socket1,(struct sockaddr *) &sa1,sizeof(sa1));

 if(zsocket1 < 0)
 {
  report_error("Connecting with the server");
 }
 else
 {
  printf("Connection establised successfully with the server.\n\n");
 }

 recv(socket1,hello,100,0);
 printf("\n\n\n%s\n",hello);

 recv(socket1,closed,100,0);
 printf("Total number of clients on this server: %s\n",closed);

 while(1)
 {
  bzero(buffer,255);
  printf("\n");
 
  /* Getting the menu list from server*/
  n=read(socket1,buffer,255);
 
  if (n < 0)
  {
   report_error("Reading socket error");
  }
  printf("%s \n",buffer);
  /* Entering the command*/
  bzero(buff,100);
  printf("\n");
  printf("Enter the commands:\n ");
  fgets(buff,100,stdin);
  n=write(socket1,buff,strlen(buff)); //Writing that option to the server buff
 
  if (n < 0)
  {
   report_error("Writing to socket error");
  }

  printf("\nCommand output received\n");
  /* If command is 'quit' */
  if(strncmp(buff,"quit",4)==0)
  {
   bzero(buff,100);
   read(socket1,buff,100); //Getting reply from server
   printf("Reply from the server is: %s\n",buff);
   close(socket1); //Closing the socket
   exit(0); //Exit if successful
  }
 }
 close(socket1);
 exit(0);
}
