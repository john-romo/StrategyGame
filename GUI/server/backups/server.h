// server.h


#ifndef _SERVER_H
#define _SERVER_H

#include <stack>

#include "default.h"
#include "board.h"
#include "piece.h"
#include "heading.h"

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
#include <sys/time.h>
#include <sys/types.h>
#include <sys/epoll.h>




typedef struct _client{
	Player* player;
	int socket;
	sockaddr_in addr;
} Client;
Client* create_client(Player* player, int socket, sockaddr_in addr);

typedef struct _game{
	bool joinable;
	std::vector<Client*> clients;
} Game;
Game* create_game(int state);


void server();
void client_options(int clientSocket, sockaddr_in clientAddr);
char* read_message(int socket);
void write_response(int socket, int errorType);


void new_game(Game* game);
void run_game(Game* game);
void start_game();
void end_game();


struct sockaddr_in get_socket_addr(int port);
int get_server_socket(int port);






/////////////// STRUCT CONSTRUCTION /////////////////////////////

typedef struct _error{
	int type;
} Error;
Error* create_error(int type);




typedef struct _binmove{
	int startX;
	int startY;
	int endX;
	int endY;
} BinMove;
BinMove* create_binmove(int _sx, int _sy, int _ex, int _ey);



///////////////////// FROM CLIENT ///////////////////////////////

char process_client_message(Client* client, int* m);
char process_client_move(Client* client, int* m);
char process_client_placement(Client* client, int* m);


///////////////////// TO CLIENT /////////////////////////////////

void create_message_header(int* m, int type, int msgLength, int unitLength);
int* create_move_message(std::stack<BinMove*>* moves);
void send_move_message(std::stack<BinMove*>* moves);



/////////////// TEST CONVERSIONS ////////////////////////////////

typedef struct _movemsg{
	int size;
	BinMove* msg;
} MoveMsg;


MoveMsg* convert_move_message(int* m);
void convert_bin_move(BinMove* b, int* m);


//////////////// PRINT //////////////////////////////////////////

//void print_message(int* m);
void print_move_message(MoveMsg* m);
void print_move(BinMove* b);

void print_message(int* m, int size);


#endif

