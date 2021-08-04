#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <stdarg.h>
#include <arpa/inet.h>

#include "../global.h"
#include "../mk_packet.h"

#define SERV_ADDRESS	"127.0.0.1"

#define	UNKNOWN	(-1)

char *packet_string[] = {
	"TW_LIVE",
	"TW_KILL",
	"TW_QUEUE",
	"EXIT"
};

int	packet_type[] = {
	TW_LIVE,
	TW_KILL,
	TW_QUEUE,
	UNKNOWN
};

int find_packet_type(char *string)
{
	int	i;

	for(i=0; (strcmp(packet_string[i],"EXIT")!=0); i++)
		if( strcmp(packet_string[i],string) == 0 )
			break;

	return( packet_type[i] );
}

int main(int argc, char **argv)
{
    int				sockfd;
    struct	sockaddr_in	servaddr;
    char			sendline[8192], recvline[8192];
    ssize_t			n;
    struct	header_normal	header;    


    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(LOBBY_PORT);
	
    if(inet_pton(AF_INET, SERV_ADDRESS, &servaddr.sin_addr) != 1)
	perror("inet_pton()"),exit(-1);


    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0))<0)
	perror("socket()"),exit(-1);


    if(argc == 1)
    {
	 header.body_size	= 0;
	 header.command		= TW_LIVE;
	 
         sendto(sockfd, &header, 8, 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
    }
    else if(argc == 2)
    {
		header.body_size	= 0;
		if( (header.command = find_packet_type(argv[1])) == UNKNOWN ){
			printf("bad argment.....\n");  
			exit(-1);
		}
		
		sendto(sockfd, &header, 8, 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
    }
    else if(argc == 3)
    {
		header.body_size	= 16;
		if( (header.command = find_packet_type(argv[1])) == UNKNOWN ){
			printf("bad argment.....\n");  
			exit(-1);
		}
		
		memcpy(sendline, &header, 8);
		strncpy(sendline+8, argv[2], 16);
		sendto(sockfd, sendline, 24, 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
    }
    else
    {
		printf("bad argment.....\n");  
		exit(-1);
    }
    
    while(1)
    {
		n = recvfrom(sockfd, recvline, 8191, 0, NULL, NULL);
		recvline[n] = 0; 
	 
		if(strcmp(recvline,"SEnd") == 0)
			break;
         
	 	printf("%s\n",recvline);

	 	if(strcmp(recvline,"FEnd")==0)
	   		break;
    }
    
    return(0);	
}

