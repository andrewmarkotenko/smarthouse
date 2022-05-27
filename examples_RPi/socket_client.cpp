#include <stdio.h>
#include <string.h> //strlen
#include <sys/socket.h>
#include <arpa/inet.h>

int socket_open(int *socket_desc)
{
	//int socket_desc;
	struct sockaddr_in server;

	//Create socket
	*socket_desc = socket(AF_INET, SOCK_DGRAM, 0);
	if (*socket_desc == -1)
	{
		printf("Could not create socket\n");
		return 2;
	}

	server.sin_addr.s_addr = inet_addr("192.168.1.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(50007);

	//Connect to remote server
	if (connect(*socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		puts("connect error");
		return 1;
	}

	puts("Connected\n");

	//Send some data
	//puts("Data Send\n");

	return 0;
}

int socket_msg(int *socket_desc, const char *message)
{
	if (send(*socket_desc, message, strlen(message), 0) < 0)
	{
		puts("Send failed");
		return 1;
	}
	return 0;
}
