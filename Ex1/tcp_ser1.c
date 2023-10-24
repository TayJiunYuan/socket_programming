/**********************************
tcp_ser.c: the source file of the server in tcp transmission
***********************************/


#include "headsock.h"

#define BACKLOG 10

void str_ser(int sockfd);                                                        // transmitting and receiving function
int main(void)
{
	int sockfd, con_fd, ret;
	struct sockaddr_in my_addr;
	struct sockaddr_in their_addr;
	int sin_size;

	pid_t pid;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);          //create socket, SOCK_STREAM for TCP 
	if (sockfd <0)
	{
		printf("error in socket!");
		exit(1);
	}
	
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(MYTCP_PORT);				//port number
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	bzero(&(my_addr.sin_zero), 8);
	ret = bind(sockfd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr));                //bind socket aka associating the socket with a ip addr and port number
	if (ret <0)
	{
		printf("error in binding");
		exit(1);
	}
	
	ret = listen(sockfd, BACKLOG);                              //listen, backlong is the number of incoming requests that can be queued
	if (ret <0) {
		printf("error in listening");
		exit(1);
	}

	printf("receiving start\n");

	while (1)
	{
		sin_size = sizeof (struct sockaddr_in);
		con_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);            //accept the incoming connection from clients, opens an accepted socket
		if (con_fd <0)
		{
			printf("error in accept\n");
			exit(1);
		}

		if ((pid = fork())==0)                                         //In child process
		{
			close(sockfd);																						//Close the listening socket in the child process
			str_ser(con_fd);                                          //receive packet and response
			close(con_fd);																						//Close the accepted socket in the child process
			exit(0);
		}
		else close(con_fd);                                         //Close the accepted socket in the parent process
	}
	close(sockfd);
	exit(0);
}

void str_ser(int sockfd)
{
	char recvs[MAXSIZE];
	int n = 0;

	if ((n= recv(sockfd, &recvs, MAXSIZE, 0))==-1)                                   //receive the packet
	{
		printf("receiving error!\n");
		return;
	}
	recvs[n] = '\0';
	printf("the received string:\n%s\n", recvs);
}
