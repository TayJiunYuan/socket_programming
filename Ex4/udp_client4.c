/*******************************
udp_client.c: the source file of the client in udp

PROGRAM OVERVIEW
Get Host Info
Initialize socket with Host Info
Send Packets and Receive Ack Loop:
	Store File into buffer
	Initilize Pointer to buffer
	Send packet at pointer and increment pointer by packet length
	Receive ACK
		If ACK negative, decrement pointer to resend packet
Calculate time taken

HOW TO RUN
gcc udp_client4.c -o udp_client4
./udp_client4 127.0.0.1
********************************/

#include "headsock.h" // Include a custom header file named "headsock.h"

// Function to send and receive data between client and server
float str_cli4(FILE *fp, int sockfd, struct sockaddr *addr, int addrlen, int *len);

// Function to calculate the time interval between two timeval structs
void tv_sub(struct timeval *out, struct timeval *in);

int main(int argc, char *argv[])
{
	int len, sockfd;
	struct sockaddr_in ser_addr; // socket address
	char **pptr;
	struct hostent *sh;
	struct in_addr **addrs; // internet address (part of socket address)
	float ti;								// time length of total transmission
	float rt;								// average transmission rate
	FILE *fp;

	// Check if the correct number of command-line arguments is provided
	if (argc != 2)
	{
		printf("parameters not match.");
		exit(0);
	}

	// Get host's information
	if ((sh = gethostbyname(argv[1])) == NULL) 
	{
		printf("error when gethostbyname");
		exit(0);
	}

	// Create a UDP socket
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0)
	{
		printf("error in socket");
		exit(1);
	}

	// Get the host's IP address information
	addrs = (struct in_addr **)sh->h_addr_list;  //extract the host's IP address from sh
	printf("canonical name: %s\n", sh->h_name);

	// Print socket information
	for (pptr = sh->h_aliases; *pptr != NULL; pptr++)
		printf("the aliases name is: %s\n", *pptr);
	switch (sh->h_addrtype)
	{
	case AF_INET:
		printf("AF_INET\n");
		break;
	default:
		printf("unknown addrtype\n");
		break;
	}

	// Initialize and specify server address structure to send to 
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port = htons(MYUDP_PORT);
	memcpy(&(ser_addr.sin_addr.s_addr), *addrs, sizeof(struct in_addr));  //copy the host's IP address into the socket
	bzero(&(ser_addr.sin_zero), 8);

	// Open the file for reading
	if ((fp = fopen("myfile.txt", "r+t")) == NULL)
	{
		printf("File doesn't exist\n");
		exit(0);
	}

	// Call the str_cli4 function to send and receive data
	ti = str_cli4(fp, sockfd, (struct sockaddr *)&ser_addr, sizeof(struct sockaddr_in), &len);
	rt = (len / (float)ti); // Calculate the average transmission rate
	printf("Time(ms): %.3f, Data sent(byte): %d\nData rate: %f (Kbytes/s)\n", ti, (int)len, rt);

	// Close the socket and the file
	close(sockfd);
	fclose(fp);
	exit(0);
}

// Function to send and receive data
float str_cli4(FILE *fp, int sockfd, struct sockaddr *addr, int addrlen, int *len)
{
	// Declare variables
	char *buf;
	long lsize, ci;
	char sends[DATALEN]; // Array of 500 chars
	struct ack_so ack;
	int n, slen, m;
	float time_inv = 0.0;
	struct timeval sendt, recvt;
	ci = 0; // points at element of the buffer
	// Move the file pointer to the end of the file to get its size
	fseek(fp, 0, SEEK_END);
	lsize = ftell(fp); // Get the current position (in bytes) within the file (at the end now)
	rewind(fp);				 // Move the pointer back to the front
	printf("The file length is %d bytes\n", (int)lsize);
	printf("The packet length is %d bytes\n", DATALEN);

	// Allocate memory to contain the whole file
	buf = (char *)malloc(lsize);
	if (buf == NULL)
	{
		exit(2);
	}
	// Copy the file into the buffer
	fread(buf, 1, lsize, fp);

	// The whole file is loaded in the buffer
	buf[lsize] = '\0'; // Append the end byte

	// Get the current time
	gettimeofday(&sendt, NULL);

	// Loop while transmitting data
	while (ci <= lsize)
	{
		if ((lsize + 1 - ci) <= DATALEN)
			slen = lsize + 1 - ci; // Adjust packet size if remaining bytes are less than 500
		else
			slen = DATALEN;

		// Copy slen (500) bytes of data from memory location buf + ci to sends
		memcpy(sends, (buf + ci), slen);

		// Send the data
		n = sendto(sockfd, &sends, slen, 0, addr, addrlen);
		printf("Packet sent\n");
		if (n == -1)
		{
			printf("send error!");
			exit(1);
		}

		ci += slen; // point to next packet to send

		// If acknowledgment is damaged, retransmit the packet
		m = recvfrom(sockfd, &ack, 2, 0, addr, (socklen_t *)&addrlen);
		printf("Ack Received\n");
		if (ack.num == 2)
		{
			printf("Negative Ack Received\n");
			ci -= slen; // point back to same packet to resend the packet
		}
	}

	// Calculate the time taken for the transmission
	gettimeofday(&recvt, NULL);
	*len = ci;							// Get the current time
	tv_sub(&recvt, &sendt); // Calculate the whole transmission time
	time_inv += (recvt.tv_sec) * 1000.0 + (recvt.tv_usec) / 1000.0;
	return (time_inv);
}

// Function to calculate the time interval between two timeval structs
void tv_sub(struct timeval *out, struct timeval *in)
{
	if ((out->tv_usec -= in->tv_usec) < 0)
	{
		--out->tv_sec;
		out->tv_usec += 1000000;
	}
	out->tv_sec -= in->tv_sec;
}