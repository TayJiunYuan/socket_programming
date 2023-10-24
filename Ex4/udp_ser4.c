/**************************************
udp_ser.c: the source file of the server in UDP transmission
**************************************/
#include "headsock.h" // Include a custom header file named "headsock.h"

// Function to handle the UDP server's data transmission and reception
void str_ser1(int sockfd, struct sockaddr *addr, int addrlen);
int probability_negative_ack(int percentage);


int main(int argc, char *argv[])
{
	srand((unsigned)time(NULL));  //seed the random number generator 
	int sockfd;
	struct sockaddr_in my_addr;

	// Create a UDP socket
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		printf("error in socket");
		exit(1);
	}

	// Set up server address structure
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(MYUDP_PORT);
	my_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(my_addr.sin_zero), 8);

	// Bind the socket to the server address
	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)
	{
		printf("error in binding");
		exit(1);
	}

	printf("start receiving\n");
	// Call the function to send and receive data
	str_ser1(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr_in));

	close(sockfd);
	exit(0);
}


int probability_negative_ack(int percentage) {
    int random_number = rand();
    int threshold = RAND_MAX / 100 * percentage; // Calculate the threshold based on the desired percentage
    if (random_number < threshold) {
        // This code block will run approximately "percentage"% of the time
        return 1; // Negative acknowledgment
    } else {
        return 0; // Positive acknowledgment
    }
}

// Function to handle data reception and saving it to a file
void str_ser1(int sockfd, struct sockaddr *addr, int addrlen)
{
	char buf[BUFSIZE];	 // Buffer for receiving data
	FILE *fp;						 // File pointer for writing received data
	char recvs[DATALEN]; // Buffer for received data packets
	struct ack_so ack;	 // Acknowledgment structure
	int end, n = 0;
	long lseek = 0;
	end = 0;

	printf("receiving data!\n");

	while (!end)
	{
		n = recvfrom(sockfd, &recvs, DATALEN, 0, addr, (socklen_t *)&addrlen); // Receive a data packet
		printf("Received Packet \n");
		if (n == -1)
		{
			printf("error when receiving\n");
			exit(1);
		}

		if (recvs[n - 1] == '\0') // Check if it is the end of the file, if yes, end loop. Notes '\0' is 1 byte
		{
			end = 1;
			n--; // don't count the last byte
		}

		// Copy received data into the buffer
		memcpy((buf + lseek), recvs, n);
		lseek += n; // increment pointer in buffer by n
		printf("lseek: %ld\n", lseek);

		if (probability_negative_ack(ERRPROB))
		{
			lseek -=n; //override faulty packet with resent packet
			ack.num = 2;	//negative acknoledgement
			printf("Negative Acknoledgement\n");
			end = 0; //if last packet rejected, do not end loop
			
		}
		else 
		{
			ack.num = 1;
		}
		ack.len = 0;
		if ((n = sendto(sockfd, &ack, 2, 0, addr, addrlen)) == -1)
		{
			printf("Error Sending ACK\n"); // Send an acknowledgment
			// exit(1);
		}
	}

	if ((fp = fopen("myUDPreceive.txt", "wt")) == NULL)
	{
		printf("File doesn't exist\n");
		exit(0);
	}
	fwrite(buf, 1, lseek, fp); // Write received data into a file
	fclose(fp);
	printf("a file has been successfully received!\nthe total data received is %d bytes\n", (int)lseek);
}
