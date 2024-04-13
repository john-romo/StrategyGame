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
#include <mutex>
#include <csignal>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <SDL2/SDL.h>

#include "board.h"
#include "player.h"
#include "test.h"
#include "task.h"
#include "default.h"
#include "coms.h"


typedef struct _flag{
	bool disconnect;
	bool shutdown;
	bool update;
	std::mutex mutex;
} Flag;


void sigint_handler(int signal);
void game(int color, SDL_Renderer* renderer);

void start_tick();


// phases
void placement_phase();


// process message
void process_message(int* msg);
void process_initial_placement_message(int* msg);


// create message
int* create_initial_placement_message();


// tick
void tick();
void process_tick(char ch);


// io
int* read_message();
void write_message(int* msg);
void split_read(char* m, int len);
void split_write(char* m, int len);


// init
void start_game(int color, SDL_Renderer* renderer);
void end_game();
Flag* init_flags();
struct sockaddr_in get_socket_addr(int port);
int get_client_socket();


// util
void set_flag(bool& flag, bool setval);
bool check_flag(bool flag);
void sec_sleep(float period);
void print_message(int* m, int size);
void print_flags();

int join_game(SDL_Renderer* renderer);



#endif
