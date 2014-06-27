#include "comm.h"

int init_server_socket() {
	int sockfd = 0;
	struct sockaddr_in my_addr;  
    struct sockaddr_in remote_addr; 

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {  
    	std::cout<< "socket create failed!\n";  
        exit(1);  
    }  
      
    my_addr.sin_family = AF_INET;  
    my_addr.sin_port = htons(10000);  
    my_addr.sin_addr.s_addr = INADDR_ANY;  
    bzero(&(my_addr.sin_zero), 8);  
	unsigned int value = 0x1;
	setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,(void *)&value,sizeof(value));
    if (bind(sockfd, (struct sockaddr*) &my_addr, sizeof(struct sockaddr)) == -1) {  
        perror("bind error");  
		printf("strerror: %s\n", strerror(errno)); 
        exit(1);  
    }  
	if (listen(sockfd, 10) == -1) {  
        perror("listen error"); 
		printf("strerror: %s\n", strerror(errno));  
        exit(1);  
    }
	return sockfd;
}
