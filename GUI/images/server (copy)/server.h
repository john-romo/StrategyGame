// server.h


#ifndef _SERVER_H
#define _SERVER_H

#include <stack>

#include "../default.h"
#include "../coms.h"
//#include "board.h"
//#include "piece.h"
//#include "heading.h"

#include <stdlib.h>
#include <stdio.h>
#include <array>
#include <unistd.h>
#include <netdb.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <algorithm>
#include <random>
#include <poll.h>
#include <mutex>
#include <atomic>
#include <csignal>
#include <errno.h>
#include <thread>
#include <queue>
#include <condition_variable>
#include <unordered_map>
#include <semaphore>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/epoll.h>



enum STATUS 
{
	READY,
	CONNECTED,
	DISCONNECTED,
	DEAD
};

enum VICTORY_STATUS
{
	PLAYING,
	WON,
	DRAW,
	LOST
};


// smartsock
typedef struct _smartsock{
	int socket;
	struct pollfd sockpoll[1];
} Smartsock;
Smartsock* create_smartsock(int sock);
bool check_smartsock(Smartsock* sock);
void delete_smartsock(Smartsock* s);
bool smart_read(Smartsock* sock, char* buff, int len);
bool smart_write(Smartsock* sock, char* buff, int len);


// client
typedef struct _client{
	std::atomic<int> connectionStatus{CONNECTED};
	std::atomic<int> victoryStatus{PLAYING};
	uint64_t id;
	uint64_t gameId;
	int color;
	Smartsock* socket;
	Smartsock* tickSocket;
} Client;
Client* create_client(int sock, int ticksock);
void set_client_connection_status(Client* c, int status);
int check_client_connection_status(Client* c);
void set_client_victory_status(Client* c, int status);
int check_client_victory_status(Client* c);
void delete_client(Client* c);


// game
typedef struct _game{
	std::atomic<int> status{CONNECTED};
	uint64_t id;
	std::array<Client*, NUM_COLORS> clients;	
} Game;
Game* create_game();
void set_game_status(Game* g, int status);
int check_game_status(Game* g);
void delete_game(Game* g);


// flag
typedef struct _flag{
	std::atomic<bool> shutdown{false};
	std::atomic<bool> disconnect{false};
	std::atomic<bool> movementPhase{false};
	std::atomic<bool> update{false};
} Flag;
Flag* create_flag();
bool check_flag(std::atomic<bool>& flag);
void set_flag(std::atomic<bool>& flag, bool setval);
void delete_flag(Flag* f);



// main
int main();
void get_client_request(Client* client);
void process_new_game_request(Client* client);
Client* get_connection(sockaddr_in* addr, socklen_t* len);


// initialize game
void new_game();
bool init_game(Game* newgame);

void clean_ongoingGames();
void clean_connectedClients();


// run game
void run_game();
int game_status_check();


// tick
void start_tick();
void tick();
void update();


// sockets
struct sockaddr_in get_socket_addr(int port);
int get_server_socket(int port);
void start_listen(int* sock, int port);


// io


// containers 
void ongoingGames_insert(Game* g);
Game* ongoingGames_at(uint64_t id);
void connectedClients_insert(Client* c);
Client* connectedClients_at(uint64_t id);
Client* readyClients_pop();
void readyClients_push(Client* c);

// garbage
void signal_clean();
void clean_games();
void clean_clients();


// utils
void sec_sleep(float period);
void print_message(int* msg, int size);

//game phases
void placement_phase();


// process message
void process_message(int* msg);
void process_initial_placement_message(int* msg);


#endif
