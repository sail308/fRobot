#ifndef COMM_H
#define COMM_H

#include <netinet/in.h>  
#include <sys/socket.h> 
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <errno.h>
using namespace std;

int init_server_socket();

#endif
