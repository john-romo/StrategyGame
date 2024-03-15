// client .h


#ifndef _CLIENT_H
#define _CLIENT_H


#include<arpa/inet.h>



#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <sys/wait.h>
#include <sys/stat.h>



#include "../default.h"
#include "../heading.h"


struct sockaddr_in get_socket_addr(int port);
int get_client_socket(const char* address);
void client(const char* address);


int* create_newgame_message(int color, int width, int pieces, int numplayers);
int* create_placement_message(int type, int color, int x, int y);
int* create_move_message(int color, int x, int y, int headingID);
int* create_cancel_message();
char* read_message(int socket);

void print_message(int* m, int size);

int get_crc(int* m, size_t len);

#endif
